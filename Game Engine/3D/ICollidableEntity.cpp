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

#include "../Enums.hpp"
#include "../Level/ILevel.hpp"
#include "ICollidableEntity.hpp"

#define DEFAULT_COLLISION_UPDATE_FREQUENCY 16

namespace GE {
	/**
	* Constructor
	* @param parent The parent
	*/
	ICollidableEntity::ICollidableEntity(ILevel * parent) : IEntity(parent) {

		SetFlag(GetFlag() | ENTITY_FLAG_COLLIDABLE);

		this->m_gravityState = true;
		this->m_collisionState = true;
		this->m_collisionUpdateFrequency = DEFAULT_COLLISION_UPDATE_FREQUENCY;
		this->m_lastCollisionUpdateTimer = 0;
		this->m_gravity = DE::CVector3F();
	}

	/**
	* Set the gravity state
	* @param state The gravity state
	*/
	void ICollidableEntity::SetGravityState(const bool state) {
		this->m_gravityState = state;
	}

	/**
	* Set the collision state
	* @param state The collision state
	*/
	void ICollidableEntity::SetCollisionState(const bool state) {
		this->m_collisionState = state;
	}

	/**
	* Set the collision update frequency in ms
	* @param frequency The collision frequency
	*/
	void ICollidableEntity::SetCollisionUpdateFrequency(const float frequency) {
		this->m_collisionUpdateFrequency = frequency;
	}

	/**
	* Get the gravity state
	* @return The gravity state
	*/
	const bool ICollidableEntity::GetGravityState() const {
		return this->m_gravityState;
	}

	/**
	* Get the collision state
	* @return The collision state
	*/
	const bool ICollidableEntity::GetCollisionState() const {
		return this->m_collisionState;
	}

	/**
	* Get the collision frequency
	* @return The collision frequency
	*/
	const float ICollidableEntity::GetCollisionFrequency() const {
		return this->m_collisionUpdateFrequency;
	}

	/**
	* Get the gravity
	* @return The gravity
	*/
	const DE::CVector3F & ICollidableEntity::GetGravity() const {
		return this->m_gravity;
	}

	/**
	* Get the entity collision shape
	* @return The entity collision shape
	*/
	DE::IShape3DF * ICollidableEntity::GetCollisionShape() {
		return this->m_collisionShape.get();
	}

	/**
	* Process the collisions (movement + gravity) based on the shape.
	* The collision depends on the frequencies (in ms) set
	* @param movement The raw movement in unit per second
	* @param time The elapsed time
	*/
	void ICollidableEntity::PerformLevelCollisions(DE::CVector3F movement, const float time) {

		const float timeSecond = time / 1000.0f;
		SetVisiblePosition(GetVisiblePosition() + movement * timeSecond);

		// If a collision test is required 
		this->m_lastCollisionUpdateTimer += time;
		if(this->m_lastCollisionUpdateTimer >= this->m_collisionUpdateFrequency) {
			// Collision test with only the movement
			DE::CVector3F finalMovement = GetVisiblePosition() - this->m_collisionShape->GetPosition();
			GetParent()->GetScene()->Slide(*this->m_collisionShape.get(), finalMovement);

			// Collision test of the gravity
			finalMovement = this->m_gravity * timeSecond;
			DE::CVector3F finalPosition = this->m_collisionShape->GetPosition() + finalMovement;
			GetParent()->GetScene()->Slide(*this->m_collisionShape, finalMovement);

			// Reset gravity and state if the entity has touched the ground
			if(this->m_gravityState && this->m_collisionShape->GetPosition().y > finalPosition.y) {

				// Callback
				OnEntityTouchedGround(this->m_gravity);

				// Gravity damage
				this->m_gravity.y = 0;
			}

			// Update the visibility position
			SetVisiblePosition(this->m_collisionShape->GetPosition());

			// Reset the collision timer
			this->m_lastCollisionUpdateTimer = 0;
		} else {
			// Add the gravity when a collision test is not needed
			SetVisiblePosition(GetVisiblePosition() + this->m_gravity * timeSecond);
		}

		// Cancel the gravity if disabled
		if(!this->m_gravityState) {
			this->m_gravity.y = 0;
		}
	}

	/**
	* Process the entity collisions (movement + gravity) based on the shape.
	* The collision depends on the frequencies (in ms) set
	* @param movement The raw movement in unit per second
	* @param time The elapsed time
	*/
	void ICollidableEntity::PerformEntityCollisions(DE::CVector3F movement, const float time) {
		// Nothing to do
	}
}
