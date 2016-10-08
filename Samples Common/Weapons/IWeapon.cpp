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

namespace SAMPLES {

	/**
	* Constructor
	* @param meshNode The weapon mesh node
	* @param scene The 3D scene
	*/
	IWeapon::IWeapon(DE::CAnimatedMeshSceneNode * meshNode, DE::C3DScene * scene) {
		this->m_state = WEAPON_STATE_NONE;
		this->m_damage = 0;
		this->m_magazineSize = 0;
		this->m_maxMagazineSize = 0;
		this->m_remainingAmmo = 0;
		this->m_maxAmmo = 0;
		this->m_precision = 100;
		this->m_hitCount = 1;
		this->m_fireRate = 1.0f;
		this->m_range = 100;

		this->m_meshNode = meshNode;
		this->m_scene = scene;
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
					SProjectile & projectile = this->m_projectiles.GetReferenceAt(projectileIndex);
					projectile.damage = this->m_damage;
					projectile.direction = this->m_scene->GetActiveCamera()->GetForwardVector();
					projectile.distance = this->m_range;
					projectile.position = this->m_scene->GetActiveCamera()->GetAbsolutePosition();
					this->m_activeProjectiles.push_back(projectileIndex);
				}
				
			}

			// Update the state
			this->m_state = WEAPON_STATE_NONE;
		}
	}

	/**
	* Reload the weapon
	*/
	void IWeapon::Reload() {
		// Start the animation
		if(this->m_state == WEAPON_STATE_NONE && this->m_magazineSize < this->m_maxMagazineSize && this->m_remainingAmmo > 0) {
			this->m_state = WEAPON_STATE_RELOAD;
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

		// Update the state
		if(this->m_state == WEAPON_STATE_FIRE && (this->m_meshNode == nullptr || (this->m_meshNode != nullptr && this->m_meshNode->GetAnimationState() == DE::MESH_ANIMATION_STATE_STOPPED))) {
			this->m_state = WEAPON_STATE_NONE;

		} else if(this->m_state == WEAPON_STATE_RELOAD && (this->m_meshNode == nullptr || (this->m_meshNode != nullptr && this->m_meshNode->GetAnimationState() == DE::MESH_ANIMATION_STATE_STOPPED))) {
			this->m_state = WEAPON_STATE_NONE;
			
			// Update the magazine size and the remaining ammo
			unsigned int ammoToShift = std::min(this->m_maxMagazineSize - this->m_magazineSize, this->m_remainingAmmo);
			this->m_magazineSize += ammoToShift;
			this->m_remainingAmmo -= ammoToShift;
		}

		// Update the live projectiles
		for(auto it = this->m_activeProjectiles.begin(); it != this->m_activeProjectiles.end(); ) {
			SProjectile & projectile = this->m_projectiles.GetReferenceAt((*it));

			DE::CVector3F collisionPoint;
			DE::CVector3F movement = projectile.direction * 200.0f * timeSecond;

			// If an impact is detected
			if(this->m_scene->GetCollisionPoint(projectile.position, movement, collisionPoint)) {
				// Generate a particle
				bool freeParticleEmitterFound = false;
				for(unsigned int index = 0; index < this->m_bulletImpactParticle.size() && !freeParticleEmitterFound; index++) {
					if(!this->m_bulletImpactParticle[index]->IsActive()) {
						freeParticleEmitterFound = true;
						this->m_bulletImpactParticle[index]->SetLife(0);
						this->m_bulletImpactParticle[index]->SetLifeMax(PARTICLE_LIFE);
						this->m_bulletImpactParticle[index]->SetActive(true);
						this->m_bulletImpactParticle[index]->SetPosition(collisionPoint - (projectile.direction * 0.05f));
					}
				}

				// Kill the projectile
				this->m_projectiles.Delete((*it));
				it = this->m_activeProjectiles.erase(it);

			} else {
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
