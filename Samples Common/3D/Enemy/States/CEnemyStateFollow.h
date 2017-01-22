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

#ifndef __CENEMYSTATEFOLLOW_H
#define __CENEMYSTATEFOLLOW_H

#include "../IEnemyState.h"

namespace SAMPLES {
	/**
	* Follow enemy state
	*/
	class CEnemyStateFollow : public IEnemyState {
	protected:
		float m_moveSpeed;
		float m_rotationSpeed;
		float m_stopDistance;
		float m_searchTimer;
		float m_searched;
		ICollidableEntity * m_target;

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
		CEnemyStateFollow(unsigned int priority, float startAnimation, float endAnimation, float animationSpeed, ICollidableEntity * target, float moveSpeed, float rotationSpeed, float stopDistance, float searchTime);

		/**
		* Reset the state
		* @param entity The entity
		*/
		virtual void Reset(IEnemyEntity * entity);

		/**
		* Callback when the entity take damage
		*/
		virtual void OnEntityTakeDamage();

		/**
		* Update the entity
		* @param entity The entity
		* @param time Elapsed time
		*/
		virtual void Update(IEnemyEntity * entity, float const time);

		/**
		* If the state is candidate for the next action
		* @param entity The entity
		* @return True if candidate
		*/
		virtual const bool IsCandidateForNextAction(IEnemyEntity * entity);

	private:
		/**
		* Update the entity rotation towards the target
		* @param entity The entity
		* @param time Elapsed time
		*/
		void UpdateRotation(IEnemyEntity * entity, float const time);
	};
}

#endif
