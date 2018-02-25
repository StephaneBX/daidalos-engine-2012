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

#define VIEW_CAMERA_MOVEMENT_SPEED 30.0F
#define VIEW_CAMERA_ROTATION_SPEED 60.0F

#include <limits>
#include <Core/CInputDevice.hpp>
#include <Core/CSoundContextManager.hpp>
#include <Core/CConfigurationManager.hpp>
#include "../CEditorConfiguration.hpp"
#include "../3D/CViewCamera.hpp"

namespace daidaloseditor {
	CViewCamera::CViewCamera(daidalosengine::ICameraSceneNode * cameraNode) {
		this->m_state = 0;
		this->m_node = cameraNode;

		// Override camera visibility
		this->m_node->SetFarDistance(200);

		this->m_inputDevice = daidalosengine::CInputDevice::GetInstance();
		this->m_soundContext = daidalosengine::CSoundContextManager::GetInstance();
		this->m_options = daidalosengine::CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);

		this->m_shape.SetPosition(this->m_node->GetPosition());
		this->m_shape.SetRadius(daidalosengine::CVector2F(1, 2.7F));
		this->m_rotation = this->m_node->GetRotation();
	}

	void CViewCamera::SetScene(daidalosengine::C3DScene * scene) {
		this->m_scene = scene;
	}

	void CViewCamera::SetPosition(const daidalosengine::CVector3F & position) {
		this->m_shape.SetPosition(position);
	}

	void CViewCamera::SetRotation(const daidalosengine::CVector3F & rotation) {
		this->m_rotation = rotation;
	}

	void CViewCamera::Update(const float time) {
		float timeSecond = time / 1000.0F;

		UpdateState();
		UpdateView(timeSecond);
		UpdateMovement(timeSecond);

		this->m_node->SetPosition(this->m_shape.GetPosition());
		this->m_node->SetRotation(this->m_rotation);

		// Update the listener
		this->m_soundContext->SetListenerPosition(this->m_shape.GetPosition());
		float orientation[] = { this->m_node->GetForwardVector().x, this->m_node->GetForwardVector().y, this->m_node->GetForwardVector().z, this->m_node->GetUpVector().x, this->m_node->GetUpVector().y, this->m_node->GetUpVector().z };
		this->m_soundContext->SetListenerOrientation(orientation);
	}

	daidalosengine::ICameraSceneNode * CViewCamera::GetCameraNode() const {
		return this->m_node;
	}

	void CViewCamera::UpdateState() {
		this->m_state = 0;

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_LEFT))
			this->m_state |= EDITOR_ACTION_CAMERA_LEFT;

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_RIGHT))
			this->m_state |= EDITOR_ACTION_CAMERA_RIGHT;

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_UP))
			this->m_state |= EDITOR_ACTION_CAMERA_UP;

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_DOWN))
			this->m_state |= EDITOR_ACTION_CAMERA_DOWN;

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_ZOOM_OUT))
			this->m_state |= EDITOR_ACTION_CAMERA_ZOOM_OUT;

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_ZOOM_IN))
			this->m_state |= EDITOR_ACTION_CAMERA_ZOOM_IN;
	}

	void CViewCamera::UpdateMovement(const float timeSecond) {
		daidalosengine::CVector3F movement;

		if((this->m_state & EDITOR_ACTION_CAMERA_LEFT) != 0) {
			movement.x += -sin((this->m_rotation.y + 90.0F) * PI_OVER_180);
			movement.z += -cos((this->m_rotation.y + 90.0F) * PI_OVER_180);
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_RIGHT) != 0) {
			movement.x += -sin((this->m_rotation.y - 90.0F) * PI_OVER_180);
			movement.z += -cos((this->m_rotation.y - 90.0F) * PI_OVER_180);
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_UP) != 0) {
			movement.x += sin(this->m_rotation.y * PI_OVER_180);
			movement.z += cos(this->m_rotation.y * PI_OVER_180);
			movement.y -= sin(this->m_rotation.x * PI_OVER_180);
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_DOWN) != 0) {
			movement.x += -sin(this->m_rotation.y * PI_OVER_180);
			movement.z += -cos(this->m_rotation.y * PI_OVER_180);
			movement.y += sin(this->m_rotation.x * PI_OVER_180);
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_ZOOM_OUT) != 0) {
			movement.y -= VIEW_CAMERA_MOVEMENT_SPEED * timeSecond;
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_ZOOM_IN) != 0) {
			movement.y += VIEW_CAMERA_MOVEMENT_SPEED * timeSecond;
		}

		movement = movement.Normalize();
		daidalosengine::CVector3F finalMovement = movement * VIEW_CAMERA_MOVEMENT_SPEED * timeSecond;
		daidalosengine::CVector3F finalPosition = this->m_shape.GetPosition() + finalMovement;
		this->m_shape.SetPosition(finalPosition);
	}

	void CViewCamera::UpdateView(const float timeSecond) {
		// Mouse view
		if(this->m_inputDevice->GetMouseMovement().x != 0 || this->m_inputDevice->GetMouseMovement().y != 0) {
			// Update the movement
			this->m_previousMouseMovement.x = (float)this->m_inputDevice->GetMouseMovement().x;
			this->m_previousMouseMovement.y = (float)this->m_inputDevice->GetMouseMovement().y;

			this->m_rotation.y += this->m_previousMouseMovement.x * VIEW_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5F);
			this->m_rotation.x += this->m_previousMouseMovement.y * VIEW_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5F);
		} else {
			// No movement detected this frame, use the smooth factor instead
			this->m_previousMouseMovement *= this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SMOOTHING, 0.2F);

			this->m_rotation.y += this->m_previousMouseMovement.x * VIEW_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5F);
			this->m_rotation.x += this->m_previousMouseMovement.y * VIEW_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5F);
		}

		if(this->m_rotation.x >= 80.0F)
			this->m_rotation.x = 80;

		if(this->m_rotation.x < -80)
			this->m_rotation.x = -80;

		if(this->m_rotation.y >= 360.0F)
			this->m_rotation.y -= 360.0F;

		if(this->m_rotation.y < 0)
			this->m_rotation.y += 360.0F;
	}
}
