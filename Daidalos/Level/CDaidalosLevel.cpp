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
#include <Core/IRenderer.h>
#include <Enums.hpp>
#include <3D/Trigger/CAreaTriggerEntity.hpp>
#include <3D/Trigger/CSwitchEntity.hpp>
#include <3D/Trigger/CDoorEntity.hpp>
#include <3D/Trigger/CLadderEntity.hpp>
#include <3D/Trigger/CCinematicEntity.hpp>
#include "CDaidalosLevel.hpp"
#include "../3D/Trigger/CFanEntity.hpp"
#include "../3D/Trigger/CTeleporterEntity.hpp"
#include "../3D/Trigger/CArmorBonusEntity.hpp"
#include "../3D/Trigger/CHealthBonusEntity.hpp"
#include "../3D/Trigger/CHandgunAmmoEntity.hpp"
#include "../3D/Trigger/CShotgunAmmoEntity.hpp"
#include "../3D/Trigger/CFlashlightEntity.hpp"
#include "../3D/Trigger/CMagneticCardEntity.hpp"
#include "../3D/Trigger/CMagneticCardReaderEntity.hpp"
#include "../3D/Misc/CSecurityCameraEntity.hpp"
#include "../3D/Misc/CAlarmEntity.hpp"
#include "../3D/Misc/CSpotEntity.hpp"
#include "../3D/Enemy/CZombieEntity.hpp"
#include "../CFPSCamera.hpp"

namespace DAIDALOS {

	/**
	* Update the level
	* @param time The elapsed time
	*/
	void CDaidalosLevel::Update(const float time) {
		ILevel::Update(time);

		// Update the hud
		this->m_hudView->SetFlashlight(this->m_player->GetObjectValue(PLAYER_OBJECT_FLASHLIGHT) >= 1 ? true : false);
		this->m_hudView->SetPosition(this->m_player->GetVisiblePosition());
		this->m_hudView->SetGravity(this->m_player->GetGravity().y);
		this->m_hudView->SetHealth(this->m_player->GetHealth());
		this->m_hudView->SetArmor(this->m_player->GetArmor());
		this->m_hudView->SetAmmo(this->m_player->GetCurrentWeaponMagazineAmmo(), this->m_player->GetCurrentWeaponStockAmmo());

		// Check for a loading call
		for(auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
			if((*it)->GetType() == GE::ENTITY_TYPE_TELEPORTER && ((GE::ITriggerEntity*)(*it).get())->CanDoAction()) {
				
				for(auto it2 = this->m_listeners.begin(); it2 != this->m_listeners.end(); ++it2) {
					(*it2)->OnTriggerNextLevel(((CTeleporterEntity*)(*it).get())->GetNextLevelName());
				}
			}
		}
	}

	/**
	* Callback when the cinematic event started
	* @param cinematicEvent The event
	*/
	void CDaidalosLevel::OnCinematicEventStarted(GE::CCinematicEvent * cinematicEvent) {
		this->m_hudView->SetVisible(false);
	}

	/**
	* Callback when the cinematic event ended
	* @param cinematicEvent The event
	*/
	void CDaidalosLevel::OnCinematicEventEnded(GE::CCinematicEvent * cinematicEvent) {
		this->m_hudView->SetVisible(true);
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
		this->m_player = std::unique_ptr<GE::IPlayer>(new CFPSCamera(this, this->m_activeCamera, this->m_scene->GetNode("flashlight")));

		// Load the doors
		std::vector<DE::ISceneNode*> doorNode = this->m_scene->GetNodeArray("door_entity");
		for(auto it = doorNode.begin(); it != doorNode.end(); it++) {
			GE::CDoorEntity * doorEntity = new GE::CDoorEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<GE::CDoorEntity>(doorEntity));
		}

		// Load the fans
		std::vector<DE::ISceneNode*> fanNode = this->m_scene->GetNodeArray("fan_entity");
		for(auto it = fanNode.begin(); it != fanNode.end(); it++) {
			CFanEntity * fanEntity = new CFanEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			fanEntity->DoAction();
			this->m_entities.push_back(std::unique_ptr<CFanEntity>(fanEntity));
		}

		// Load the switchs
		std::vector<DE::ISceneNode*> switchNode = this->m_scene->GetNodeArray("switch_entity");
		for(auto it = switchNode.begin(); it != switchNode.end(); it++) {
			GE::CSwitchEntity * switchEntity = new GE::CSwitchEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<GE::CSwitchEntity>(switchEntity));
		}

		/*if(switchNode.size() > 0) {
		switchNode[0]->AddTarget(fanNode[0].get();
		}*/

		// Load the teleporters
		std::vector<DE::ISceneNode*> teleporterNode = this->m_scene->GetNodeArray("teleporter_entity");
		for(auto it = teleporterNode.begin(); it != teleporterNode.end(); it++) {
			CTeleporterEntity * teleporterEntity = new CTeleporterEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CTeleporterEntity>(teleporterEntity));
		}

		// Load the alarms
		std::vector<DE::ISceneNode*> alarmNode = this->m_scene->GetNodeArray("alarm_entity");
		for(auto it = alarmNode.begin(); it != alarmNode.end(); it++) {
			CAlarmEntity * alarmEntity = new CAlarmEntity(this, (DE::CStaticMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CAlarmEntity>(alarmEntity));
		}

		// Load the armor bonus
		std::vector<DE::ISceneNode*> armorNode = this->m_scene->GetNodeArray("armor_bonus_entity");
		for(auto it = armorNode.begin(); it != armorNode.end(); it++) {
			CArmorBonusEntity * armorBonusEntity = new CArmorBonusEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CArmorBonusEntity>(armorBonusEntity));
		}

		// Load the health bonus
		std::vector<DE::ISceneNode*> healthNode = this->m_scene->GetNodeArray("health_bonus_entity");
		for(auto it = healthNode.begin(); it != healthNode.end(); it++) {
			CHealthBonusEntity * healthBonusEntity = new CHealthBonusEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CHealthBonusEntity>(healthBonusEntity));
		}

		// Load the handgun ammo
		std::vector<DE::ISceneNode*> handgunAmmoNode = this->m_scene->GetNodeArray("handgun_ammo_entity");
		for(auto it = handgunAmmoNode.begin(); it != handgunAmmoNode.end(); it++) {
			CHandgunAmmoEntity * handgunAmmoEntity = new CHandgunAmmoEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CHandgunAmmoEntity>(handgunAmmoEntity));
		}

		// Load the shotgun ammo
		std::vector<DE::ISceneNode*> shotgunAmmoNode = this->m_scene->GetNodeArray("shotgun_ammo_entity");
		for(auto it = shotgunAmmoNode.begin(); it != shotgunAmmoNode.end(); it++) {
			CShotgunAmmoEntity * shotgunAmmoEntity = new CShotgunAmmoEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CShotgunAmmoEntity>(shotgunAmmoEntity));
		}

		// Load the flashlight
		std::vector<DE::ISceneNode*> flashlightNode = this->m_scene->GetNodeArray("flashlight_entity");
		for(auto it = flashlightNode.begin(); it != flashlightNode.end(); it++) {
			CFlashlightEntity * flashlightEntity = new CFlashlightEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CFlashlightEntity>(flashlightEntity));
		}

		// Load the security cameras
		std::vector<DE::ISceneNode*> securityCameraNode = this->m_scene->GetNodeArray("security_camera_entity");
		for(auto it = securityCameraNode.begin(); it != securityCameraNode.end(); it++) {
			CSecurityCameraEntity * securityCameraEntity = new CSecurityCameraEntity(this, (DE::CStaticMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CSecurityCameraEntity>(securityCameraEntity));
		}

		// Load the ladders
		std::vector<DE::ISceneNode*> ladderNode = this->m_scene->GetNodeArray("ladder_entity");
		for(auto it = ladderNode.begin(); it != ladderNode.end(); it++) {
			GE::CLadderEntity * ladderEntity = new GE::CLadderEntity(this, (DE::CStaticMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<GE::CLadderEntity>(ladderEntity));
		}

		// Load the zombies
		std::vector<DE::ISceneNode*> zombieNode = this->m_scene->GetNodeArray("zombie_enemy_entity");
		for(auto it = zombieNode.begin(); it != zombieNode.end(); it++) {
			CZombieEntity * zombieEntity = new CZombieEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CZombieEntity>(zombieEntity));
		}

		// Load the area triggers
		std::vector<DE::ISceneNode*> areaTriggerNode = this->m_scene->GetNodeArray("area_trigger_entity");
		for(auto it = areaTriggerNode.begin(); it != areaTriggerNode.end(); it++) {
			GE::CAreaTriggerEntity * areaTriggerEntity = new GE::CAreaTriggerEntity(this, (*it));
			this->m_entities.push_back(std::unique_ptr<GE::CAreaTriggerEntity>(areaTriggerEntity));
		}

		// Load the spots
		std::vector<DE::ISceneNode*> spotNode = this->m_scene->GetNodeArray("spot_entity");
		for(auto it = spotNode.begin(); it != spotNode.end(); it++) {
			CSpotEntity * spotEntity = new CSpotEntity(this, (DE::CStaticMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CSpotEntity>(spotEntity));
		}

		// Load the magnetic cards
		std::vector<DE::ISceneNode*> magneticCardNode = this->m_scene->GetNodeArray("magnetic_card_entity");
		for(auto it = magneticCardNode.begin(); it != magneticCardNode.end(); it++) {
			CMagneticCardEntity * magneticCardEntity = new CMagneticCardEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CMagneticCardEntity>(magneticCardEntity));
		}

		// Load the magnetic card readers
		std::vector<DE::ISceneNode*> magneticCardReaderNode = this->m_scene->GetNodeArray("magnetic_card_reader_entity");
		for(auto it = magneticCardReaderNode.begin(); it != magneticCardReaderNode.end(); it++) {
			CMagneticCardReaderEntity * magneticCardReaderEntity = new CMagneticCardReaderEntity(this, (DE::CStaticMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CMagneticCardReaderEntity>(magneticCardReaderEntity));
		}

		// Load the cinematic triggers
		std::vector<DE::ISceneNode*> cinematicTriggerNode = this->m_scene->GetNodeArray("cinematic_trigger_entity");
		for (auto it = cinematicTriggerNode.begin(); it != cinematicTriggerNode.end(); it++) {
			GE::CCinematicEntity * cinematicTriggerEntity = new GE::CCinematicEntity(this, (DE::CStaticMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<GE::CCinematicEntity>(cinematicTriggerEntity));
		}
		

		// Load the hud
		DE::IRenderer * renderer = DE::IRenderer::GetRenderer();
		this->m_hudView = new CHudView(DE::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));
		this->m_hudView->SetDebugMode(false);
		this->m_view->AddToMainView(this->m_hudView);


		// End of loading callback
		for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
			(*it)->OnLevelLoaded();
		}
	}
}
