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

#ifndef __CFPSCAMERA_H
#define __CFPSCAMERA_H

#include <Scene/3D/ICameraSceneNode.h>
#include <Scene/3D/C3DScene.h>
#include <Scene/3D/CEllipse.h>
#include <Core/Utility/CVector3.h>
#include <Core/CInputDevice.h>
#include <Core/CSoundContextManager.h>
#include <SiniP/SiniP.h>
#include "IPlayer.h"
#include "Weapons/IWeapon.h"

namespace SAMPLES {
	enum EFPSCameraAction {
		FPS_CAMERA_EXIT				= 1 << 1,
		FPS_CAMERA_STRAFELEFT		= 1 << 2,
		FPS_CAMERA_STRAFERIGHT		= 1 << 3,
		FPS_CAMERA_FORWARD			= 1 << 4,
		FPS_CAMERA_BACKWARD			= 1 << 5,
		FPS_CAMERA_JUMP				= 1 << 6,
		FPS_CAMERA_LOOKUP			= 1 << 7,
		FPS_CAMERA_LOOKDOWN			= 1 << 8,
		FPS_CAMERA_LOOKLEFT			= 1 << 9,
		FPS_CAMERA_LOOKRIGHT		= 1 << 10,
		FPS_CAMERA_RUN				= 1 << 11,
		FPS_CAMERA_INTERACT			= 1 << 12,
		FPS_CAMERA_FIRE				= 1 << 13,
		FPS_CAMERA_RELOAD			= 1 << 14,
		FPS_CAMERA_NEXT_WEAPON		= 1 << 15,
		FPS_CAMERA_PREVIOUS_WEAPON	= 1 << 16,
		FPS_CAMERA_CROUCH			= 1 << 17
	};

	enum EFPSCameraState {
		FPS_CAMERA_STATE_STAND,
		FPS_CAMERA_STATE_RUN,
		FPS_CAMERA_STATE_CROUCH,
		FPS_CAMERA_STATE_STANDING,
		FPS_CAMERA_STATE_CROUCHING
	};

	/**
	* FPS type camera implementation
	*/
	class CFPSCamera : public IPlayer
	{
	private:
		unsigned int m_actionState;
		int m_armor;
		EFPSCameraState m_state;
		DE::CVector2F m_previousMouseMovement;
		DE::CVector3F m_rotation;
		DE::ICameraSceneNode * m_node;
		DE::ISceneNode * m_flashlightNode;
		DE::CInputDevice * m_inputDevice;
		DE::CSoundContextManager * m_soundContext;
		SiniP m_options;

		IWeapon * m_currentWeapon;
		int m_currentWeaponIndex;
		std::vector<std::unique_ptr<IWeapon> > m_weapons;
		DE::CSound m_weaponChangeSound;

	public:
		/**
		* Constructor
		* @param parent The parent
		* @param cameraNode The camera node
		* @param flashlightNode The flashlight node
		*/
		CFPSCamera(ILevel * parent, DE::ICameraSceneNode * cameraNode, DE::ISceneNode * flashlightNode);

		/**
		* Set the player armor
		* @param health The player armor
		*/
		void SetArmor(const int armor);

		/**
		* Set a player object
		* @param object The player object id
		* @param value The object value
		*/
		void SetObject(EPlayerObject object, const unsigned int value);

		/**
		* Set the player position
		* @param position The position
		*/
		void SetPosition(const DE::CVector3F position);

		/**
		* Update the camera
		* @param time Elapsed time
		*/
		void Update(float const time = 0);

		/**
		* Check if the player can see a shape
		* @param shape A shape
		* @return True if the player can see the shape
		*/
		const bool IsShapeVisible(DE::IShape3DF * shape);

		/**
		* If the player interaction action is active
		* @return True if the player interaction action is active
		*/
		const bool IsInteracting() const;

		/**
		* Get the player health
		* @return The player health
		*/
		const int GetArmor() const;

		/**
		* Get the player magazine ammo for the current weapon
		* @return The current magazine ammo
		*/
		const unsigned int GetCurrentWeaponMagazineAmmo() const;

		/**
		* Get the player stock ammo for the current weapon
		* @return The current stock ammo
		*/
		const unsigned int GetCurrentWeaponStockAmmo() const;

		/**
		* Get the player's object value
		* @param object The object id
		* @return The object value
		*/
		const unsigned int GetObjectValue(EPlayerObject object) const;

	protected:
		/**
		* Callback used to handle event when the entity fall and touch the ground
		* @param gravity The gravity value before touching the ground
		*/
		void OnEntityTouchedGround(const DE::CVector3F gravity);

	private:
		/**
		* Update the state
		*/
		void UpdateState();

		/**
		* Update the movement
		* @param timeSecond Elapsed time
		*/
		void UpdateMovement(const float timeSecond, const float time);

		/**
		* Update the view
		* @param timeSecond Elapsed time
		*/
		void UpdateView(const float timeSecond);

		/**
		* Update the action
		*/
		void UpdateAction();
	};
}

#endif
