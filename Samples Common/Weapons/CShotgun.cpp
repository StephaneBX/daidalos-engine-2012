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

#include "CShotgun.h"

#define PROJECTILE_BUFFER_SIZE 25

namespace SAMPLES {

	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The player node
	*/
	CShotgun::CShotgun(ILevel * parent, DE::ISceneNode * playerNode) : IWeapon(parent) {
		this->m_damage = 12;
		this->m_magazineSize = 7;
		this->m_maxMagazineSize = 7;
		this->m_remainingAmmo = 0;
		this->m_maxAmmo = 43;
		this->m_precision = 65;
		this->m_hitCount = 5;
		this->m_fireRate = 1.0f;
		this->m_fireDelay = 1200.0f;
		this->m_reloadDelay = 2800.0f;
		this->m_range = 100;

		this->m_fireSound.LoadFromFile("shotgun_fire.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_fireSound.SetRelativeToListener(true);
		this->m_reloadSound.LoadFromFile("shotgun_reload.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_reloadSound.SetRelativeToListener(true);

		this->m_projectiles.Allocate(PROJECTILE_BUFFER_SIZE);

		InitializeWeaponNode(playerNode);
		InitializeParticleEmitters();
		InitializeFireSmokeEmitter();
	}

	/**
	* Initialize the weapon node
	* @param playerNode The player node to attach the weapon node to
	*/
	void CShotgun::InitializeWeaponNode(DE::ISceneNode * playerNode) {
	
	}

	/**
	* Initialize the particle emitters
	*/
	void CShotgun::InitializeParticleEmitters() {
		for(int i = 0; i < PROJECTILE_BUFFER_SIZE; i++) {
			DE::CParticleEmitter * emitter = this->m_parent->GetScene()->GetParticleSystem().InstanciateEmitter("default.dept", "environmentImpactEmitter");
			emitter->SetTextureId(1);
			emitter->SetActive(false);
			emitter->SetReusable(true);

			this->m_bulletImpactParticle.push_back(emitter);
		}
	}

	/**
	* Initialize the smoke emitter
	*/
	void CShotgun::InitializeFireSmokeEmitter() {
		DE::CParticleEmitter * emitter = this->m_parent->GetScene()->GetParticleSystem().InstanciateEmitter("default.dept", "weaponSmokeEmitter");
		emitter->SetTextureId(1);
		emitter->SetReusable(true);

		this->m_smokeEmitter = new DE::CParticleEmitterSceneNode(this->m_parent->GetScene()->GetScene(), this->m_meshNode, "weaponSmokeEmitter", emitter);
		this->m_smokeEmitter->SetPosition(DE::CVector3F(0, 0, 2.0f));
		this->m_smokeEmitter->SetActive(false);
		this->m_meshNode->AddChild(this->m_smokeEmitter);
	}
}

