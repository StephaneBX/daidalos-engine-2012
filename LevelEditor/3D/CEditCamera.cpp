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

#include <limits>
#include <Core/CInputDevice.hpp>
#include <Core/CSoundContextManager.hpp>
#include <Core/CConfigurationManager.hpp>
#include "../CEditorConfiguration.hpp"
#include "../3D/CEditCamera.hpp"

namespace daidaloseditor {
	CEditCamera::CEditCamera(daidalosengine::ICameraSceneNode * cameraNode) : m_node(cameraNode) {
		this->m_state = 0;
		this->m_layer = 0;
		this->m_zoom = 8;
		this->m_scene = nullptr;

		// Override camera visibility
		this->m_node->SetFarDistance(200);

		this->m_inputDevice = daidalosengine::CInputDevice::GetInstance();
		this->m_soundContext = daidalosengine::CSoundContextManager::GetInstance();
		this->m_options = daidalosengine::CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);
	}

	void CEditCamera::SetScene(daidalosengine::C3DScene * scene) {
		this->m_scene = scene;
	}

	void CEditCamera::SetPosition(const daidalosengine::CVector3F & position) {
		this->m_node->SetPosition(position);
	}

	void CEditCamera::SetRotation(const daidalosengine::CVector3F & rotation) {
		this->m_rotation = rotation;
	}

	void CEditCamera::Update(const float time) {
		const auto timeSecond = time / 1000.0F;

		UpdateState();
		UpdateMovement(timeSecond);

		this->m_node->SetPosition(daidalosengine::CVector3F(this->m_node->GetPosition().x, static_cast<float>(this->m_layer * EDIT_CAMERA_LAYER_HEIGHT + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z));
		this->m_node->SetRotation(daidalosengine::CVector3F(80, 0, 0));
	}

	daidalosengine::ICameraSceneNode * CEditCamera::GetCameraNode() const {
		return this->m_node;
	}

	int CEditCamera::GetLayer() const {
		return this->m_layer;
	}

	void CEditCamera::UpdateState() {
		this->m_state = 0;

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_LEFT)) {
			this->m_state |= EDITOR_ACTION_CAMERA_LEFT;
			this->m_inputDevice->FreezeAction(EDITOR_ACTION_CAMERA_LEFT);
		}

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_RIGHT)) {
			this->m_state |= EDITOR_ACTION_CAMERA_RIGHT;
			this->m_inputDevice->FreezeAction(EDITOR_ACTION_CAMERA_RIGHT);
		}

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_UP)) {
			this->m_state |= EDITOR_ACTION_CAMERA_UP;
			this->m_inputDevice->FreezeAction(EDITOR_ACTION_CAMERA_UP);
		}
			
		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_DOWN)) {
			this->m_state |= EDITOR_ACTION_CAMERA_DOWN;
			this->m_inputDevice->FreezeAction(EDITOR_ACTION_CAMERA_DOWN);
		}

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_ZOOM_IN)) {
			this->m_state |= EDITOR_ACTION_CAMERA_ZOOM_IN;
			this->m_inputDevice->FreezeAction(EDITOR_ACTION_CAMERA_ZOOM_IN);
		}

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_ZOOM_OUT)) {
			this->m_state |= EDITOR_ACTION_CAMERA_ZOOM_OUT;
			this->m_inputDevice->FreezeAction(EDITOR_ACTION_CAMERA_ZOOM_OUT);
		}

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_LAYER_UP)) {
			this->m_state |= EDITOR_ACTION_CAMERA_LAYER_UP;
		}

		if(this->m_inputDevice->IsActionActive(EDITOR_ACTION_CAMERA_LAYER_DOWN)) {
			this->m_state |= EDITOR_ACTION_CAMERA_LAYER_DOWN;
		}
	}

	void CEditCamera::UpdateMovement(const float timeSecond) {

		if((this->m_state & EDITOR_ACTION_CAMERA_ZOOM_IN) != 0) {
			this->m_zoom--;
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_ZOOM_OUT) != 0) {
			this->m_zoom++;
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_LAYER_UP) != 0) {
			this->m_layer++;
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_LAYER_DOWN) != 0) {
			this->m_layer--;
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_LEFT) != 0) {
			this->m_node->SetPosition(daidalosengine::CVector3F(this->m_node->GetPosition().x, static_cast<float>(this->m_layer * 10 + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z) + daidalosengine::CVector3F(-EDIT_CAMERA_STEP, 0, 0));
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_RIGHT) != 0) {
			this->m_node->SetPosition(daidalosengine::CVector3F(this->m_node->GetPosition().x, static_cast<float>(this->m_layer * 10 + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z) + daidalosengine::CVector3F(EDIT_CAMERA_STEP, 0, 0));
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_UP) != 0) {
			this->m_node->SetPosition(daidalosengine::CVector3F(this->m_node->GetPosition().x, static_cast<float>(this->m_layer * 10 + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z) + daidalosengine::CVector3F(0, 0, EDIT_CAMERA_STEP));
		}

		if((this->m_state & EDITOR_ACTION_CAMERA_DOWN) != 0) {
			this->m_node->SetPosition(daidalosengine::CVector3F(this->m_node->GetPosition().x, static_cast<float>(this->m_layer * 10 + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z) + daidalosengine::CVector3F(0, 0, -EDIT_CAMERA_STEP));
		}
	}
}
