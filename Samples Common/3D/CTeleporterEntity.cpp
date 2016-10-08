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

#include "CTeleporterEntity.h"

namespace SAMPLES {

#define TRIGGER_RADIUS 3.0f

	/**
	* @param node The switch node
	* @param camera The player
	*/
	CTeleporterEntity::CTeleporterEntity(DE::CAnimatedMeshSceneNode * meshNode, IPlayer * player, DE::C3DScene * scene3D) {

		this->m_type = ENTITY_TYPE_TELEPORTER;
		this->m_node = meshNode;
		this->m_player = player;
		this->m_levelName = this->m_node->GetAttribute("nextLevelName");

		// Create the particle emitters
		DE::CParticleEmitter * emitter1 = scene3D->GetParticleSystem().InstanciateEmitter();
		emitter1->SetPosition(this->m_node->GetAbsolutePosition());
		emitter1->SetTextureId(1);
		emitter1->SetBlendMode(1);
		emitter1->SetLife(-1);
		emitter1->SetLifeMax(-1);
		emitter1->SetEmissionRate(80);
		
		if(this->m_levelName != "") {
			emitter1->AddInitializer(new DE::CColorParticleInitializer(DE::CColor(0.0f, 0.7f, 0.0f, 0.2f), DE::CColor(0.0f, 0.7f, 0.0f, 0.2f)));
		} else {
			emitter1->AddInitializer(new DE::CColorParticleInitializer(DE::CColor(0.7f, 0.0f, 0.0f, 0.2f), DE::CColor(0.7f, 0.0f, 0.0f, 0.2f)));
		}
		emitter1->AddInitializer(new DE::CSizeParticleInitializer(0.4f, 0.4f));
		emitter1->AddInitializer(new DE::CLifeParticleInitializer(2000, 2000));
		emitter1->AddInitializer(new DE::CRadiusPositionParticleInitializer(1));
		emitter1->AddInitializer(new DE::CLinearVelocityParticleInitializer(DE::CVector3F(0,1,0), 0, 2, 4));

		emitter1->AddUpdater(new DE::CColorParticleUpdater());
		emitter1->AddUpdater(new DE::CLifeParticleUpdater());
		emitter1->AddUpdater(new DE::CVelocityParticleUpdater(0));

		DE::CParticleEmitter * emitter2 = scene3D->GetParticleSystem().InstanciateEmitter();
		emitter2->SetPosition(this->m_node->GetAbsolutePosition() + DE::CVector3F(0, 8, 0));
		emitter2->SetTextureId(1);
		emitter2->SetBlendMode(1);
		emitter2->SetLife(-1);
		emitter2->SetLifeMax(-1);
		emitter2->SetEmissionRate(80);

		if(this->m_levelName != "") {
			emitter2->AddInitializer(new DE::CColorParticleInitializer(DE::CColor(0.0f, 0.7f, 0.0f, 0.2f), DE::CColor(0.0f, 0.7f, 0.0f, 0.2f)));
		} else {
			emitter2->AddInitializer(new DE::CColorParticleInitializer(DE::CColor(0.7f, 0.0f, 0.0f, 0.2f), DE::CColor(0.7f, 0.0f, 0.0f, 0.2f)));
		}
		emitter2->AddInitializer(new DE::CSizeParticleInitializer(0.4f, 0.4f));
		emitter2->AddInitializer(new DE::CLifeParticleInitializer(2000, 2000));
		emitter2->AddInitializer(new DE::CRadiusPositionParticleInitializer(1));
		emitter2->AddInitializer(new DE::CLinearVelocityParticleInitializer(DE::CVector3F(0, -1, 0), 0, 2, 4));

		emitter2->AddUpdater(new DE::CColorParticleUpdater());
		emitter2->AddUpdater(new DE::CLifeParticleUpdater());
		emitter2->AddUpdater(new DE::CVelocityParticleUpdater(0));
	}

	/**
	* Update the teleporter
	* @param time Elapsed time
	*/
	void CTeleporterEntity::Update(float const time) {

	}

	/**
	* Trigger the entity action
	*/
	void CTeleporterEntity::DoAction() {

	}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	const bool CTeleporterEntity::CanDoAction() const {
		return this->m_levelName != "" && DE::CVector3F::Distance(this->m_node->GetAbsolutePosition() + DE::CVector3F(0,4,0), this->m_player->GetPosition()) <= TRIGGER_RADIUS;
	}

	/**
	* Get the next level name
	* @return The next level name
	*/
	const std::string & CTeleporterEntity::GetNextLevelName() const {
		return this->m_levelName;
	}
}