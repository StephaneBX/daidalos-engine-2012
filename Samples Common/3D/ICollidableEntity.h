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

#ifndef __ICOLLIDABLEENTITY_H
#define __ICOLLIDABLEENTITY_H

#include <set>
#include <memory>
#include <Core/Utility/CVector3.h>
#include <Scene/3D/IShape3D.h>
#include "IEntity.h"

namespace SAMPLES {

	/**
	* Abstract entity with collision properties
	*/
	class ICollidableEntity : public IEntity {
	protected:
		bool m_gravityState;
		bool m_collisionState;
		float m_collisionUpdateFrequency;
		float m_lastCollisionUpdateTimer;
		DE::CVector3F m_gravity;
		DE::CVector3F m_visiblePosition;
		std::unique_ptr<DE::IShape3DF> m_collisionShape;

	public:
		/**
		* Constructor
		* @param parent The parent
		*/
		ICollidableEntity(ILevel * parent);

		/**
		* Update the entity
		* @param time Elapsed time
		*/
		virtual void Update(float const time) = 0;

		/**
		* Set the gravity state
		* @param state The gravity state
		*/
		void SetGravityState(const bool state);

		/**
		* Set the collision state
		* @param state The collision state
		*/
		void SetCollisionState(const bool state);

		/**
		* Set the collision update frequency in ms
		* @param frequency The collision frequency
		*/
		void SetCollisionUpdateFrequency(const float frequency);

		/**
		* Get the gravity state
		* @return The gravity state
		*/
		const bool GetGravityState() const;

		/**
		* Get the collision state
		* @return The collision state
		*/
		const bool GetCollisionState() const;

		/**
		* Get the collision frequency
		* @return The collision frequency
		*/
		const float GetCollisionFrequency() const;

		/**
		* Get the gravity
		* @return The gravity
		*/
		const DE::CVector3F & GetGravity() const;

		/**
		* Get the visible position
		* @return The visible position
		*/
		const DE::CVector3F & GetVisiblePosition() const;

		/**
		* Get the entity collision shape
		* @return The entity collision shape
		*/
		DE::IShape3DF * GetCollisionShape();

	protected:
		/**
		* Callback used to handle event when the entity fall and touch the ground
		* @param gravity The gravity value before touching the ground
		*/
		virtual void OnEntityTouchedGround(const DE::CVector3F gravity) = 0;

		/**
		* Process the level collisions (movement + gravity) based on the shape.
		* The collision depends on the frequencies (in ms) set
		* @param movement The raw movement in unit per second
		* @param time The elapsed time
		*/
		void PerformLevelCollisions(DE::CVector3F movement, float const time);

		/**
		* Process the entity collisions (movement + gravity) based on the shape.
		* The collision depends on the frequencies (in ms) set
		* @param movement The raw movement in unit per second
		* @param time The elapsed time
		*/
		void PerformEntityCollisions(DE::CVector3F movement, float const time);
	};
}

#endif
