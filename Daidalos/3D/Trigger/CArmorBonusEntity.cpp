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
#include "../../Enums.hpp"
#include "CArmorBonusEntity.hpp"

#define ROTATION_SPEED 70.0f
#define TRIGGER_RADIUS 5
#define ARMOR_BONUS 50

namespace DAIDALOS {
	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The armor bonus node
	*/
	CArmorBonusEntity::CArmorBonusEntity(GE::ILevel * parent, DE::CAnimatedMeshSceneNode * meshNode) : ITriggerEntity(parent) {

		SetId(meshNode->GetId());
		SetType(GE::ENTITY_TYPE_ARMOR);
		this->m_node = meshNode;
		this->m_node->SetMeshShadingType(DE::MESH_SHADING_SMOOTH);

		this->m_sound.LoadFromFile("armor_bonus.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_sound.SetRelativeToListener(true);
	}

	/**
	* Update the entity
	* @param time Elapsed time
	*/
	void CArmorBonusEntity::Update(const float time) {
		
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
	void CArmorBonusEntity::DoAction() {
		this->m_node->SetEnabled(false);
		this->m_sound.SetStatus(DE::SOUND_STATUS_PLAY);
		GetParent()->GetPlayer()->SetArmor(GetParent()->GetPlayer()->GetArmor() < 150 ? GetParent()->GetPlayer()->GetArmor() + ARMOR_BONUS : 200);
	}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	const bool CArmorBonusEntity::CanDoAction() {
		return this->m_node->IsEnabled() && DE::CVector3F::Distance(this->m_node->GetAbsolutePosition(), GetParent()->GetPlayer()->GetVisiblePosition()) <= TRIGGER_RADIUS && GetParent()->GetPlayer()->GetArmor() < 200;
	}
}
