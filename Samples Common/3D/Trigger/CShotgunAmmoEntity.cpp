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

#include "../../Enums.h"
#include "../../Level/ILevel.h"
#include "CShotgunAmmoEntity.h"

#define ROTATION_SPEED 70.0f
#define TRIGGER_RADIUS 5
#define AMMO_QUANTITY 7

namespace SAMPLES {
	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The shotgun ammo node
	*/
	CShotgunAmmoEntity::CShotgunAmmoEntity(ILevel * parent, DE::CAnimatedMeshSceneNode * meshNode) : ITriggerEntity(parent) {

		this->m_id = meshNode->GetId();
		this->m_type = ENTITY_TYPE_SHOTGUN_AMMO;
		this->m_node = meshNode;
		this->m_node->SetMeshShadingType(DE::MESH_SHADING_SMOOTH);

		this->m_sound.LoadFromFile("shotgun_ammo.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_sound.SetRelativeToListener(true);
	}

	/**
	* Update the entity
	* @param time Elapsed time
	*/
	void CShotgunAmmoEntity::Update(const float time) {

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
	void CShotgunAmmoEntity::DoAction() {
		this->m_node->SetEnabled(false);
		this->m_sound.SetStatus(DE::SOUND_STATUS_PLAY);
		this->m_parent->GetPlayer()->SetObject(PLAYER_OBJECT_SHOTGUN_AMMO, AMMO_QUANTITY);
	}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	const bool CShotgunAmmoEntity::CanDoAction() {
		return this->m_node->IsEnabled() && this->m_parent->GetPlayer()->GetObjectValue(PLAYER_OBJECT_SHOTGUN_WEAPON) > 0 && DE::CVector3F::Distance(this->m_node->GetAbsolutePosition(), this->m_parent->GetPlayer()->GetVisiblePosition()) <= TRIGGER_RADIUS;
	}
}
