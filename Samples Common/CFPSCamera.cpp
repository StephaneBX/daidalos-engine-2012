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

#define FPS_CAMERA_MOVEMENT_SPEED 10.0f
#define FPS_CAMERA_ROTATION_SPEED 90.0f
#define FPS_CAMERA_ROTATION_SPEED_GAMEPAD_FACTOR 0.25f
#define FPS_CAMERA_ROTATION_SPEED_GAMEPAD_VERTICAL_FACTOR 0.20f
#define FPS_CAMERA_GRAVITY -20.0f
#define FPS_CAMERA_JUMP_IMPULSION 10.0f
#define FPS_CAMERA_COLLISION_UPDATE 16
#define FPS_CAMERA_WALK_FACTOR 1.0f
#define FPS_CAMERA_RUN_FACTOR 1.7f
#define FPS_CAMERA_CROUCH_FACTOR 0.6f
#define FPS_CAMERA_SHAPE_HORIZONTAL_RADIUS 1.0f
#define FPS_CAMERA_SHAPE_VERTICAL_RADIUS_STAND 2.7f
#define FPS_CAMERA_SHAPE_VERTICAL_RADIUS_CROUCH 1.0f
#define FPS_CAMERA_SHAPE_VERTICAL_RADIUS_DEAD 0.5f
#define FPS_CAMERA_STEP_DISTANCE 7.0f
#define FPS_CAMERA_FALL_STEP_MIN_GRAVITY -14.0f
#define FPS_CAMERA_LADDER_DISTANCE 3.0f

#include <limits>
#include <Core/CInputDevice.h>
#include <Core/CSoundContextManager.h>
#include <Core/CConfigurationManager.h>
#include "Level/ILevel.h"
#include "CFPSCamera.h"
#include "Weapons/CHandgun.h"
#include "Weapons/CShotgun.h"

namespace SAMPLES {
	CFPSCamera::CFPSCamera(ILevel * parent, DE::ICameraSceneNode * cameraNode, DE::ISceneNode * flashlightNode) : IPlayer(parent) {
		this->m_actionState = 0;
		this->m_state = FPS_CAMERA_STATE_STAND;
		this->m_node = cameraNode;
		this->m_flashlightNode = flashlightNode;
		this->m_health = 80;
		this->m_armor = 0;
		this->m_stepDistance = 0;
		this->m_ladderDistance = 0;

		this->m_inputDevice = DE::CInputDevice::GetInstance();
		this->m_soundContext = DE::CSoundContextManager::GetInstance();
		this->m_options = DE::CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);

		DE::SInputActionDeclaration decl[] =
		{
			{ FPS_CAMERA_EXIT, -1 },
			{ FPS_CAMERA_STRAFELEFT, FPS_CAMERA_STRAFERIGHT },
			{ FPS_CAMERA_STRAFERIGHT, FPS_CAMERA_STRAFELEFT },
			{ FPS_CAMERA_FORWARD, FPS_CAMERA_BACKWARD },
			{ FPS_CAMERA_BACKWARD, FPS_CAMERA_FORWARD },
			{ FPS_CAMERA_JUMP, -1 },
			{ FPS_CAMERA_LOOKUP, FPS_CAMERA_LOOKDOWN },
			{ FPS_CAMERA_LOOKDOWN, FPS_CAMERA_LOOKUP },
			{ FPS_CAMERA_LOOKLEFT, FPS_CAMERA_LOOKRIGHT },
			{ FPS_CAMERA_LOOKRIGHT, FPS_CAMERA_LOOKLEFT },
			{ FPS_CAMERA_RUN, -1 },
			{ FPS_CAMERA_INTERACT, -1 },
			{ FPS_CAMERA_FIRE , -1 },
			{ FPS_CAMERA_RELOAD, -1 },
			{ FPS_CAMERA_NEXT_WEAPON, -1 },
			{ FPS_CAMERA_PREVIOUS_WEAPON, -1 },
			{ FPS_CAMERA_CROUCH, -1 }
		};

		this->m_inputDevice->AddActions(decl);

		// Keyboard mapping
		this->m_inputDevice->SetActionKey(FPS_CAMERA_EXIT, DE::INPUT_KEY_KEYBOARD_ESCAPE);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_STRAFELEFT, DE::INPUT_KEY_KEYBOARD_Q);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_STRAFERIGHT, DE::INPUT_KEY_KEYBOARD_D);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_FORWARD, DE::INPUT_KEY_KEYBOARD_Z);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_BACKWARD, DE::INPUT_KEY_KEYBOARD_S);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_JUMP, DE::INPUT_KEY_KEYBOARD_SPACE);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_LOOKUP, DE::INPUT_KEY_KEYBOARD_UP);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_LOOKDOWN, DE::INPUT_KEY_KEYBOARD_DOWN);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_LOOKLEFT, DE::INPUT_KEY_KEYBOARD_LEFT);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_LOOKRIGHT, DE::INPUT_KEY_KEYBOARD_RIGHT);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_RUN, DE::INPUT_KEY_KEYBOARD_LSHIFT);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_INTERACT, DE::INPUT_KEY_KEYBOARD_E);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_RELOAD, DE::INPUT_KEY_KEYBOARD_R);	
		this->m_inputDevice->SetActionKey(FPS_CAMERA_CROUCH, DE::INPUT_KEY_KEYBOARD_C);

		// Mouse mapping
		this->m_inputDevice->SetActionKey(FPS_CAMERA_FIRE, DE::INPUT_KEY_MOUSE_BUTTON0);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_NEXT_WEAPON, DE::INPUT_KEY_MOUSE_WHEELUP);
		this->m_inputDevice->SetActionKey(FPS_CAMERA_PREVIOUS_WEAPON, DE::INPUT_KEY_MOUSE_WHEELDOWN);

		// Gamepad mapping
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_EXIT, 0, DE::INPUT_KEY_GAMEPAD_BUTTON6);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_FORWARD, 0, DE::INPUT_KEY_GAMEPAD_AXIS_LYN);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_BACKWARD, 0, DE::INPUT_KEY_GAMEPAD_AXIS_LYP);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_STRAFELEFT, 0, DE::INPUT_KEY_GAMEPAD_AXIS_LXN);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_STRAFERIGHT, 0, DE::INPUT_KEY_GAMEPAD_AXIS_LXP);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_LOOKUP, 0, DE::INPUT_KEY_GAMEPAD_AXIS_RYN);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_LOOKDOWN, 0, DE::INPUT_KEY_GAMEPAD_AXIS_RYP);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_LOOKLEFT, 0, DE::INPUT_KEY_GAMEPAD_AXIS_RXN);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_LOOKRIGHT, 0, DE::INPUT_KEY_GAMEPAD_AXIS_RXP);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_JUMP, 0, DE::INPUT_KEY_GAMEPAD_BUTTON0);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_RUN, 0, DE::INPUT_KEY_GAMEPAD_BUTTON4);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_INTERACT, 0, DE::INPUT_KEY_GAMEPAD_BUTTON2);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_RELOAD, 0, DE::INPUT_KEY_GAMEPAD_BUTTON3);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_FIRE, 0, DE::INPUT_KEY_GAMEPAD_AXIS_LZN);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_NEXT_WEAPON, 0, DE::INPUT_KEY_GAMEPAD_POV_RIGHT);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_PREVIOUS_WEAPON, 0, DE::INPUT_KEY_GAMEPAD_POV_LEFT);
		this->m_inputDevice->SetGamepadActionKey(FPS_CAMERA_CROUCH, 0, DE::INPUT_KEY_GAMEPAD_BUTTON1);

		this->m_collisionShape = std::unique_ptr<DE::IShape3DF>(new DE::CEllipseF(this->m_node->GetPosition() - DE::CVector3F(0, 1, 0), DE::CVector2F(FPS_CAMERA_SHAPE_HORIZONTAL_RADIUS, FPS_CAMERA_SHAPE_VERTICAL_RADIUS_STAND)));
		this->m_rotation = this->m_node->GetRotation();
		this->m_visiblePosition = this->m_node->GetPosition();

		this->m_weaponChangeSound.LoadFromFile("change_weapon.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_weaponChangeSound.SetRelativeToListener(true);

		// Initialize the step sounds
		for(int i = 1; i <= 4; ++i) {
			this->m_stepSounds.push_back(std::unique_ptr<DE::CSound>(new DE::CSound()));
			this->m_stepSounds[i - 1]->LoadFromFile("footstep" + std::to_string(i) + ".wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
			this->m_stepSounds[i - 1]->SetRelativeToListener(true);
		}

		// Initialize the fall sound
		this->m_stepFallSound.LoadFromFile("footstep_fall.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_stepFallSound.SetRelativeToListener(true);

		// Initialize the ladder
		this->m_ladderSound.LoadFromFile("ladder.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_ladderSound.SetRelativeToListener(true);

		// Initialize the jump sound
		this->m_jumpSound.LoadFromFile("jump.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_jumpSound.SetRelativeToListener(true);

		// Initialize the hurt sound
		this->m_hurtSound.LoadFromFile("hurt.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_hurtSound.SetRelativeToListener(true);

		// Initialize the die sound
		this->m_dieSound.LoadFromFile("die.wav", DE::SOUND_TYPE_STATIC, DE::SOUND_CATEGORY_ENVIRONMENT);
		this->m_dieSound.SetRelativeToListener(true);

		// Initialize the weapons
		this->m_weapons.push_back(std::unique_ptr<IWeapon>(new CHandgun(this->m_parent, nullptr)));
		this->m_weapons.push_back(std::unique_ptr<IWeapon>(new CShotgun(this->m_parent, nullptr)));
		this->m_currentWeaponIndex = 0;
		this->m_currentWeapon = this->m_weapons[0].get();
	}

	/**
	* Damage the entity
	* @param damage The damage
	*/
	void CFPSCamera::TakeDamage(const unsigned int damage) {
		IPlayer::TakeDamage(damage);

		if(this->m_health > 0) {
			this->m_hurtSound.SetStatus(DE::SOUND_STATUS_PLAY);
		} else {
			DE::CEllipseF * collisionShapeEllipse = static_cast<DE::CEllipseF *>(this->m_collisionShape.get());
			collisionShapeEllipse->SetRadius(DE::CVector2F(FPS_CAMERA_SHAPE_HORIZONTAL_RADIUS, FPS_CAMERA_SHAPE_VERTICAL_RADIUS_DEAD));

			this->m_dieSound.SetStatus(DE::SOUND_STATUS_PLAY);
			this->m_node->SetEnabled(false);
		}
	}

	/**
	* Set the player armor
	* @param health The player armor
	*/
	void CFPSCamera::SetArmor(const int armor) {
		this->m_armor = armor;
	}

	/**
	* Set a player object
	* @param object The player object id
	* @param value The object value
	*/
	void CFPSCamera::SetObject(EPlayerObject object, const unsigned int value) {

		switch(object) {
		case PLAYER_OBJECT_FLASHLIGHT:
			if(this->m_flashlightNode != nullptr) {
				this->m_flashlightNode->SetEnabled(value >= 1 ? true : false);
			}
			break;

		case PLAYER_OBJECT_HANDGUN_AMMO:
			this->m_weapons[0]->AddAmmo(value);
			break;

		case PLAYER_OBJECT_SHOTGUN_AMMO:
			this->m_weapons[1]->AddAmmo(value);
			break;

		default:
			break;
		}
	}

	/**
	* Set the player position
	* @param position The position
	*/
	void CFPSCamera::SetPosition(const DE::CVector3F position) {
		this->m_visiblePosition = position;
	}

	void CFPSCamera::Update(const float time) {
		float timeSecond = time / 1000.0f;

		this->m_actionState = this->m_actionState & FPS_CAMERA_JUMP;
		if(IsEnabled()) {
			this->m_currentWeapon->Update(time);

			UpdateState();
			UpdateView(timeSecond);
			UpdateAction();
		}
		UpdateMovement(timeSecond, time);
		
		// Update the node
		this->m_node->SetPosition(this->m_visiblePosition + DE::CVector3F(0, 1, 0));
		this->m_node->SetRotation(this->m_rotation);

		// Update the listener
		this->m_soundContext->SetListenerPosition(this->m_visiblePosition);
		float orientation[] = { this->m_node->GetForwardVector().x, this->m_node->GetForwardVector().y, this->m_node->GetForwardVector().z, this->m_node->GetUpVector().x, this->m_node->GetUpVector().y, this->m_node->GetUpVector().z };
		this->m_soundContext->SetListenerOrientation(orientation);
	}

	/**
	* Check if the player can see a shape
	* @param shape A shape
	* @return True if the player can see the shape
	*/
	const bool CFPSCamera::IsShapeVisible(DE::IShape3DF * shape) {
		return !this->m_node->FrustumCull(shape);
	}

	/**
	* If the player interaction action is active
	* @return True if the player interaction action is active
	*/
	const bool CFPSCamera::IsInteracting() const {
		return this->m_inputDevice->IsActionActive(FPS_CAMERA_INTERACT) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_INTERACT);
	}

	/**
	* Check if the entity is enabled
	* @return True if the entity is enabled
	*/
	const bool CFPSCamera::IsEnabled() const {
		return this->m_health > 0;
	}

	/**
	* Get the player health
	* @return The player health
	*/
	const int CFPSCamera::GetArmor() const {
		return this->m_armor;
	}

	/**
	* Get the player magazine ammo for the current weapon
	* @return The current magazine ammo
	*/
	const unsigned int CFPSCamera::GetCurrentWeaponMagazineAmmo() const {
		return this->m_currentWeapon != nullptr ? this->m_currentWeapon->GetMagazineSize() : 0;
	}

	/**
	* Get the player stock ammo for the current weapon
	* @return The current stock ammo
	*/
	const unsigned int CFPSCamera::GetCurrentWeaponStockAmmo() const {
		return this->m_currentWeapon != nullptr ? this->m_currentWeapon->GetRemainingAmmo() : 0;
	}

	/**
	* Get the player's object value
	* @param object The object id
	* @return The object value
	*/
	const unsigned int CFPSCamera::GetObjectValue(EPlayerObject object) const {

		switch(object) {
		case PLAYER_OBJECT_FLASHLIGHT:
			return (this->m_flashlightNode != nullptr && this->m_flashlightNode->IsEnabled()) ? 1 : 0;

		default:
			return 0;
		}
	}

	/**
	* Callback used to handle event when the entity fall and touch the ground
	* @param gravity The gravity value before touching the ground
	*/
	void CFPSCamera::OnEntityTouchedGround(const DE::CVector3F gravity) {
		this->m_actionState &= ~FPS_CAMERA_JUMP;

		// Fall sound at a minimal hight
		if(gravity.y <= FPS_CAMERA_FALL_STEP_MIN_GRAVITY) {
			this->m_stepFallSound.SetStatus(DE::SOUND_STATUS_PLAY);
		}
	}

	void CFPSCamera::UpdateState() {

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_STRAFELEFT) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_STRAFELEFT)) {
			this->m_actionState |= FPS_CAMERA_STRAFELEFT;
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_STRAFERIGHT) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_STRAFERIGHT)) {
			this->m_actionState |= FPS_CAMERA_STRAFERIGHT;
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_FORWARD) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_FORWARD)) {
			this->m_actionState |= FPS_CAMERA_FORWARD;
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_BACKWARD) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_BACKWARD)) {
			this->m_actionState |= FPS_CAMERA_BACKWARD;
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_LOOKUP) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKUP)) {
			this->m_actionState |= FPS_CAMERA_LOOKUP;
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_LOOKDOWN) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKDOWN)) {
			this->m_actionState |= FPS_CAMERA_LOOKDOWN;
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_LOOKLEFT) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKLEFT)) {
			this->m_actionState |= FPS_CAMERA_LOOKLEFT;
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_LOOKRIGHT) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKRIGHT)) {
			this->m_actionState |= FPS_CAMERA_LOOKRIGHT;
		}

		if(this->m_gravityState && (this->m_state == FPS_CAMERA_STATE_STAND || this->m_state == FPS_CAMERA_STATE_RUN) && (this->m_inputDevice->IsActionActive(FPS_CAMERA_JUMP) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_JUMP)) && !(this->m_actionState & FPS_CAMERA_JUMP)) {
			this->m_actionState |= FPS_CAMERA_JUMP;
			this->m_gravity.y = FPS_CAMERA_JUMP_IMPULSION;
			this->m_inputDevice->FreezeAction(FPS_CAMERA_JUMP);
			this->m_inputDevice->FreezeGamepadAction(0, FPS_CAMERA_JUMP);
			this->m_jumpSound.SetStatus(DE::SOUND_STATUS_PLAY);
		}

		if(this->m_gravityState && (this->m_state == FPS_CAMERA_STATE_STAND || this->m_state == FPS_CAMERA_STATE_RUN) && ((this->m_inputDevice->IsActionActive(FPS_CAMERA_RUN) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_RUN)))) {
			this->m_actionState |= FPS_CAMERA_RUN;
			this->m_state = FPS_CAMERA_STATE_RUN;
		} else if(this->m_state == FPS_CAMERA_STATE_RUN) {
			this->m_state = FPS_CAMERA_STATE_STAND;
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_FIRE) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_FIRE)) {
			this->m_actionState |= FPS_CAMERA_FIRE;
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_RELOAD) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_RELOAD)) {
			this->m_actionState |= FPS_CAMERA_RELOAD;
			this->m_inputDevice->FreezeAction(FPS_CAMERA_RELOAD);
			this->m_inputDevice->FreezeGamepadAction(0, FPS_CAMERA_RELOAD);
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_NEXT_WEAPON) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_NEXT_WEAPON)) {
			this->m_actionState |= FPS_CAMERA_NEXT_WEAPON;
			this->m_inputDevice->FreezeAction(FPS_CAMERA_NEXT_WEAPON);
			this->m_inputDevice->FreezeGamepadAction(0, FPS_CAMERA_NEXT_WEAPON);
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_PREVIOUS_WEAPON) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_PREVIOUS_WEAPON)) {
			this->m_actionState |= FPS_CAMERA_PREVIOUS_WEAPON;
			this->m_inputDevice->FreezeAction(FPS_CAMERA_PREVIOUS_WEAPON);
			this->m_inputDevice->FreezeGamepadAction(0, FPS_CAMERA_PREVIOUS_WEAPON);
		}

		if(this->m_inputDevice->IsActionActive(FPS_CAMERA_CROUCH) || this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_CROUCH)) {
			this->m_actionState |= FPS_CAMERA_CROUCH;
			this->m_inputDevice->FreezeAction(FPS_CAMERA_CROUCH);
			this->m_inputDevice->FreezeGamepadAction(0, FPS_CAMERA_CROUCH);
		}
	}

	void CFPSCamera::UpdateMovement(const float timeSecond, const float time) {
		DE::CVector3F movement;
		DE::CEllipseF * collisionShapeEllipse = static_cast<DE::CEllipseF *>(this->m_collisionShape.get());

		if((this->m_actionState & FPS_CAMERA_CROUCH) != 0 && this->m_state == FPS_CAMERA_STATE_STAND && this->m_gravity.y == 0) {
			this->m_state = FPS_CAMERA_STATE_CROUCH;
			collisionShapeEllipse->SetRadius(DE::CVector2F(FPS_CAMERA_SHAPE_HORIZONTAL_RADIUS, FPS_CAMERA_SHAPE_VERTICAL_RADIUS_CROUCH));

		} else if((this->m_actionState & FPS_CAMERA_CROUCH) != 0 && this->m_state == FPS_CAMERA_STATE_CROUCH && this->m_gravity.y == 0) {

			// Check if the player can stand up
			DE::CVector3F movementUp = DE::CVector3F(0, (FPS_CAMERA_SHAPE_VERTICAL_RADIUS_STAND - FPS_CAMERA_SHAPE_VERTICAL_RADIUS_CROUCH) * 2, 0);
			DE::CVector3F collisionPoint;

			// If no up collision detected at the standing position
			if(!this->m_parent->GetScene()->GetCollisionPoint(*collisionShapeEllipse, movementUp, collisionPoint)) {
				this->m_state = FPS_CAMERA_STATE_STAND;
				collisionShapeEllipse->SetRadius(DE::CVector2F(FPS_CAMERA_SHAPE_HORIZONTAL_RADIUS, FPS_CAMERA_SHAPE_VERTICAL_RADIUS_STAND));
				collisionShapeEllipse->SetPosition(collisionShapeEllipse->GetPosition() + movementUp);
			}

		} else {
			// Strafe left
			if((this->m_actionState & FPS_CAMERA_STRAFELEFT) != 0) {
				float factor = 1;

				if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_STRAFELEFT)) {
					factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_STRAFELEFT);
				}

				movement.x += -sin((this->m_rotation.y + 90.0f) * PI_OVER_180) * factor;
				movement.z += -cos((this->m_rotation.y + 90.0f) * PI_OVER_180) * factor;
			}

			// Strafe right
			if((this->m_actionState & FPS_CAMERA_STRAFERIGHT) != 0) {
				float factor = 1;

				if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_STRAFERIGHT)) {
					factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_STRAFERIGHT);
				}

				movement.x += -sin((this->m_rotation.y - 90.0f) * PI_OVER_180) * factor;
				movement.z += -cos((this->m_rotation.y - 90.0f) * PI_OVER_180) * factor;
			}

			// Go forward
			if((this->m_actionState & FPS_CAMERA_FORWARD) != 0) {
				float factor = 1;

				if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_FORWARD)) {
					factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_FORWARD);
				}

				movement.x += sin(this->m_rotation.y * PI_OVER_180) * factor;
				movement.z += cos(this->m_rotation.y * PI_OVER_180) * factor;

				// If no gravity, add the vertical part
				if(!this->m_gravityState) {
					this->m_gravity.y = -sin(this->m_rotation.x * PI_OVER_180) * factor;
				}
			}

			// Go backward
			if((this->m_actionState & FPS_CAMERA_BACKWARD) != 0) {
				float factor = 1;

				if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_BACKWARD)) {
					factor = this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_BACKWARD);
				}

				movement.x += -sin(this->m_rotation.y * PI_OVER_180) * factor;
				movement.z += -cos(this->m_rotation.y * PI_OVER_180) * factor;

				// If no gravity, add the vertical part
				if(!this->m_gravityState) {
					this->m_gravity.y = sin(this->m_rotation.x * PI_OVER_180) * factor;
				}
			}
		}

		if(this->m_gravityState) {
			this->m_gravity.y += FPS_CAMERA_GRAVITY * timeSecond;
		} else {
			this->m_gravity.y *= FPS_CAMERA_MOVEMENT_SPEED;
		}
		
		// Visibility position update based on the state
		float speedFactor = 1.0f;
		switch(this->m_state) {
		case FPS_CAMERA_STATE_STAND:
			speedFactor = FPS_CAMERA_WALK_FACTOR;
			break;

		case FPS_CAMERA_STATE_RUN:
			speedFactor = FPS_CAMERA_RUN_FACTOR;
			break;

		default:
			speedFactor = FPS_CAMERA_CROUCH_FACTOR;
			break;
		}

		// Collision test
		DE::CVector3F previousVisiblePosition = this->m_visiblePosition;
		PerformLevelCollisions(movement * FPS_CAMERA_MOVEMENT_SPEED * speedFactor, time);

		// Step sounds
		if(this->m_gravityState) {
			if(this->m_gravity.y == 0) {
				this->m_stepDistance += (this->m_visiblePosition - previousVisiblePosition).Length();
				if(this->m_stepDistance >= FPS_CAMERA_STEP_DISTANCE) {
					// Play a random step sound
					this->m_stepSounds[DE::RandomNumber<int>(0, this->m_stepSounds.size() - 1)]->SetStatus(DE::SOUND_STATUS_PLAY);
					this->m_stepDistance = 0;
				}
			} else {
				// Reset the step and ladder distance counter
				this->m_stepDistance = 0;
				this->m_ladderDistance = 0;
			}
		} else {
			this->m_ladderDistance += fabsf(this->m_visiblePosition.y - previousVisiblePosition.y);
			if(this->m_ladderDistance >= FPS_CAMERA_LADDER_DISTANCE) {
				// Play the ladder sound
				this->m_ladderSound.SetStatus(DE::SOUND_STATUS_PLAY);
				this->m_ladderDistance = 0;
			}
		}
	}

	void CFPSCamera::UpdateView(const float timeSecond) {
		// Mouse view
		if(this->m_inputDevice->GetMouseMovement().x != 0 || this->m_inputDevice->GetMouseMovement().y != 0) {
			// Update the movement
			this->m_previousMouseMovement.x = (float)this->m_inputDevice->GetMouseMovement().x;
			this->m_previousMouseMovement.y = (float)this->m_inputDevice->GetMouseMovement().y;

			this->m_rotation.y += this->m_previousMouseMovement.x * FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5f);
			this->m_rotation.x += this->m_previousMouseMovement.y * FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5f);
		} else {
			// No movement detected this frame, use the smooth factor instead
			this->m_previousMouseMovement *= this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SMOOTHING, 0.2f);

			this->m_rotation.y += this->m_previousMouseMovement.x * FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5f);
			this->m_rotation.x += this->m_previousMouseMovement.y * FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_MOUSE, APPLICATION_OPTION_MOUSE_SENSIBILITY, 0.5f);
		}

		// Keyboard view and gamepad

		// Camera up action
		if((this->m_actionState & FPS_CAMERA_LOOKUP) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKUP)) {
				factor = FPS_CAMERA_ROTATION_SPEED_GAMEPAD_VERTICAL_FACTOR / (1.3f - (this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKUP) * this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKUP)));
				this->m_previousGamepadViewMovement.y = -factor;
			}
			this->m_rotation.x -= FPS_CAMERA_ROTATION_SPEED * timeSecond * factor;

		} else if(this->m_previousGamepadViewMovement.y < 0) {
			// Gamepad smooth factor
			this->m_previousGamepadViewMovement.y = this->m_previousGamepadViewMovement.y * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_GAMEPAD, APPLICATION_OPTION_GAMEPAD_SMOOTH_FACTOR, 0.0f);
			this->m_rotation.x += FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_previousGamepadViewMovement.y;
		}

		// Camera down action
		if((this->m_actionState & FPS_CAMERA_LOOKDOWN) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKDOWN)) {
				factor = FPS_CAMERA_ROTATION_SPEED_GAMEPAD_VERTICAL_FACTOR / (1.3f - (this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKDOWN) * this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKDOWN)));
				this->m_previousGamepadViewMovement.y = factor;
			}
			this->m_rotation.x += FPS_CAMERA_ROTATION_SPEED * timeSecond * factor;

		} else if(this->m_previousGamepadViewMovement.y > 0) {
			// Gamepad smooth factor
			this->m_previousGamepadViewMovement.y = this->m_previousGamepadViewMovement.y * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_GAMEPAD, APPLICATION_OPTION_GAMEPAD_SMOOTH_FACTOR, 0.0f);
			this->m_rotation.x += FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_previousGamepadViewMovement.y;
		}

		// Camera left action
		if((this->m_actionState & FPS_CAMERA_LOOKLEFT) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKLEFT)) {
				factor = FPS_CAMERA_ROTATION_SPEED_GAMEPAD_FACTOR / (1.3f - (this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKLEFT) * this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKLEFT)));
				this->m_previousGamepadViewMovement.x = -factor;
			}
			this->m_rotation.y -= FPS_CAMERA_ROTATION_SPEED * timeSecond * factor;

		} else if(this->m_previousGamepadViewMovement.x < 0) {
			// Gamepad smooth factor
			this->m_previousGamepadViewMovement.x = this->m_previousGamepadViewMovement.x * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_GAMEPAD, APPLICATION_OPTION_GAMEPAD_SMOOTH_FACTOR, 0.0f);
			this->m_rotation.y += FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_previousGamepadViewMovement.x;
		}

		// Camera right action
		if((this->m_actionState & FPS_CAMERA_LOOKRIGHT) != 0) {
			float factor = 1;

			if(this->m_inputDevice->IsGamepadActionActive(0, FPS_CAMERA_LOOKRIGHT)) {
				factor = FPS_CAMERA_ROTATION_SPEED_GAMEPAD_FACTOR / (1.3f - (this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKRIGHT) * this->m_inputDevice->GetGamepadActionFactor(0, FPS_CAMERA_LOOKRIGHT)));
				this->m_previousGamepadViewMovement.x = factor;
			}
			this->m_rotation.y += FPS_CAMERA_ROTATION_SPEED * timeSecond * factor;

		} else if(this->m_previousGamepadViewMovement.x > 0) {
			// Gamepad smooth factor
			this->m_previousGamepadViewMovement.x = this->m_previousGamepadViewMovement.x * this->m_options.GetValueFloat(APPLICATION_OPTION_SECTION_GAMEPAD, APPLICATION_OPTION_GAMEPAD_SMOOTH_FACTOR, 0.0f);
			this->m_rotation.y += FPS_CAMERA_ROTATION_SPEED * timeSecond * this->m_previousGamepadViewMovement.x;
		}


		if(this->m_rotation.x >= 80.0f)
			this->m_rotation.x = 80;

		if(this->m_rotation.x < -80)
			this->m_rotation.x = -80;

		if(this->m_rotation.y >= 360.0f)
			this->m_rotation.y -= 360.0f;

		if(this->m_rotation.y < 0)
			this->m_rotation.y += 360.0f;
	}

	/**
	* Update the action
	*/
	void CFPSCamera::UpdateAction() {
		if((this->m_actionState & FPS_CAMERA_FIRE) != 0) {
			this->m_currentWeapon->Fire();
		}

		if((this->m_actionState & FPS_CAMERA_RELOAD) != 0) {
			this->m_currentWeapon->Reload();
		}

		if((this->m_actionState & FPS_CAMERA_NEXT_WEAPON) != 0) {
			this->m_currentWeaponIndex = (this->m_currentWeaponIndex + 1) % this->m_weapons.size();
			this->m_currentWeapon = this->m_weapons[this->m_currentWeaponIndex].get();
			this->m_weaponChangeSound.SetStatus(DE::SOUND_STATUS_PLAY);
		}

		if((this->m_actionState & FPS_CAMERA_PREVIOUS_WEAPON) != 0) {
			this->m_currentWeaponIndex = (this->m_currentWeaponIndex - 1) % this->m_weapons.size();
			this->m_currentWeapon = this->m_weapons[this->m_currentWeaponIndex].get();
			this->m_weaponChangeSound.SetStatus(DE::SOUND_STATUS_PLAY);
		}
	}
}
