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

#define ROTATION_SPEED 45
#define TRIGGER_RADIUS 5

#include <Core/Utility/CVector3.h>
#include "CFlashlightEntity.h"

namespace SAMPLE3 {
	CFlashlightEntity::CFlashlightEntity(DE::ISceneNode * meshNode, DE::ISceneNode * lightNode, SAMPLES::CFPSCamera * camera) {
		this->m_node = meshNode;
		this->m_light = lightNode;
		this->m_player = camera;

		this->m_sound.LoadFromFile("health_bonus.wav", DE::SOUND_TYPE_STATIC);
		this->m_sound.SetRelativeToListener(true);
	}

	void CFlashlightEntity::Update(float const time) {
		DE::CVector3F rotation = this->m_node->GetRotation();
		rotation.y += (ROTATION_SPEED * time) / 1000.0f;
		if(rotation.y > 360) {
			rotation.y -= 360;
		}
		this->m_node->SetRotation(rotation);

		if(this->m_node->IsEnabled() && DE::CVector3F::Distance(this->m_node->GetAbsolutePosition(), this->m_player->GetCameraNode()->GetAbsolutePosition()) <= TRIGGER_RADIUS) {
			this->m_node->SetEnabled(false);
			this->m_light->SetEnabled(true);
			this->m_sound.SetStatus(DE::SOUND_STATUS_PLAY);
		}
	}
}
