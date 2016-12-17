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

#ifndef __IPLAYER_H
#define __IPLAYER_H

#include <Core/Utility/CVector3.h>
#include <Scene/3D/IShape3D.h>
#include "3D/ICollidableEntity.h"

namespace SAMPLES {

	enum EPlayerObject {
		PLAYER_OBJECT_FLASHLIGHT,
		PLAYER_OBJECT_HANDGUN_WEAPON,
		PLAYER_OBJECT_SHOTGUN_WEAPON,
		PLAYER_OBJECT_HANDGUN_AMMO,
		PLAYER_OBJECT_SHOTGUN_AMMO,
	};

	/**
	* Player interface
	*/
	class IPlayer : public ICollidableEntity {
	public:
		/**
		* Set the player health
		* @param health The player health
		*/
		virtual void SetHealth(const int health) = 0;

		/**
		* Set the player armor
		* @param health The player armor
		*/
		virtual void SetArmor(const int armor) = 0;

		/**
		* Set a player object
		* @param id The player object id
		* @param value The object value
		*/
		virtual void SetObject(EPlayerObject object, const unsigned int value) = 0;

		/**
		* Set the player position
		* @param position The position
		*/
		virtual void SetPosition(const DE::CVector3F position) = 0;

		/**
		* Update the camera
		* @param time Elapsed time
		*/
		virtual void Update(float const time = 0) = 0;

		/**
		* Check if the player can see a shape
		* @param shape A shape
		* @return True if the player can see the shape
		*/
		virtual const bool IsShapeVisible(DE::IShape3DF * shape) = 0;

		/**
		* If the player interaction action is active
		* @return True if the player interaction action is active
		*/
		virtual const bool IsInteracting() const = 0;

		/**
		* Get the player health
		* @return The player health
		*/
		virtual const int GetHealth() const = 0;

		/**
		* Get the player health
		* @return The player health
		*/
		virtual const int GetArmor() const = 0;

		/**
		* Get the player magazine ammo for the current weapon
		* @return The current magazine ammo
		*/
		virtual const unsigned int GetCurrentWeaponMagazineAmmo() const = 0;

		/**
		* Get the player stock ammo for the current weapon
		* @return The current stock ammo
		*/
		virtual const unsigned int GetCurrentWeaponStockAmmo() const = 0;

		/**
		* Get the player's object value
		* @param id The object id
		* @return The object value
		*/
		virtual const unsigned int GetObjectValue(EPlayerObject object) const = 0;

	protected:
		/**
		* Callback used to handle event when the entity fall and touch the ground
		* @param gravity The gravity value before touching the ground
		*/
		virtual void OnEntityTouchedGround(const DE::CVector3F gravity) = 0;
	};
}

#endif
