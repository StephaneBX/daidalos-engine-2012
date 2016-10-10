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

#include <sstream>
#include "Scene\Particles\Initializers\CColorParticleInitializer.h"
#include "Scene\Particles\Initializers\CLifeParticleInitializer.h"
#include "Scene\Particles\Initializers\CRotationParticleInitializer.h"
#include "Scene\Particles\Initializers\CSizeParticleInitializer.h"
#include "Scene\Particles\Initializers\CLinearVelocityParticleInitializer.h"
#include "Scene\Particles\Initializers\CSphereVelocityParticleInitializer.h"
#include "Scene\Particles\Updaters\CColorParticleUpdater.h"
#include "Scene\Particles\Updaters\CGravityParticleUpdater.h"
#include "Scene\Particles\Updaters\CLifeParticleUpdater.h"
#include "Scene\Particles\Updaters\CRotationParticleUpdater.h"
#include "Scene\Particles\Updaters\CSizeParticleUpdater.h"
#include "Scene\Particles\Updaters\CVelocityParticleUpdater.h"
#include "3D/CDoorEntity.h"
#include "3D/CFanEntity.h"
#include "3D/CSwitchEntity.h"
#include "3D/CTeleporterEntity.h"
#include "3D/CAlarmEntity.h"
#include "3D/CArmorBonusEntity.h"
#include "3D/CHealthBonusEntity.h"
#include "3D/CHandgunAmmoEntity.h"
#include "CSampleApplication.h"

namespace SAMPLE2 {
	CSampleApplication::CSampleApplication(const HINSTANCE hInstance, const int nCmdShow) : SAMPLES::IForwardSampleApplication(hInstance, nCmdShow) {

		this->m_scene2D->AddToMainView(this->m_hud);
		std::thread(&CSampleApplication::LoadLevel, this, "factory.des").detach();

		MainLoop();
	}

	CSampleApplication::~CSampleApplication() {}

	void CSampleApplication::Update(const float time) {

		if(!this->m_loading) {
			this->m_fpsCounter.Update(time);

			if(this->m_input->IsActionActive(SAMPLES::FPS_CAMERA_EXIT) || this->m_input->IsGamepadActionActive(0, SAMPLES::FPS_CAMERA_EXIT)) {
				this->m_alive = false;
			}

			this->m_player->Update(time);

			if(this->m_spotLight) {
				DE::CVector3F lightRotation = this->m_spotLight->GetRotation();
				lightRotation.y += (40 * time) / 1000.0f;
				this->m_spotLight->SetRotation(lightRotation);
			}

			// Update the 3D scene
			this->m_scene3D->SetActiveCamera(this->m_activeCamera);
			this->m_scene3D->UpdateAll(time);

			// Update the entities
			for(auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
				(*it)->Update(time);
			}

			// Update the hud
			this->m_hud->SetFPS(this->m_fpsCounter.GetText());
			this->m_hud->SetPosition(this->m_player->GetPosition());
			this->m_hud->SetGravity(this->m_player->GetGravity());
			this->m_hud->SetHealth(this->m_player->GetHealth());
			this->m_hud->SetArmor(this->m_player->GetArmor());
			this->m_hud->SetAmmo(this->m_player->GetCurrentWeaponMagazineAmmo(), this->m_player->GetCurrentWeaponStockAmmo());

			this->m_cursor.Update(time);
			this->m_scene2D->Update(time);

			// Check for a loading call
			for(auto it = this->m_entities.begin(); it != this->m_entities.end() && !this->m_loading; it++) {
				if((*it)->GetType() == SAMPLES::ENTITY_TYPE_TELEPORTER && (*it)->CanDoAction()) {
					std::thread(&CSampleApplication::LoadLevel, this, ((SAMPLES::CTeleporterEntity*)(*it).get())->GetNextLevelName()).detach();
				}
			}

		} else {
			this->m_loadingScene->Update(time);
		}
	}

	/**
	* Do post level loading process
	*/
	void CSampleApplication::PostLoadLevel() {
		this->m_player = std::unique_ptr<SAMPLES::CFPSCamera>(new SAMPLES::CFPSCamera(this->m_activeCamera));
		((SAMPLES::CFPSCamera*)this->m_player.get())->SetScene(this->m_scene3D);
		this->m_spotLight = (DE::CSpotLightSceneNode*)this->m_scene3D->GetNode("spotLight1");
		
		IForwardSampleApplication::PostLoadLevel();
	}
}
