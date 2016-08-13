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

#define TRIGGER_RADIUS 8
#define OPENED_TIME 6000
#define ANIMATION_SPEED 1.0f

#include <sstream>
#include "CDoorEntity.h"

namespace SAMPLES {

	CDoorEntity::CDoorEntity(DE::CAnimatedMeshSceneNode * meshNode, CFPSCamera * camera, DE::C3DScene * scene3D) {
		this->m_node = meshNode;
		this->m_emitterNode = nullptr;
		this->m_scene3D = scene3D;
		this->m_player = camera;

		this->m_state = DOOR_STATE_CLOSED;
		this->m_closable = true;
		this->m_animationSpeed = ANIMATION_SPEED;
		this->m_triggerRadius = TRIGGER_RADIUS;
		this->m_timer = 0;

		this->m_startSound.LoadFromFile("door_start.wav", DE::SOUND_TYPE_STATIC);
		this->m_startSound.SetRelativeToListener(false);
		this->m_startSound.SetPosition(this->m_node->GetAbsolutePosition());

		this->m_stopSound.LoadFromFile("door_stop.wav", DE::SOUND_TYPE_STATIC);
		this->m_stopSound.SetRelativeToListener(false);
		this->m_stopSound.SetPosition(this->m_node->GetAbsolutePosition());

		// Read the attributes
		if(meshNode->GetAttribute("closable") == "false") {
			this->m_closable = false;
		}
		if(meshNode->GetAttribute("animationSpeed") != "") {
			std::istringstream ss(meshNode->GetAttribute("animationSpeed"));
			ss >> this->m_animationSpeed;
		}
		if(meshNode->GetAttribute("openingParticleEmitter") != "") {
			this->m_emitterNode = static_cast<DE::CParticleEmitterSceneNode*>(this->m_scene3D->GetNode(meshNode->GetAttribute("openingParticleEmitter")));
		}
		if(meshNode->GetAttribute("triggerRadius") != "") {
			std::istringstream ss(meshNode->GetAttribute("triggerRadius"));
			ss >> this->m_triggerRadius;
		}
	}

	void CDoorEntity::Update(float const time) {

		if(this->m_state == DOOR_STATE_OPENING && this->m_node->GetAnimationState() == DE::MESH_ANIMATION_STATE_STOPPED) {
			this->m_state = DOOR_STATE_OPENED;
			this->m_timer = 0;
			this->m_scene3D->SetBrushCollision(this->m_node->GetId(), false);
			this->m_stopSound.SetPosition(this->m_node->GetAbsolutePosition());
			this->m_stopSound.SetStatus(DE::SOUND_STATUS_PLAY);
			if(this->m_emitterNode != nullptr) {
				this->m_emitterNode->SetActive(false);
			}
		}

		if(this->m_state == DOOR_STATE_CLOSING && this->m_node->GetAnimationState() == DE::MESH_ANIMATION_STATE_STOPPED) {
			this->m_state = DOOR_STATE_CLOSED;
			this->m_stopSound.SetPosition(this->m_node->GetAbsolutePosition());
			this->m_stopSound.SetStatus(DE::SOUND_STATUS_PLAY);
		}

		// Trigger action
		if(this->m_node->IsEnabled() && DE::CVector3F::Distance(this->m_node->GetAbsolutePosition(), this->m_player->GetCameraNode()->GetAbsolutePosition()) <= this->m_triggerRadius) {

			switch(this->m_state) {
			case DOOR_STATE_CLOSED:
				this->m_state = DOOR_STATE_OPENING;
				this->m_node->StartAnimation(1, 2, this->m_animationSpeed);
				this->m_startSound.SetPosition(this->m_node->GetAbsolutePosition());
				this->m_startSound.SetStatus(DE::SOUND_STATUS_PLAY);
				if(this->m_emitterNode != nullptr) {
					this->m_emitterNode->SetActive(true);
				}
				break;

			case DOOR_STATE_OPENED:
				this->m_timer = 0;
				break;

			default:
				break;
			}
			
		} else {
			if(this->m_state == DOOR_STATE_OPENED && this->m_closable) {
				this->m_timer += time;

				// The door stay opened for a limited time
				if(this->m_timer >= OPENED_TIME) {
					this->m_timer = 0;
					this->m_state = DOOR_STATE_CLOSING;
					this->m_node->StartAnimation(2, 3, this->m_animationSpeed);
					this->m_scene3D->SetBrushCollision(this->m_node->GetId(), true);
					this->m_startSound.SetPosition(this->m_node->GetAbsolutePosition());
					this->m_startSound.SetStatus(DE::SOUND_STATUS_PLAY);
				}
			}
		}
	}
}
