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

#ifndef __DAIDALOS_CENEMYSTATEFOLLOW_HPP
#define __DAIDALOS_CENEMYSTATEFOLLOW_HPP

#include <3D/Enemy/IEnemyState.hpp>
#include <3D/ICollidableEntity.hpp>

namespace daidalos {
	/**
	* Follow enemy state
	*/
	class CEnemyStateFollow : public daidalosgameengine::IEnemyState {
	private:
		float m_moveSpeed;
		float m_rotationSpeed;
		float m_stopDistance;
		float m_searchTimer;
		float m_searched;
		daidalosgameengine::ICollidableEntity * m_target;

	public:
		/**
		* Constructor
		* @param priority The priority
		* @param startAnimation The start animation frame
		* @param endAnimation The end animation frame
		* @param animationSpeed The animation speed
		* @param target The target entity
		* @param moveSpeed The move speed
		* @param rotationSpeed The rotation speed
		* @param stopDistance the stop distance
		* @param searchTime Time the entity search if the target is lost
		*/
		CEnemyStateFollow(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, daidalosgameengine::ICollidableEntity * target, const float moveSpeed, const float rotationSpeed, const float stopDistance, const float searchTime);

		/**
		* Destructor
		*/
		virtual ~CEnemyStateFollow();

		/**
		* Set the move speed.
		* @param moveSpeed The move speed
		*/
		void SetMoveSpeed(const float moveSpeed);

		/**
		* Set the rotation speed.
		* @param rotationSpeed The rotation speed
		*/
		void SetRotationSpeed(const float rotationSpeed);

		/**
		* Set the stop distance.
		* @param stopDistance The stop distance
		*/
		void SetStopDistance(const float stopDistance);

		/**
		* Set the search timer.
		* @param searchTimer The search timer
		*/
		void SetSearchTimer(const float searchTimer);

		/**
		* Set the target.
		* @param target The target
		*/
		void SetTarget(daidalosgameengine::ICollidableEntity * target);

		/**
		* Callback when the entity take damage
		*/
		virtual void OnEntityTakeDamage();

		/**
		* Update the entity
		* @param entity The entity
		* @param time Elapsed time
		*/
		virtual void Update(daidalosgameengine::IEnemyEntity * entity, const float time);

		/**
		* If the state is candidate for the next action
		* @param entity The entity
		* @return True if candidate
		*/
		virtual const bool IsCandidateForNextAction(daidalosgameengine::IEnemyEntity * entity);

		/**
		* Get the move speed.
		* @return The move speed
		*/
		const float GetMoveSpeed() const;

		/**
		* Get the rotation speed.
		* @return The rotation speed
		*/
		const float GetRotationSpeed() const;

		/**
		* Get the stop distance.
		* @return The stop distance
		*/
		const float GetStopDistance() const;

		/**
		* Get the search timer.
		* @return The search timer
		*/
		const float GetSearchTimer() const;

		/**
		* Get the target.
		* @return The target
		*/
		daidalosgameengine::ICollidableEntity * GetTarget() const;

	private:
		/**
		* Update the entity rotation towards the target
		* @param entity The entity
		* @param time Elapsed time
		*/
		void UpdateRotation(daidalosgameengine::IEnemyEntity * entity, const float time);
	};
}

#endif
