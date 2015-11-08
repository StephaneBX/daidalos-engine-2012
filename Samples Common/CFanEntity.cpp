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

#define ANIMATION_SPEED 15.0f
#define ANIMATION_FACTOR 3.0f

#include "CFanEntity.h"

namespace SAMPLES {
	CFanEntity::CFanEntity(DE::CAnimatedMeshSceneNode * meshNode, CFPSCamera * camera) {
		this->m_node = meshNode;
		this->m_player = camera;

		this->m_speed = 0;
		this->m_state = FAN_STATE_STOPPED;

		this->m_node->SetLooped(true);
		this->m_node->SetStartFrame(1);
		this->m_node->SetEndFrame(72);
		this->m_node->SetCurrentFrame(1);
	}

	void CFanEntity::Update(float const time) {
		switch(this->m_state) {
		case FAN_STATE_STARTING:
			this->m_speed += (ANIMATION_FACTOR * time);
			this->m_node->SetAnimationSpeed(this->m_speed);
			if(this->m_speed > ANIMATION_SPEED) {
				this->m_speed = ANIMATION_SPEED;
				this->m_state = FAN_STATE_STARTED;
			}
			break;

		case FAN_STATE_STOPPING:
			this->m_speed -= ANIMATION_FACTOR * time;
			this->m_node->SetAnimationSpeed(this->m_speed);
			if(this->m_speed < 0) {
				this->m_speed = 0;
				this->m_state = FAN_STATE_STOPPED;
				this->m_node->StopAnimation();
			}
			break;

		default:
			break;
		}
	}

	void CFanEntity::DoAction() {
		switch(this->m_state) {
		case FAN_STATE_STARTED:
			Stop();
			break;

		case FAN_STATE_STOPPED:
			Start();
			break;

		default:
			break;
		}
	}

	const bool CFanEntity::CanDoAction() const {
		return this->m_state == FAN_STATE_STARTED || this->m_state == FAN_STATE_STOPPED;
	}

	void CFanEntity::Start() {
		this->m_speed = 0;
		this->m_state = FAN_STATE_STARTING;
		this->m_node->ResumeAnimation();
	}

	void CFanEntity::Stop() {
		this->m_speed = ANIMATION_SPEED;
		this->m_state = FAN_STATE_STOPPING;
	}
}