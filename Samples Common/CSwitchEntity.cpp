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

#define TRIGGER_RADIUS 4
#define ANIMATION_SPEED 10.0f

#include "CSwitchEntity.h"

namespace SAMPLES {
	CSwitchEntity::CSwitchEntity(DE::CAnimatedMeshSceneNode * meshNode, CFPSCamera * camera, DE::C3DScene * scene3D) {
		this->m_inputDevice = DE::CInputDevice::GetInstance();

		this->m_node = meshNode;
		this->m_player = camera;
		this->m_player = camera;

		this->m_highlighted = false;
		this->m_state = SWITCH_STATE_OFF;
	}

	void CSwitchEntity::Update(float const time) {

		if(CanDoAction()) {

			if(!this->m_highlighted) {
				this->m_node->SetActiveMeshMaterialElement("highlighted");
				this->m_highlighted = true;
			}

			if(this->m_inputDevice->IsActionActive(FPS_CAMERA_INTERACT)) {
				DoAction();
			}
		} else if(this->m_highlighted) {
			this->m_node->RestoreDefaultMeshMaterialElement();
			this->m_highlighted = false;
		}

		switch(this->m_state) {
		case SWITCH_STATE_ACTIVATING:
			if(this->m_node->GetAnimationState() == DE::MESH_ANIMATION_STATE_STOPPED) {
				for(auto it = this->m_targetsEntity.begin(); it != this->m_targetsEntity.end(); it++) {
					(*it)->DoAction();
				}
				this->m_state = SWITCH_STATE_ON;
			}
			break;

		case SWITCH_STATE_DEACTIVATING:
			if(this->m_node->GetAnimationState() == DE::MESH_ANIMATION_STATE_STOPPED) {
				for(auto it = this->m_targetsEntity.begin(); it != this->m_targetsEntity.end(); it++) {
					(*it)->DoAction();
				}
				this->m_state = SWITCH_STATE_OFF;
			}
			break;

		default:
			break;
		}
	}

	void CSwitchEntity::DoAction() {
		switch(this->m_state) {
		case SWITCH_STATE_OFF:
			SetOn();
			break;

		case SWITCH_STATE_ON:
			SetOff();
			break;

		default:
			break;
		}
	}

	const bool CSwitchEntity::CanDoAction() const {

		bool targetsOk = true;
		for(auto it = this->m_targetsEntity.begin(); it != this->m_targetsEntity.end() && targetsOk; it++) {
			targetsOk = (*it)->CanDoAction();
		}

		return targetsOk && (this->m_state == SWITCH_STATE_OFF || this->m_state == SWITCH_STATE_ON) && DE::CVector3F::Distance(this->m_node->GetAbsolutePosition(), this->m_player->GetCameraNode()->GetAbsolutePosition()) <= TRIGGER_RADIUS && !this->m_player->GetCameraNode()->FrustumCull(this->m_node->GetVisibilityShape());
	}

	void CSwitchEntity::SetOn() {
		this->m_state = SWITCH_STATE_ACTIVATING;
		this->m_node->StartAnimation(1, 10, ANIMATION_SPEED);
		this->m_node->RestoreDefaultMeshMaterialElement();
		this->m_highlighted = false;
	}

	void CSwitchEntity::SetOff() {
		this->m_state = SWITCH_STATE_DEACTIVATING;
		this->m_node->StartAnimation(10, 20, ANIMATION_SPEED);
		this->m_node->RestoreDefaultMeshMaterialElement();
		this->m_highlighted = false;
	}
}
