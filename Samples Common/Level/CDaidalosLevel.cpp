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

#include "CDaidalosLevel.h"
#include "../Enums.h"
#include "../3D/Trigger/CAreaTriggerEntity.h"
#include "../3D/Trigger/CDoorEntity.h"
#include "../3D/Trigger/CFanEntity.h"
#include "../3D/Trigger/CSwitchEntity.h"
#include "../3D/Trigger/CTeleporterEntity.h"
#include "../3D/Trigger/CArmorBonusEntity.h"
#include "../3D/Trigger/CHealthBonusEntity.h"
#include "../3D/Trigger/CHandgunAmmoEntity.h"
#include "../3D/Trigger/CShotgunAmmoEntity.h"
#include "../3D/Trigger/CFlashlightEntity.h"
#include "../3D/Trigger/CLadderEntity.h"
#include "../3D/Misc/CSecurityCameraEntity.h"
#include "../3D/Misc/CAlarmEntity.h"
#include "../3D/Enemy/CZombieEntity.h"
#include "../CFPSCamera.h"
#include <Core/IRenderer.h>

namespace SAMPLES {

	/**
	* Update the level
	* @param time The elapsed time
	*/
	void CDaidalosLevel::Update(const float time) {
		ILevel::Update(time);

		// Update the hud
		this->m_hudView->SetFlashlight(this->m_player->GetObjectValue(SAMPLES::PLAYER_OBJECT_FLASHLIGHT) >= 1 ? true : false);
		this->m_hudView->SetPosition(this->m_player->GetVisiblePosition());
		this->m_hudView->SetGravity(this->m_player->GetGravity().y);
		this->m_hudView->SetHealth(this->m_player->GetHealth());
		this->m_hudView->SetArmor(this->m_player->GetArmor());
		this->m_hudView->SetAmmo(this->m_player->GetCurrentWeaponMagazineAmmo(), this->m_player->GetCurrentWeaponStockAmmo());

		// Check for a loading call
		for(auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
			if((*it)->GetType() == ENTITY_TYPE_TELEPORTER && ((ITriggerEntity*)(*it).get())->CanDoAction()) {
				
				for(auto it2 = this->m_listeners.begin(); it2 != this->m_listeners.end(); ++it2) {
					(*it2)->OnTriggerNextLevel(((CTeleporterEntity*)(*it).get())->GetNextLevelName());
				}
			}
		}
	}

	/**
	* Do pre level loading process
	*/
	void CDaidalosLevel::PreLoadLevel() {

	}

	/**
	* Do post level loading process
	*/
	void CDaidalosLevel::PostLoadLevel() {
		this->m_entities.clear();

		// Create the FPS camera
		this->m_player = std::unique_ptr<IPlayer>(new CFPSCamera(this, this->m_activeCamera, this->m_scene->GetNode("flashlight")));

		// Load the doors
		std::vector<DE::ISceneNode*> doorNode = this->m_scene->GetNodeArray("door_entity");
		for(auto it = doorNode.begin(); it != doorNode.end(); it++) {
			SAMPLES::CDoorEntity * doorEntity = new SAMPLES::CDoorEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CDoorEntity>(doorEntity));
		}

		// Load the fans
		std::vector<DE::ISceneNode*> fanNode = this->m_scene->GetNodeArray("fan_entity");
		for(auto it = fanNode.begin(); it != fanNode.end(); it++) {
			SAMPLES::CFanEntity * fanEntity = new SAMPLES::CFanEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			fanEntity->DoAction();
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CFanEntity>(fanEntity));
		}

		// Load the switchs
		std::vector<DE::ISceneNode*> switchNode = this->m_scene->GetNodeArray("switch_entity");
		for(auto it = switchNode.begin(); it != switchNode.end(); it++) {
			SAMPLES::CSwitchEntity * switchEntity = new SAMPLES::CSwitchEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CSwitchEntity>(switchEntity));
		}

		/*if(switchNode.size() > 0) {
		switchNode[0]->AddTarget(fanNode[0].get();
		}*/

		// Load the teleporters
		std::vector<DE::ISceneNode*> teleporterNode = this->m_scene->GetNodeArray("teleporter_entity");
		for(auto it = teleporterNode.begin(); it != teleporterNode.end(); it++) {
			SAMPLES::CTeleporterEntity * teleporterEntity = new SAMPLES::CTeleporterEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CTeleporterEntity>(teleporterEntity));
		}

		// Load the alarms
		std::vector<DE::ISceneNode*> alarmNode = this->m_scene->GetNodeArray("alarm_entity");
		for(auto it = alarmNode.begin(); it != alarmNode.end(); it++) {
			SAMPLES::CAlarmEntity * alarmEntity = new SAMPLES::CAlarmEntity(this, (DE::CStaticMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CAlarmEntity>(alarmEntity));
		}

		// Load the armor bonus
		std::vector<DE::ISceneNode*> armorNode = this->m_scene->GetNodeArray("armor_bonus_entity");
		for(auto it = armorNode.begin(); it != armorNode.end(); it++) {
			SAMPLES::CArmorBonusEntity * armorBonusEntity = new SAMPLES::CArmorBonusEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CArmorBonusEntity>(armorBonusEntity));
		}

		// Load the health bonus
		std::vector<DE::ISceneNode*> healthNode = this->m_scene->GetNodeArray("health_bonus_entity");
		for(auto it = healthNode.begin(); it != healthNode.end(); it++) {
			SAMPLES::CHealthBonusEntity * healthBonusEntity = new SAMPLES::CHealthBonusEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CHealthBonusEntity>(healthBonusEntity));
		}

		// Load the handgun ammo
		std::vector<DE::ISceneNode*> handgunAmmoNode = this->m_scene->GetNodeArray("handgun_ammo_entity");
		for(auto it = handgunAmmoNode.begin(); it != handgunAmmoNode.end(); it++) {
			SAMPLES::CHandgunAmmoEntity * handgunAmmoEntity = new SAMPLES::CHandgunAmmoEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CHandgunAmmoEntity>(handgunAmmoEntity));
		}

		// Load the shotgun ammo
		std::vector<DE::ISceneNode*> shotgunAmmoNode = this->m_scene->GetNodeArray("shotgun_ammo_entity");
		for(auto it = shotgunAmmoNode.begin(); it != shotgunAmmoNode.end(); it++) {
			SAMPLES::CShotgunAmmoEntity * shotgunAmmoEntity = new SAMPLES::CShotgunAmmoEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CShotgunAmmoEntity>(shotgunAmmoEntity));
		}

		// Load the flashlight
		std::vector<DE::ISceneNode*> flashlightNode = this->m_scene->GetNodeArray("flashlight_entity");
		for(auto it = flashlightNode.begin(); it != flashlightNode.end(); it++) {
			SAMPLES::CFlashlightEntity * flashlightEntity = new SAMPLES::CFlashlightEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CFlashlightEntity>(flashlightEntity));
		}

		// Load the security cameras
		std::vector<DE::ISceneNode*> securityCameraNode = this->m_scene->GetNodeArray("security_camera_entity");
		for(auto it = securityCameraNode.begin(); it != securityCameraNode.end(); it++) {
			SAMPLES::CSecurityCameraEntity * securityCameraEntity = new SAMPLES::CSecurityCameraEntity(this, (DE::CStaticMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CSecurityCameraEntity>(securityCameraEntity));
		}

		// Load the ladders
		std::vector<DE::ISceneNode*> ladderNode = this->m_scene->GetNodeArray("ladder_entity");
		for(auto it = ladderNode.begin(); it != ladderNode.end(); it++) {
			SAMPLES::CLadderEntity * ladderEntity = new SAMPLES::CLadderEntity(this, (DE::CStaticMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CLadderEntity>(ladderEntity));
		}

		// Load the zombies
		std::vector<DE::ISceneNode*> zombieNode = this->m_scene->GetNodeArray("zombie_enemy_entity");
		for(auto it = zombieNode.begin(); it != zombieNode.end(); it++) {
			SAMPLES::CZombieEntity * zombieEntity = new SAMPLES::CZombieEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CZombieEntity>(zombieEntity));
		}

		// Load the area triggers
		std::vector<DE::ISceneNode*> areaTriggerNode = this->m_scene->GetNodeArray("area_trigger_entity");
		for(auto it = areaTriggerNode.begin(); it != areaTriggerNode.end(); it++) {
			SAMPLES::CAreaTriggerEntity * areaTriggerEntity = new SAMPLES::CAreaTriggerEntity(this, (*it));
			this->m_entities.push_back(std::unique_ptr<SAMPLES::CAreaTriggerEntity>(areaTriggerEntity));
		}

		// Load the hud
		DE::IRenderer * renderer = DE::IRenderer::GetRenderer();
		this->m_hudView = new CHudView(DE::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));
		this->m_hudView->SetDebugMode(false);
		this->m_hud->AddToMainView(this->m_hudView);


		// End of loading callback
		for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
			(*it)->OnLevelLoaded();
		}
	}
}
