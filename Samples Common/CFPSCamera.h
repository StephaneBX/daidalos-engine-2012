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

namespace SAMPLES 
{
	enum EFPSCameraMovement
	{
		FPS_CAMERA_EXIT			= 1 << 1,
		FPS_CAMERA_STRAFELEFT	= 1 << 2,
		FPS_CAMERA_STRAFERIGHT	= 1 << 3,
		FPS_CAMERA_FORWARD		= 1 << 4,
		FPS_CAMERA_BACKWARD		= 1 << 5,
		FPS_CAMERA_JUMP			= 1 << 6,
		FPS_CAMERA_LOOKUP		= 1 << 7,
		FPS_CAMERA_LOOKDOWN		= 1 << 8,
		FPS_CAMERA_LOOKLEFT		= 1 << 9,
		FPS_CAMERA_LOOKRIGHT	= 1 << 10,
		FPS_CAMERA_RUN			= 1 << 11,
		FPS_CAMERA_INTERACT		= 1 << 12,
		FPS_CAMERA_FIRE			= 1 << 13,
		FPS_CAMERA_RELOAD		= 1 << 14
	};

	/**
	* FPS type camera implementation
	*/
	class CFPSCamera : public IPlayer
	{
	private:
		unsigned int m_state;
		int m_health;
		int m_armor;
		float m_lastCollisionUpdateTimer;
		DE::CVector2F m_previousMouseMovement;
		DE::CVector3F m_rotation;
		DE::CVector3F m_gravity;
		DE::CVector3F m_visiblePosition;
		DE::ICameraSceneNode * m_node;
		DE::CInputDevice * m_inputDevice;
		DE::CEllipseF m_shape;
		DE::C3DScene * m_scene;
		DE::CSoundContextManager * m_soundContext;
		SiniP m_options;
		std::unique_ptr<IWeapon> m_currentWeapon;

	public:
		/**
		* @param cameraNode The camera node
		*/
		CFPSCamera(DE::ICameraSceneNode * cameraNode);

		/**
		* Set the associated scene
		* @param scene The associated scene
		*/
		void SetScene(DE::C3DScene * scene);

		/**
		* Set the player health
		* @param health The player health
		*/
		void SetHealth(const int health);

		/**
		* Set the player armor
		* @param health The player armor
		*/
		void SetArmor(const int armor);

		/**
		* Set a player object
		* @param id The player object id
		* @param value The object value
		*/
		void SetObject(const unsigned int id, const int value);

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
		* Get the player shape
		* @return The player shape
		*/
		DE::IShape3DF * GetShape();

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
		const int GetHealth() const;

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
		* @param id The object id
		* @return The object value
		*/
		const int GetObjectValue(const unsigned int id) const;

		/**
		* Get the player gravity factor
		*/
		const float GetGravity() const;

		/**
		* Get the player position
		* @return The player position
		*/
		const DE::CVector3F & GetPosition() const;

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
