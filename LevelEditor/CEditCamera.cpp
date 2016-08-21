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

#define EDIT_CAMERA_LAYER_HEIGHT 8.0f
#define EDIT_CAMERA_STEP 10.0f

#include <limits>
#include <Core/CInputDevice.h>
#include <Core/CSoundContextManager.h>
#include <Core/CConfigurationManager.h>
#include "CEditCamera.h"

namespace EDITOR {
	CEditCamera::CEditCamera(DE::ICameraSceneNode * cameraNode) {
		this->m_state = 0;
		this->m_node = cameraNode;
		this->m_layer = 0;
		this->m_zoom = 8;

		// Override camera visibility
		this->m_node->SetFarDistance(200);

		this->m_inputDevice = DE::CInputDevice::GetInstance();
		this->m_soundContext = DE::CSoundContextManager::GetInstance();
		this->m_options = DE::CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);

		DE::SInputActionDeclaration decl[] = {
			{ EDIT_CAMERA_DONE, -1 },
			{ EDIT_CAMERA_UP, EDIT_CAMERA_DOWN },
			{ EDIT_CAMERA_DOWN, EDIT_CAMERA_UP },
			{ EDIT_CAMERA_LEFT, EDIT_CAMERA_RIGHT },
			{ EDIT_CAMERA_RIGHT, EDIT_CAMERA_LEFT },
			{ EDIT_CAMERA_ZOOM_IN , -1 },
			{ EDIT_CAMERA_ZOOM_OUT, -1 },
			{ EDIT_CAMERA_MESH_ROTATE, -1 },
			{ EDIT_CAMERA_MESH_NEXT, -1 },
			{ EDIT_CAMERA_TEXTURE_NEXT, -1 },
			{ EDIT_CAMERA_MESH_GROUND, -1 },
			{ EDIT_CAMERA_MESH_WALL, -1 },
			{ EDIT_CAMERA_MESH_CEILING, -1 }
		};

		this->m_inputDevice->AddActions(decl);

		// Keyboard mapping
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_DONE, DE::INPUT_KEY_KEYBOARD_SPACE);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_LEFT, DE::INPUT_KEY_KEYBOARD_Q);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_RIGHT, DE::INPUT_KEY_KEYBOARD_D);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_UP, DE::INPUT_KEY_KEYBOARD_Z);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_DOWN, DE::INPUT_KEY_KEYBOARD_S);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_ZOOM_IN, DE::INPUT_KEY_KEYBOARD_KP_PLUS);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_ZOOM_OUT, DE::INPUT_KEY_KEYBOARD_KP_MINUS);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_MESH_ROTATE, DE::INPUT_KEY_KEYBOARD_R);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_MESH_NEXT, DE::INPUT_KEY_KEYBOARD_A);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_TEXTURE_NEXT, DE::INPUT_KEY_KEYBOARD_E);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_MESH_GROUND, DE::INPUT_KEY_KEYBOARD_1);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_MESH_WALL, DE::INPUT_KEY_KEYBOARD_2);
		this->m_inputDevice->SetActionKey(EDIT_CAMERA_MESH_CEILING, DE::INPUT_KEY_KEYBOARD_3);
	}

	void CEditCamera::SetScene(DE::C3DScene * scene) {
		this->m_scene = scene;
	}

	void CEditCamera::SetPosition(const DE::CVector3F & position) {
		this->m_node->SetPosition(position);
	}

	void CEditCamera::SetRotation(const DE::CVector3F & rotation) {
		this->m_rotation = rotation;
	}

	void CEditCamera::Update(const float time) {
		float timeSecond = time / 1000.0f;

		this->m_node->SetPosition(DE::CVector3F(this->m_node->GetPosition().x, (float)(this->m_layer * 10 + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z));
		this->m_node->SetRotation(DE::CVector3F(80, 0, 0));

		UpdateState();
		UpdateMovement(timeSecond);
	}

	DE::ICameraSceneNode * CEditCamera::GetCameraNode() const {
		return this->m_node;
	}

	void CEditCamera::UpdateState() {
		this->m_state = 0;

		if(this->m_inputDevice->IsActionActive(EDIT_CAMERA_LEFT)) {
			this->m_state |= EDIT_CAMERA_LEFT;
			this->m_inputDevice->FreezeAction(EDIT_CAMERA_LEFT);
		}

		if(this->m_inputDevice->IsActionActive(EDIT_CAMERA_RIGHT)) {
			this->m_state |= EDIT_CAMERA_RIGHT;
			this->m_inputDevice->FreezeAction(EDIT_CAMERA_RIGHT);
		}

		if(this->m_inputDevice->IsActionActive(EDIT_CAMERA_UP)) {
			this->m_state |= EDIT_CAMERA_UP;
			this->m_inputDevice->FreezeAction(EDIT_CAMERA_UP);
		}
			
		if(this->m_inputDevice->IsActionActive(EDIT_CAMERA_DOWN)) {
			this->m_state |= EDIT_CAMERA_DOWN;
			this->m_inputDevice->FreezeAction(EDIT_CAMERA_DOWN);
		}

		if(this->m_inputDevice->IsActionActive(EDIT_CAMERA_ZOOM_IN)) {
			this->m_state |= EDIT_CAMERA_ZOOM_IN;
			this->m_inputDevice->FreezeAction(EDIT_CAMERA_ZOOM_IN);
		}

		if(this->m_inputDevice->IsActionActive(EDIT_CAMERA_ZOOM_OUT)) {
			this->m_state |= EDIT_CAMERA_ZOOM_OUT;
			this->m_inputDevice->FreezeAction(EDIT_CAMERA_ZOOM_OUT);
		}
	}

	void CEditCamera::UpdateMovement(const float timeSecond) {
		DE::CVector3F movement;

		if((this->m_state & EDIT_CAMERA_ZOOM_IN) != 0) {
			this->m_zoom--;
		}

		if((this->m_state & EDIT_CAMERA_ZOOM_OUT) != 0) {
			this->m_zoom++;
		}

		if((this->m_state & EDIT_CAMERA_LEFT) != 0) {
			this->m_node->SetPosition(DE::CVector3F(this->m_node->GetPosition().x, (float)(this->m_layer * 10 + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z) + DE::CVector3F(-EDIT_CAMERA_STEP, 0, 0));
		}

		if((this->m_state & EDIT_CAMERA_RIGHT) != 0) {
			this->m_node->SetPosition(DE::CVector3F(this->m_node->GetPosition().x, (float)(this->m_layer * 10 + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z) + DE::CVector3F(EDIT_CAMERA_STEP, 0, 0));
		}

		if((this->m_state & EDIT_CAMERA_UP) != 0) {
			this->m_node->SetPosition(DE::CVector3F(this->m_node->GetPosition().x, (float)(this->m_layer * 10 + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z) + DE::CVector3F(0, 0, EDIT_CAMERA_STEP));
		}

		if((this->m_state & EDIT_CAMERA_DOWN) != 0) {
			this->m_node->SetPosition(DE::CVector3F(this->m_node->GetPosition().x, (float)(this->m_layer * 10 + this->m_zoom * EDIT_CAMERA_LAYER_HEIGHT), this->m_node->GetPosition().z) + DE::CVector3F(0, 0, -EDIT_CAMERA_STEP));
		}
	}
}
