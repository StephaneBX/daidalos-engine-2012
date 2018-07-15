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

#include <Core/IRenderer.hpp>
#include <Enums.hpp>
#include <3D/Trigger/CAreaTriggerEntity.hpp>
#include <3D/Trigger/CSwitchEntity.hpp>
#include <3D/Trigger/CDoorEntity.hpp>
#include <3D/Trigger/CLadderEntity.hpp>
#include <3D/Trigger/CCinematicEntity.hpp>
#include <Scene/2D/CButton.hpp>
#include <Scene/2D/CCheckbox.hpp>
#include <Scene/2D/CScrollableView.hpp>
#include <Scene/2D/CSelectionList.hpp>
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

namespace daidalos {

	/**
	* Destructor
	*/
	CDaidalosLevel::~CDaidalosLevel() {}

	/**
	* Update the level
	* @param time The elapsed time
	*/
	void CDaidalosLevel::Update(const float time) {
		ILevel::Update(time);

		// Update the hud
		this->m_hudView->SetFlashlight(GetPlayer()->GetObjectValue(PLAYER_OBJECT_FLASHLIGHT) >= 1 ? true : false);
		this->m_hudView->SetPosition(GetPlayer()->GetVisiblePosition());
		this->m_hudView->SetGravity(GetPlayer()->GetGravity().y);
		this->m_hudView->SetHealth(GetPlayer()->GetHealth());
		this->m_hudView->SetArmor(GetPlayer()->GetArmor());
		this->m_hudView->SetAmmo(GetPlayer()->GetCurrentWeaponMagazineAmmo(), GetPlayer()->GetCurrentWeaponStockAmmo());

		// Check for a loading call
		for(auto it = GetEntities().begin(); it != GetEntities().end(); it++) {
			if((*it)->GetType() == daidalosgameengine::ENTITY_TYPE_TELEPORTER && ((daidalosgameengine::ITriggerEntity*)(*it).get())->CanDoAction()) {
				TriggerNextLevel(((CTeleporterEntity*)(*it).get())->GetNextLevelName());
			}
		}
	}

	/**
	* Callback when the cinematic event started
	* @param cinematicEvent The event
	*/
	void CDaidalosLevel::OnCinematicEventStarted(daidalosgameengine::CCinematicEvent * cinematicEvent) {
		this->m_hudView->SetVisible(false);
	}

	/**
	* Callback when the cinematic event ended
	* @param cinematicEvent The event
	*/
	void CDaidalosLevel::OnCinematicEventEnded(daidalosgameengine::CCinematicEvent * cinematicEvent) {
		this->m_hudView->SetVisible(true);
	}

	/**
	* Do pre level loading process
	*/
	void CDaidalosLevel::PreLoadLevel() {
		// Nothing to do
	}

	/**
	* Do post level loading process
	*/
	void CDaidalosLevel::PostLoadLevel() {
		GetEntities().clear();

		// Create the FPS camera
		SetPlayer(new CFPSCamera(this, GetActiveCamera(), GetScene()->GetNode("flashlight")));

		// Load the doors
		std::vector<daidalosengine::ISceneNode*> doorNode = GetScene()->GetNodeArray("door_entity");
		for(auto it = doorNode.begin(); it != doorNode.end(); it++) {
			daidalosgameengine::CDoorEntity * doorEntity = new daidalosgameengine::CDoorEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<daidalosgameengine::CDoorEntity>(doorEntity));
		}

		// Load the fans
		std::vector<daidalosengine::ISceneNode*> fanNode = GetScene()->GetNodeArray("fan_entity");
		for(auto it = fanNode.begin(); it != fanNode.end(); it++) {
			CFanEntity * fanEntity = new CFanEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			fanEntity->DoAction();
			GetEntities().push_back(std::unique_ptr<CFanEntity>(fanEntity));
		}

		// Load the switchs
		std::vector<daidalosengine::ISceneNode*> switchNode = GetScene()->GetNodeArray("switch_entity");
		for(auto it = switchNode.begin(); it != switchNode.end(); it++) {
			daidalosgameengine::CSwitchEntity * switchEntity = new daidalosgameengine::CSwitchEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<daidalosgameengine::CSwitchEntity>(switchEntity));
		}

		/*if(switchNode.size() > 0) {
		switchNode[0]->AddTarget(fanNode[0].get();
		}*/

		// Load the teleporters
		std::vector<daidalosengine::ISceneNode*> teleporterNode = GetScene()->GetNodeArray("teleporter_entity");
		for(auto it = teleporterNode.begin(); it != teleporterNode.end(); it++) {
			CTeleporterEntity * teleporterEntity = new CTeleporterEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CTeleporterEntity>(teleporterEntity));
		}

		// Load the alarms
		std::vector<daidalosengine::ISceneNode*> alarmNode = GetScene()->GetNodeArray("alarm_entity");
		for(auto it = alarmNode.begin(); it != alarmNode.end(); it++) {
			CAlarmEntity * alarmEntity = new CAlarmEntity(this, (daidalosengine::CStaticMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CAlarmEntity>(alarmEntity));
		}

		// Load the armor bonus
		std::vector<daidalosengine::ISceneNode*> armorNode = GetScene()->GetNodeArray("armor_bonus_entity");
		for(auto it = armorNode.begin(); it != armorNode.end(); it++) {
			CArmorBonusEntity * armorBonusEntity = new CArmorBonusEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CArmorBonusEntity>(armorBonusEntity));
		}

		// Load the health bonus
		std::vector<daidalosengine::ISceneNode*> healthNode = GetScene()->GetNodeArray("health_bonus_entity");
		for(auto it = healthNode.begin(); it != healthNode.end(); it++) {
			CHealthBonusEntity * healthBonusEntity = new CHealthBonusEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CHealthBonusEntity>(healthBonusEntity));
		}

		// Load the handgun ammo
		std::vector<daidalosengine::ISceneNode*> handgunAmmoNode = GetScene()->GetNodeArray("handgun_ammo_entity");
		for(auto it = handgunAmmoNode.begin(); it != handgunAmmoNode.end(); it++) {
			CHandgunAmmoEntity * handgunAmmoEntity = new CHandgunAmmoEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CHandgunAmmoEntity>(handgunAmmoEntity));
		}

		// Load the shotgun ammo
		std::vector<daidalosengine::ISceneNode*> shotgunAmmoNode = GetScene()->GetNodeArray("shotgun_ammo_entity");
		for(auto it = shotgunAmmoNode.begin(); it != shotgunAmmoNode.end(); it++) {
			CShotgunAmmoEntity * shotgunAmmoEntity = new CShotgunAmmoEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CShotgunAmmoEntity>(shotgunAmmoEntity));
		}

		// Load the flashlight
		std::vector<daidalosengine::ISceneNode*> flashlightNode = GetScene()->GetNodeArray("flashlight_entity");
		for(auto it = flashlightNode.begin(); it != flashlightNode.end(); it++) {
			CFlashlightEntity * flashlightEntity = new CFlashlightEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CFlashlightEntity>(flashlightEntity));
		}

		// Load the security cameras
		std::vector<daidalosengine::ISceneNode*> securityCameraNode = GetScene()->GetNodeArray("security_camera_entity");
		for(auto it = securityCameraNode.begin(); it != securityCameraNode.end(); it++) {
			CSecurityCameraEntity * securityCameraEntity = new CSecurityCameraEntity(this, (daidalosengine::CStaticMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CSecurityCameraEntity>(securityCameraEntity));
		}

		// Load the ladders
		std::vector<daidalosengine::ISceneNode*> ladderNode = GetScene()->GetNodeArray("ladder_entity");
		for(auto it = ladderNode.begin(); it != ladderNode.end(); it++) {
			daidalosgameengine::CLadderEntity * ladderEntity = new daidalosgameengine::CLadderEntity(this, (daidalosengine::CStaticMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<daidalosgameengine::CLadderEntity>(ladderEntity));
		}

		// Load the zombies
		std::vector<daidalosengine::ISceneNode*> zombieNode = GetScene()->GetNodeArray("zombie_enemy_entity");
		for(auto it = zombieNode.begin(); it != zombieNode.end(); it++) {
			CZombieEntity * zombieEntity = new CZombieEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CZombieEntity>(zombieEntity));
		}

		// Load the area triggers
		std::vector<daidalosengine::ISceneNode*> areaTriggerNode = GetScene()->GetNodeArray("area_trigger_entity");
		for(auto it = areaTriggerNode.begin(); it != areaTriggerNode.end(); it++) {
			daidalosgameengine::CAreaTriggerEntity * areaTriggerEntity = new daidalosgameengine::CAreaTriggerEntity(this, (*it));
			GetEntities().push_back(std::unique_ptr<daidalosgameengine::CAreaTriggerEntity>(areaTriggerEntity));
		}

		// Load the spots
		std::vector<daidalosengine::ISceneNode*> spotNode = GetScene()->GetNodeArray("spot_entity");
		for(auto it = spotNode.begin(); it != spotNode.end(); it++) {
			CSpotEntity * spotEntity = new CSpotEntity(this, (daidalosengine::CStaticMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CSpotEntity>(spotEntity));
		}

		// Load the magnetic cards
		std::vector<daidalosengine::ISceneNode*> magneticCardNode = GetScene()->GetNodeArray("magnetic_card_entity");
		for(auto it = magneticCardNode.begin(); it != magneticCardNode.end(); it++) {
			CMagneticCardEntity * magneticCardEntity = new CMagneticCardEntity(this, (daidalosengine::CAnimatedMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CMagneticCardEntity>(magneticCardEntity));
		}

		// Load the magnetic card readers
		std::vector<daidalosengine::ISceneNode*> magneticCardReaderNode = GetScene()->GetNodeArray("magnetic_card_reader_entity");
		for(auto it = magneticCardReaderNode.begin(); it != magneticCardReaderNode.end(); it++) {
			CMagneticCardReaderEntity * magneticCardReaderEntity = new CMagneticCardReaderEntity(this, (daidalosengine::CStaticMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<CMagneticCardReaderEntity>(magneticCardReaderEntity));
		}

		// Load the cinematic triggers
		std::vector<daidalosengine::ISceneNode*> cinematicTriggerNode = GetScene()->GetNodeArray("cinematic_trigger_entity");
		for (auto it = cinematicTriggerNode.begin(); it != cinematicTriggerNode.end(); it++) {
			daidalosgameengine::CCinematicEntity * cinematicTriggerEntity = new daidalosgameengine::CCinematicEntity(this, (daidalosengine::CStaticMeshSceneNode*)(*it));
			GetEntities().push_back(std::unique_ptr<daidalosgameengine::CCinematicEntity>(cinematicTriggerEntity));
		}
		
		// Load the hud
		daidalosengine::IRenderer * renderer = daidalosengine::IRenderer::GetRenderer();
		this->m_hudView = new CHudView(daidalosengine::CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));
		this->m_hudView->SetDebugMode(false);
		GetView()->AddToMainView(this->m_hudView);

		// TMP load views

		daidalosengine::CScrollableView * scroll = new daidalosengine::CScrollableView();
		scroll->SetBounds(daidalosengine::CRectangleI(100, 100, 150, 80));
		scroll->SetScrollBarThickness(5);
		GetView()->AddToMainView(scroll);

		daidalosengine::CCheckbox * checkbox1 = new daidalosengine::CCheckbox();
		checkbox1->SetBounds(daidalosengine::CRectangleI(0, 0, 32, 32));
		checkbox1->SetUncheckedTexture("checkbox_off.png");
		checkbox1->SetCheckedTexture("checkbox_on.png");
		checkbox1->SetClickedTexture("checkbox_clicked.png");
		checkbox1->GetLabelElement()->SetColor(daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F));
		checkbox1->GetLabelElement()->SetSize(26);
		checkbox1->GetLabelElement()->SetText("TEST");
		scroll->AddSubview(checkbox1);

		daidalosengine::CButton * button = new daidalosengine::CButton("button1.png");
		button->SetBounds(daidalosengine::CRectangleI(0, 100, 120, 60));
		button->SetDefaultTexture("button1.png");
		button->SetClickedTexture("button2.png");
		button->SetImageInset(daidalosengine::CInsetF(0.1F, 0.1F, 0.1F, 0.1F));
		button->SetDefaultTitleColor(daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F));
		button->GetTitleElement()->SetSize(26);
		button->GetTitleElement()->SetText("Test");
		scroll->AddSubview(button);

		daidalosengine::CCheckbox * checkbox2 = new daidalosengine::CCheckbox();
		checkbox2->SetBounds(daidalosengine::CRectangleI(0, 200, 32, 32));
		checkbox2->SetUncheckedTexture("checkbox_off.png");
		checkbox2->SetCheckedTexture("checkbox_on.png");
		checkbox2->SetClickedTexture("checkbox_clicked.png");
		scroll->AddSubview(checkbox2);


		daidalosengine::CSelectionList * selectionList = new daidalosengine::CSelectionList();
		selectionList->SetBounds(daidalosengine::CRectangleI(600, 100, 600, 200));
		selectionList->SetSelectionListTitle("SELECTION LIST: ");
		selectionList->AddSelection("VALUE 1");
		selectionList->AddSelection("VALUE 2");
		selectionList->AddSelection("VALUE 3");
		selectionList->AddSelection("VALUE 4");
		selectionList->AddSelection("VALUE 5");
		selectionList->AddSelection("VALUE 6");
		GetView()->AddToMainView(selectionList);

		ILevel::PostLoadLevel();
	}
}
