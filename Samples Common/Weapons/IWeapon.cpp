/*
==============================================================================================================================================================

Author: Stephane Baudoux
Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/

#include <set>
#include <Scene/3D/CSphere.h>
#include "IWeapon.h"

#define PARTICLE_LIFE 50
#define PARTICLE_DIRECTION_DECAL 0.08f

namespace SAMPLES {

	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The weapon mesh node
	*/
	IWeapon::IWeapon(ILevel * parent, DE::CAnimatedMeshSceneNode * meshNode) {
		this->m_state = WEAPON_STATE_NONE;
		this->m_damage = 0;
		this->m_magazineSize = 0;
		this->m_maxMagazineSize = 0;
		this->m_remainingAmmo = 0;
		this->m_maxAmmo = 0;
		this->m_precision = 90;
		this->m_hitCount = 1;
		this->m_fireRate = 1.0f;
		this->m_fireDelay = 0;
		this->m_reloadDelay = 0;
		this->m_remainingDelay = 0;
		this->m_range = 100;

		this->m_meshNode = meshNode;
		this->m_parent = parent;
	}

	/**
	* Fire the weapon
	*/
	void IWeapon::Fire() {

		// Only if there is ammo left in the magazine
		if(this->m_state == WEAPON_STATE_NONE && this->m_magazineSize > 0) {
			this->m_magazineSize--;

			// Play the sound
			this->m_fireSound.SetStatus(DE::SOUND_STATUS_PLAY);

			// Start the animation
			if(this->m_meshNode != nullptr) {
				this->m_meshNode->StartAnimation(this->m_fireAnimation.first, this->m_fireAnimation.second, this->m_fireRate);
			}

			// Generate the bullets
			for(unsigned int bullet = 0; bullet < this->m_hitCount; bullet++) {

				int projectileIndex = this->m_projectiles.New();
				if(projectileIndex > -1) {

					// Add a direction variation
					DE::CVector3F directionAltered = this->m_parent->GetScene()->GetActiveCamera()->GetForwardVector();
					directionAltered.x += DE::RandomNumber<float>(-(100 - (float)this->m_precision) / 400.0f, (100 - (float)this->m_precision) / 400.0f);
					directionAltered.y += DE::RandomNumber<float>(-(100 - (float)this->m_precision) / 400.0f, (100 - (float)this->m_precision) / 400.0f);
					directionAltered.z += DE::RandomNumber<float>(-(100 - (float)this->m_precision) / 400.0f, (100 - (float)this->m_precision) / 400.0f);
					directionAltered = directionAltered.Normalize();

					// Create the projectile
					SProjectile & projectile = this->m_projectiles.GetReferenceAt(projectileIndex);
					projectile.damage = this->m_damage;
					projectile.direction = directionAltered;
					projectile.distance = this->m_range;
					projectile.position = this->m_parent->GetScene()->GetActiveCamera()->GetAbsolutePosition();
					this->m_activeProjectiles.push_back(projectileIndex);
				}
				
			}

			// Update the state and the delay
			this->m_state = WEAPON_STATE_FIRE;
			this->m_remainingDelay = this->m_fireDelay;
		}
	}

	/**
	* Reload the weapon
	*/
	void IWeapon::Reload() {
		// Start the animation
		if(this->m_state == WEAPON_STATE_NONE && this->m_magazineSize < this->m_maxMagazineSize && this->m_remainingAmmo > 0) {
			this->m_state = WEAPON_STATE_RELOAD;
			this->m_remainingDelay = this->m_reloadDelay;
			this->m_reloadSound.SetStatus(DE::SOUND_STATUS_PLAY);
		}
	}

	/**
	* Add ammo
	* @param ammo The ammo count
	*/
	void IWeapon::AddAmmo(const unsigned int ammo) {
		this->m_remainingAmmo += ammo;
		if(this->m_remainingAmmo > this->m_maxAmmo) {
			this->m_remainingAmmo = this->m_maxAmmo;
		}
	}

	/**
	* Update the weapon
	* @param time elapsed time
	*/
	void IWeapon::Update(const float time) {

		const float timeSecond = time / 1000.0f;

		if(this->m_state == WEAPON_STATE_FIRE || this->m_state == WEAPON_STATE_RELOAD) {
			this->m_remainingDelay -= time;
		}

		// Update the state
		if(this->m_state == WEAPON_STATE_FIRE && this->m_remainingDelay <= 0 && (this->m_meshNode == nullptr || (this->m_meshNode != nullptr && this->m_meshNode->GetAnimationState() == DE::MESH_ANIMATION_STATE_STOPPED))) {
			this->m_state = WEAPON_STATE_NONE;

		} else if(this->m_state == WEAPON_STATE_RELOAD && this->m_remainingDelay <= 0 && (this->m_meshNode == nullptr || (this->m_meshNode != nullptr && this->m_meshNode->GetAnimationState() == DE::MESH_ANIMATION_STATE_STOPPED))) {
			this->m_state = WEAPON_STATE_NONE;
			
			// Update the magazine size and the remaining ammo
			unsigned int ammoToShift = std::min(this->m_maxMagazineSize - this->m_magazineSize, this->m_remainingAmmo);
			this->m_magazineSize += ammoToShift;
			this->m_remainingAmmo -= ammoToShift;
		}

		// Update the live projectiles
		for(auto it = this->m_activeProjectiles.begin(); it != this->m_activeProjectiles.end(); ) {
			SProjectile & projectile = this->m_projectiles.GetReferenceAt((*it));

			bool hit = false;
			DE::CVector3F collisionPoint;
			DE::CVector3F movement = projectile.direction * 200.0f * timeSecond;

			// Check for impact on the entities
			for(auto it2 = this->m_parent->GetEntities().begin(); it2 != this->m_parent->GetEntities().end() && !hit; ++it2) {
				if((*it2)->GetCollisionState() && (*it2)->GetCollisionShape() != nullptr) {

					// If the projectile movement is greater or equal than the projectile/entity distance
					if(((*it2)->GetVisiblePosition() - projectile.position).Length() <= movement.Length()) {
						for(int i = 1; i < (int)movement.Length() + 1 && !hit; i++) {
							DE::CVector3F movementStep = (movement * (float)i) / movement.Length();

							// Collision check
							if((*it2)->GetCollisionShape()->IsCollision(projectile.position + movementStep)) {

								// Take damage and generate entity specific particles
								(*it2)->TakeDamage(projectile.damage);
								(*it2)->GenerateImpactParticles(projectile.position + movementStep - (projectile.direction * PARTICLE_DIRECTION_DECAL));
								hit = true;

								// Kill the projectile
								this->m_projectiles.Delete((*it));
								it = this->m_activeProjectiles.erase(it);
							}
						}
					}
				}
			}

			// If an impact is detected on the static part
			if(!hit && this->m_parent->GetScene()->GetCollisionPoint(projectile.position, movement, collisionPoint)) {
				hit = true;
				// Generate a default impact particle
				bool freeParticleEmitterFound = false;
				for(unsigned int index = 0; index < this->m_bulletImpactParticle.size() && !freeParticleEmitterFound; index++) {
					if(!this->m_bulletImpactParticle[index]->IsActive()) {
						freeParticleEmitterFound = true;
						this->m_bulletImpactParticle[index]->SetLife(0);
						this->m_bulletImpactParticle[index]->SetLifeMax(PARTICLE_LIFE);
						this->m_bulletImpactParticle[index]->SetActive(true);
						this->m_bulletImpactParticle[index]->SetPosition(collisionPoint - (projectile.direction * PARTICLE_DIRECTION_DECAL));
					}
				}

				// Kill the projectile
				this->m_projectiles.Delete((*it));
				it = this->m_activeProjectiles.erase(it);

			}

			if(!hit) {
				// The projectile is moving
				projectile.position += movement;
				projectile.distance -= movement.Length();

				if(projectile.distance <= 0) {
					// Kill the projectile
					this->m_projectiles.Delete((*it));
					it = this->m_activeProjectiles.erase(it);
				} else {
					it++;
				}
			}
		}
	}

	/**
	* Get the magazine size
	* @return The magazine size
	*/
	const unsigned int IWeapon::GetMagazineSize() const {
		return this->m_magazineSize;
	}

	/**
	* Get the remaining ammo
	* @return The remaining ammo
	*/
	const unsigned int IWeapon::GetRemainingAmmo() const {
		return this->m_remainingAmmo;
	}
}
