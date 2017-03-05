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

#include "CHandgun.h"

#define PROJECTILE_BUFFER_SIZE 15

namespace SAMPLES {

	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The player node
	*/
	CHandgun::CHandgun(ILevel * parent, DE::ISceneNode * playerNode) : IWeapon(parent) {
		this->m_damage = 10;
		this->m_magazineSize = 12;
		this->m_maxMagazineSize = 12;
		this->m_remainingAmmo = 0;
		this->m_maxAmmo = 108;
		this->m_precision = 90;
		this->m_hitCount = 1;
		this->m_fireRate = 1.0f;
		this->m_fireDelay = 350.0f;
		this->m_reloadDelay = 800.0f;
		this->m_range = 100;

		this->m_fireSound.LoadFromFile("handgun_fire.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_fireSound.SetRelativeToListener(true);
		this->m_reloadSound.LoadFromFile("handgun_reload.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_reloadSound.SetRelativeToListener(true);

		this->m_projectiles.Allocate(PROJECTILE_BUFFER_SIZE);

		InitializeWeaponNode(playerNode);
		InitializeParticleEmitters();
	}

	/**
	* Initialize the weapon node
	* @param playerNode The player node to attach the weapon node to
	*/
	void CHandgun::InitializeWeaponNode(DE::ISceneNode * playerNode) {
		this->m_meshNode = new DE::CAnimatedMeshSceneNode(this->m_parent->GetScene()->GetScene(), playerNode, "handgun", "handgun.ms3d", DE::CVector3F(0.38f, -1.3f, 2.5f), DE::CVector3F(), DE::CVector3F(3.0f, 3.0f, 3.0f));
		this->m_meshNode->SetId(SCENE_TOP_LAYER_ID);
		this->m_meshNode->SetSaved(false);
		this->m_meshNode->SetEnabled(true);
		this->m_meshNode->SetShadowCaster(false);
		this->m_meshNode->SetMeshTexture(DE::TEXTURE_TYPE_DIFFUSE, "handgun_d.png", DE::PIXEL_FORMAT_A8R8G8B8, DE::IRenderer::GetRenderer()->GetDefaultTextureFilter());
		this->m_meshNode->SetMeshTexture(DE::TEXTURE_TYPE_NORMAL, "handgun_n.png", DE::PIXEL_FORMAT_A8R8G8B8, DE::IRenderer::GetRenderer()->GetDefaultTextureFilter());
		this->m_meshNode->SetMeshShadingType(DE::MESH_SHADING_FLAT);
		this->m_meshNode->SetMeshMaterial("default.mtl");
		playerNode->AddChild(this->m_meshNode);

		this->m_fireAnimation.first = 0;
		this->m_fireAnimation.second = 0;

		this->m_reloadAnimation.first = 0;
		this->m_reloadAnimation.second = 0;
	}

	/**
	* Initialize the particle emitters
	*/
	void CHandgun::InitializeParticleEmitters() {
		for(int i = 0; i < PROJECTILE_BUFFER_SIZE; i++) {
			DE::CParticleEmitter * emitter = this->m_parent->GetScene()->GetParticleSystem().InstanciateEmitter("default.dept", "environmentImpactEmitter");
			emitter->SetTextureId(1);
			emitter->SetActive(false);
			emitter->SetReusable(true);

			this->m_bulletImpactParticle.push_back(emitter);
		}
	}
}

