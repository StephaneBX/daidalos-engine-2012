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

#include <Level/ILevel.hpp>
#include <Enums.hpp>
#include "CMagneticCardEntity.hpp"

#define ROTATION_SPEED 70.0f
#define TRIGGER_RADIUS 5

namespace DAIDALOS {
	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The magnetic card node
	*/
	CMagneticCardEntity::CMagneticCardEntity(GE::ILevel * parent, DE::CAnimatedMeshSceneNode * meshNode) : ITriggerEntity(parent) {

		this->m_id = meshNode->GetId();
		this->m_type = GE::ENTITY_TYPE_MAGNETIC_CARD;
		this->m_node = meshNode;
		this->m_node->SetMeshShadingType(DE::MESH_SHADING_SMOOTH);

		this->m_sound.LoadFromFile("misc_bonus.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_sound.SetRelativeToListener(true);

		// Read the attributes
		if(this->m_node->GetAttribute("color") != "") {
			const std::string color = this->m_node->GetAttribute("color");
			if(color == "red") {
				this->m_playerObject = GE::PLAYER_OBJECT_MAGNETIC_CARD_RED;
			}
		}
	}

	/**
	* Update the entity
	* @param time Elapsed time
	*/
	void CMagneticCardEntity::Update(const float time) {

		ITriggerEntity::Update(time);
		if(this->m_node->IsEnabled()) {
			const float timePerSecond = time / 1000.0f;

			float rotation = this->m_node->GetRotation().y + ROTATION_SPEED * timePerSecond;
			this->m_node->SetRotation(DE::CVector3F(0, rotation, 0));
		}
	}

	/**
	* Trigger the entity action
	*/
	void CMagneticCardEntity::DoAction() {
		this->m_node->SetEnabled(false);
		this->m_sound.SetStatus(DE::SOUND_STATUS_PLAY);
		this->m_parent->GetPlayer()->SetObject(this->m_playerObject, 1);
	}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	const bool CMagneticCardEntity::CanDoAction() {
		return this->m_node->IsEnabled() && DE::CVector3F::Distance(this->m_node->GetAbsolutePosition(), this->m_parent->GetPlayer()->GetVisiblePosition()) <= TRIGGER_RADIUS;
	}
}
