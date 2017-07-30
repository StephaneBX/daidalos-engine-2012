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

#ifndef __GAMEENGINE_IENEMYSTATE_HPP
#define __GAMEENGINE_IENEMYSTATE_HPP

#include <Core/Utility/CVector2.hpp>
#include <Core/Utility/CVector3.hpp>

namespace GE {
	class IEnemyEntity;

	/**
	* Abstract enemy state
	*/
	class IEnemyState {
	protected:
		unsigned int m_priority;
		float m_animationSpeed;
		DE::CVector2F m_animationFrames;
		std::vector<DE::CVector3F> m_wayPoints;
		std::vector<IEnemyState *> m_listeners;

	public:
		/**
		* Constructor
		* @param priority The priority
		*/
		IEnemyState(const unsigned int priority);

		/**
		* Add a listener
		* @param state The state
		*/
		void AddListener(IEnemyState * state);

		/**
		* Remove a listener
		* @param state The state
		*/
		void RemoveListener(IEnemyState * state);

		/**
		* Reset the state
		* @param entity The entity
		*/
		virtual void Reset(IEnemyEntity * entity);

		/**
		* Callback when an entity is elected as the active state
		* @param state The state
		*/
		virtual void OnStateElected(IEnemyState * state);

		/**
		* Callback when an entity interrupt its process
		* @param state The state
		*/
		virtual void OnStateInterrupted(IEnemyState * state);

		/**
		* Callback when an entity is dismissed from the active state
		* @param state The state
		*/
		virtual void OnStateDismissed(IEnemyState * state);

		/**
		* Callback when the entity take damage
		*/
		virtual void OnEntityTakeDamage();

		/**
		* Update the entity
		* @param entity The entity
		* @param time Elapsed time
		*/
		virtual void Update(IEnemyEntity * entity, const float time) = 0;

		/**
		* If the state is candidate for the next action
		* @param entity The entity
		* @return True if candidate
		*/
		virtual const bool IsCandidateForNextAction(IEnemyEntity * entity) = 0;

		/**
		* Get the priority
		* @return The priority
		*/
		const unsigned int GetPriority() const;

	protected:
		/**
		* Process the level collisions (movement + gravity) based on the shape.
		* The collision depends on the frequencies (in ms) set
		* @param entity The enemy entity
		* @param movement The raw movement in unit per second
		* @param time The elapsed time
		*/
		void PerformEnemyLevelCollisions(IEnemyEntity * entity, DE::CVector3F movement, const float time);

		/**
		* Process the entity collisions (movement + gravity) based on the shape.
		* The collision depends on the frequencies (in ms) set
		* @param entity The enemy entity
		* @param movement The raw movement in unit per second
		* @param time The elapsed time
		*/
		void PerformEnemyEntityCollisions(IEnemyEntity * entity, DE::CVector3F movement, const float time);
	};
}

#endif
