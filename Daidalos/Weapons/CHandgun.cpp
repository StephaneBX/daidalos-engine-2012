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

#include "CHandgun.hpp"

#define PROJECTILE_BUFFER_SIZE 15

namespace daidalos {

	/**
	* Constructor
	* @param parent The parent
	* @param playerNode The player node
	*/
	CHandgun::CHandgun(daidalosgameengine::ILevel * parent, daidalosengine::ISceneNode * playerNode) : IWeapon(parent) {
		// Initialize the weapon stats
		SetMagazineStats(12, 12);
		SetAmmoStats(0, 108);
		SetOffensiveStats(10, 1, 90, 100);
		SetFireStats(5.0F, 350.0F);
		SetReloadStats(1.0F, 800.0F);

		// Initialize the sounds
		SetFireSound("handgun_fire.wav");
		SetReloadSound("handgun_reload.wav");

		// Initialize the projectiles & particles
		InitializeProjectileBuffer(PROJECTILE_BUFFER_SIZE);
		InitializeWeaponNode(playerNode);
		InitializeParticleEmitters();
		InitializeFireSmokeEmitter();
		InitializeFireFlashEmitter();
	}

	/**
	* Destructor
	*/
	CHandgun::~CHandgun() {}

	/**
	* Initialize the weapon node
	* @param playerNode The player node to attach the weapon node to
	*/
	void CHandgun::InitializeWeaponNode(daidalosengine::ISceneNode * playerNode) {
		SetMeshNode(new daidalosengine::CAnimatedMeshSceneNode(GetParent()->GetScene()->GetScene(), playerNode, "handgun", "handgun.ms3d", daidalosengine::CVector3F(0.38F, -1.3F, 2.5F), daidalosengine::CVector3F(), daidalosengine::CVector3F(3.0F, 3.0F, 3.0F)));
		GetMeshNode()->SetId(SCENE_TOP_LAYER_ID);
		GetMeshNode()->SetSaved(false);
		GetMeshNode()->SetEnabled(true);
		GetMeshNode()->SetShadowCaster(false);
		GetMeshNode()->SetMeshTexture(daidalosengine::TEXTURE_TYPE_DIFFUSE, "handgun_d.png", daidalosengine::PIXEL_FORMAT_A8R8G8B8, daidalosengine::IRenderer::GetRenderer()->GetDefaultTextureFilter());
		GetMeshNode()->SetMeshTexture(daidalosengine::TEXTURE_TYPE_NORMAL, "handgun_n.png", daidalosengine::PIXEL_FORMAT_A8R8G8B8, daidalosengine::IRenderer::GetRenderer()->GetDefaultTextureFilter());
		GetMeshNode()->SetMeshShadingType(daidalosengine::MESH_SHADING_FLAT);
		GetMeshNode()->SetMeshMaterial("default.mtl");
		playerNode->AddChild(GetMeshNode());

		// Initialize animations
		SetFireAnimation(1, 3);
		SetReloadAnimation(4, 6);

		// Initialize the light effect
		SetFireLight(new daidalosengine::CSpotLightSceneNode(GetParent()->GetScene()->GetScene(), GetMeshNode(), "handgun_light", daidalosengine::CVector3F(0, 0, 2), daidalosengine::CVector3F(), daidalosengine::CColor(0.7F, 0.7F, 0.7F), 20.0F, 45.0F, 0.3F, 0.3F, false, false));
	}

	/**
	* Initialize the particle emitters
	*/
	void CHandgun::InitializeParticleEmitters() {
		for(int i = 0; i < PROJECTILE_BUFFER_SIZE; i++) {
			daidalosengine::CParticleEmitter * emitter = GetParent()->GetScene()->GetParticleSystem().InstanciateEmitter("default.dept", "environmentImpactEmitter");
			emitter->SetTextureId(1);
			emitter->SetActive(false);
			emitter->SetReusable(true);
			AddParticleEmitter(emitter);
		}
	}

	/**
	* Initialize the smoke emitter
	*/
	void CHandgun::InitializeFireSmokeEmitter() {
		daidalosengine::CParticleEmitter * emitter = GetParent()->GetScene()->GetParticleSystem().InstanciateEmitter("default.dept", "weaponSmokeEmitter");
		emitter->SetTextureId(3);
		emitter->SetReusable(true);

		daidalosengine::CParticleEmitterSceneNode * node = new daidalosengine::CParticleEmitterSceneNode(GetParent()->GetScene()->GetScene(), GetMeshNode(), "weaponSmokeEmitter", emitter);
		node->SetPosition(daidalosengine::CVector3F(0, 0, 1.4F));
		SetSmokeEmitter(node);
	}

	/**
	* Initialize the flash emitter
	*/
	void CHandgun::InitializeFireFlashEmitter() {
		daidalosengine::CParticleEmitter * emitter = GetParent()->GetScene()->GetParticleSystem().InstanciateEmitter("default.dept", "weaponFlashEmitter");
		emitter->SetTextureId(1);
		emitter->SetReusable(true);

		daidalosengine::CParticleEmitterSceneNode * node = new daidalosengine::CParticleEmitterSceneNode(GetParent()->GetScene()->GetScene(), GetMeshNode(), "weaponFlashEmitter", emitter);
		node->SetPosition(daidalosengine::CVector3F(0, 0, 1.4F));
		SetFlashEmitter(node);
	}
}

