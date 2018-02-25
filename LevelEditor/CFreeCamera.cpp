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

#define FPS_CAMERA_MOVEMENT_SPEED 10.0F
#define FPS_CAMERA_ROTATION_SPEED 60.0F
#define FPS_CAMERA_GRAVITY -20.0F
#define FPS_CAMERA_JUMP_IMPULSION 10.0F

#include <limits>
#include <Core/CInputDevice.hpp>
#include <Core/CSoundContextManager.hpp>
#include <Core/CConfigurationManager.hpp>
#include "CFreeCamera.hpp"

namespace daidaloseditor {
	/**
	* Constructor
	* @param parent The parent
	* @param cameraNode The camera node
	*/
	CFreeCamera::CFreeCamera(daidalosgameengine::ILevel * parent, daidalosengine::ICameraSceneNode * cameraNode) : m_node(cameraNode), IPlayer(parent) {
		this->m_state = 0;

		this->m_inputDevice = daidalosengine::CInputDevice::GetInstance();
		this->m_soundContext = daidalosengine::CSoundContextManager::GetInstance();
		this->m_options = daidalosengine::CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);

		daidalosengine::SInputActionDeclaration decl[] =
		{
			{ FPS_CAMERA_EXIT, -1 },
			{ FPS_CAMERA_STRAFELEFT, FPS_CAMERA_STRAFERIGHT },
			{ FPS_CAMERA_STRAFERIGHT, FPS_CAMERA_STRAFELEFT },
			{ FPS_CAMERA_FORWARD, FPS_CAMERA_BACKWARD },
			{ FPS_CAMERA_BACKWARD, FPS_CAMERA_FORWARD },
			{ FPS_CAMERA_LOOKUP, FPS_CAMERA_LOOKDOWN },
			{ FPS_CAMERA_LOOKDOWN, FPS_CAMERA_LOOKUP },
			{ FPS_CAMERA_LOOKLEFT, FPS_CAMERA_LOOKRIGHT },
			{ FPS_CAMERA_LOOKRIGHT, FPS_CAMERA_LOOKLEFT },
		};

		this->m_inputDevice->AddActions(decl);

		// Keyboard mapping
		this->m_inputDevice->SetActionKey(FPS_CAMERA_EXIT, daidalosengine::INPUT_KEY_KEYBOARD_ESCAPE);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_STRAFELEFT, daidalosengine::INPUT_KEY_KEYBOARD_Q);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_STRAFERIGHT, daidalosengine::INPUT_KEY_KEYBOARD_D);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_FORWARD, daidalosengine::INPUT_KEY_KEYBOARD_Z);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_BACKWARD, daidalosengine::INPUT_KEY_KEYBOARD_S);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_LOOKUP, daidalosengine::INPUT_KEY_KEYBOARD_UP);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_LOOKDOWN, daidalosengine::INPUT_KEY_KEYBOARD_DOWN);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_LOOKLEFT, daidalosengine::INPUT_KEY_KEYBOARD_LEFT);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_LOOKRIGHT, daidalosengine::INPUT_KEY_KEYBOARD_RIGHT);

		// Gamepad mapping
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_FORWARD, 0, daidalosengine::INPUT_KEY_GAMEPAD_AXIS_LYN);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_BACKWARD, 0, daidalosengine::INPUT_KEY_GAMEPAD_AXIS_LYP);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_STRAFELEFT, 0, daidalosengine::INPUT_KEY_GAMEPAD_AXIS_LXN);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_STRAFERIGHT, 0, daidalosengine::INPUT_KEY_GAMEPAD_AXIS_LXP);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_LOOKUP, 0, daidalosengine::INPUT_KEY_GAMEPAD_AXIS_RXN);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_LOOKDOWN, 0, daidalosengine::INPUT_KEY_GAMEPAD_AXIS_RXP);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_LOOKLEFT, 0, daidalosengine::INPUT_KEY_GAMEPAD_AXIS_RYN);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_LOOKRIGHT, 0, daidalosengine::INPUT_KEY_GAMEPAD_AXIS_RYP);

		this->m_shape.SetPosition(this->m_node->GetPosition() - daidalosengine::CVector3F(0, 1, 0));
		this->m_shape.SetRadius(daidalosengine::CVector2F(1, 2.7F));
		this->m_rotation = this->m_node->GetRotation();
	}

	/**
	* Set the player active state
	* @param enabled The player active state
	*/
	void CFreeCamera::SetEnabled(const bool enabled) {
		this->m_node->SetEnabled(enabled);
	}

	/**
	* Set the player armor
	* @param health The player armor
	*/
	void CFreeCamera::SetArmor(const int armor) {
		// Nothing to do
	}

	/**
	* Set a player object
	* @param object The player object id
	* @param value The object value
	*/
	void CFreeCamera::SetObject(unsigned int object, const unsigned int value) {
		// Nothing to do
	}

	/**
	* Set the player position
	* @param position The position
	*/
	void CFreeCamera::SetPosition(const daidalosengine::CVector3F position) {
		SetVisiblePosition(position);
	}

	void CFreeCamera::Update(const float time) {
		float timeSecond = time / 1000.0F;

		UpdateState();
		UpdateView(timeSecond);
		UpdateMovement(timeSecond);

		this->m_node->SetPosition(this->m_shape.GetPosition() + daidalosengine::CVector3F(0, 1, 0));
		this->m_node->SetRotation(this->m_rotation);

		// Update the listener
		this->m_soundContext->SetListenerPosition(this->m_shape.GetPosition());
		float orientation[] = { this->m_node->GetForwardVector().x, this->m_node->GetForwardVector().y, this->m_node->GetForwardVector().z, this->m_node->GetUpVector().x, this->m_node->GetUpVector().y, this->m_node->GetUpVector().z };
		this->m_soundContext->SetListenerOrientation(orientation);
	}

	daidalosengine::ICameraSceneNode * CFreeCamera::GetCameraNode() const {
		return this->m_node;
	}

	/**
	* Check if the player can see a shape
	* @param shape A shape
	* @return True if the player can see the shape
	*/
	const bool CFreeCamera::IsShapeVisible(daidalosengine::IShape3DF * shape) {
		return !this->m_node->FrustumCull(shape);
	}

	/**
	* If the player interaction action is active
	* @return True if the player interaction action is active
	*/
	const bool CFreeCamera::IsInteracting() const {
		return false;
	}

	/**
	* If the player has moved during the current frame
	* return True if the player has moved during the current frame
	*/
	const bool CFreeCamera::HasMoved() {
		return true;
	}

	/**
	* Get the player health
	* @return The player health
	*/
	const int CFreeCamera::GetArmor() const {
		return 0;
	}

	/**
	* Get the player magazine ammo for the current weapon
	* @return The current magazine ammo
	*/
	const unsigned int CFreeCamera::GetCurrentWeaponMagazineAmmo() const {
		return 0;
	}

	/**
	* Get the player stock ammo for the current weapon
	* @return The current stock ammo
	*/
	const unsigned int CFreeCamera::GetCurrentWeaponStockAmmo() const {
		return 0;
	}

	/**
	* Get the player's object value
	* @param object The object id
	* @return The object value
	*/
	const unsigned int CFreeCamera::GetObjectValue(unsigned int object) const {
		return 0;
	}

	/**
	* Callback used to handle event when the entity fall and touch the ground
	* @param gravity The gravity value before touching the ground
	*/
	void CFreeCamera::OnEntityTouchedGround(const daidalosengine::CVector3F gravity) {
		// Nothing to do
	}

	void CFreeCamera::UpdateState() {
		this->m_state = 0;

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_STRAFELEFT) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_STRAFELEFT))
			this->m_state |= FPS_CAMERA_STRAFELEFT;

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_STRAFERIGHT) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_STRAFERIGHT))
			this->m_state |= FPS_CAMERA_STRAFERIGHT;

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_FORWARD) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_FORWARD))
			this->m_state |= FPS_CAMERA_FORWARD;

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_BACKWARD) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_BACKWARD))
			this->m_state |= FPS_CAMERA_BACKWARD;

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_LOOKUP) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKUP))
			this->m_state |= FPS_CAMERA_LOOKUP;

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_LOOKDOWN) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKDOWN))
			this->m_state |= FPS_CAMERA_LOOKDOWN;

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_LOOKLEFT) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKLEFT))
			this->m_state |= FPS_CAMERA_LOOKLEFT;

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_LOOKRIGHT) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKRIGHT))
			this->m_state |= FPS_CAMERA_LOOKRIGHT;
	}

	void CFreeCamera::UpdateMovement(const float timeSecond) {
		daidalosengine::CVector3F movement;

		if((this->m_state & FPS_CAMERA_STRAFELEFT) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_STRAFELEFT)) {
				factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_STRAFELEFT);
			}

			movement.x += -sin((this->m_rotation.y + 90.0F) * PI_OVER_180) * factor;
			movement.z += -cos((this->m_rotation.y + 90.0F) * PI_OVER_180) * factor;
		}

		if((this->m_state & FPS_CAMERA_STRAFERIGHT) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_STRAFERIGHT)) {
				factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_STRAFERIGHT);
			}

			movement.x += -sin((this->m_rotation.y - 90.0F) * PI_OVER_180) * factor;
			movement.z += -cos((this->m_rotation.y - 90.0F) * PI_OVER_180) * factor;
		}

		if((this->m_state & FPS_CAMERA_FORWARD) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_FORWARD)) {
				factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_FORWARD);
			}

			movement.x += sin(this->m_rotation.y * PI_OVER_180) * factor;
			movement.z += cos(this->m_rotation.y * PI_OVER_180) * factor;
			movement.y -= sin(this->m_rotation.x * PI_OVER_180) * factor;
		}

		if((this->m_state & FPS_CAMERA_BACKWARD) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_BACKWARD)) {
				factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_BACKWARD);
			}

			movement.x += -sin(this->m_rotation.y * PI_OVER_180) * factor;
			movement.z += -cos(this->m_rotation.y * PI_OVER_180) * factor;
			movement.y += sin(this->m_rotation.x * PI_OVER_180) * factor;
		}

		movement = movement.Normalize();
		daidalosengine::CVector3F finalMovement = movement * FPS_CAMERA_MOVEMENT_SPEED * timeSecond;
		daidalosengine::CVector3F finalPosition = this->m_shape.GetPosition() + finalMovement;
		this->m_shape.SetPosition(finalPosition);
	}

	void CFreeCamera::UpdateView(const float timeSecond) {
		// Mouse view
		if(this->m_inputDevice->GetMouseMovement().x != 0 || this->m_inputDevice->GetMouseMovement().y != 0) {
			// Update the movement
			this->m_previousMouseMovement.x = (float)this->m_inputDevice->GetMouseMovement().x;
			this->m_previousMouseMovement.y = (float)this->m_inputDevice->GetMouseMovement().y;

			this->m_rotation.y += this->m_previousMouseMovement.x * FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5F);
			this->m_rotation.x += this->m_previousMouseMovement.y * FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5F);
		} else {
			// No movement detected this frame, use the smooth factor instead
			this->m_previousMouseMovement *= this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SMOOTHING, 0.2F);

			this->m_rotation.y += this->m_previousMouseMovement.x * FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5F);
			this->m_rotation.x += this->m_previousMouseMovement.y * FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5F);
		}

		// Keyboard view
		if((this->m_state & FPS_CAMERA_LOOKUP) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKUP)) {
				factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKUP);
			}

			this->m_rotation.x -= FPS_CAMERA_ROTATION_SPEED * timeSecond * factor;
		}

		if((this->m_state & FPS_CAMERA_LOOKDOWN) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKDOWN)) {
				factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKDOWN);
			}

			this->m_rotation.x += FPS_CAMERA_ROTATION_SPEED * timeSecond * factor;
		}

		if((this->m_state & FPS_CAMERA_LOOKLEFT) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKLEFT)) {
				factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKLEFT);
			}

			this->m_rotation.y -= FPS_CAMERA_ROTATION_SPEED * timeSecond * factor;
		}

		if((this->m_state & FPS_CAMERA_LOOKRIGHT) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKRIGHT)) {
				factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKRIGHT);
			}

			this->m_rotation.y += FPS_CAMERA_ROTATION_SPEED * timeSecond * factor;
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
