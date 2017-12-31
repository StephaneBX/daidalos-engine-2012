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

#include "IEnemyEntity.hpp"
#include "IEnemyState.hpp"

namespace GE {
	/**
	* Constructor
	* @param priority The priority
	*/
	IEnemyState::IEnemyState(const unsigned int priority) {
		this->m_priority = priority;
	}

	/**
	* Add a listener
	* @param entity The entity
	*/
	void IEnemyState::AddListener(IEnemyState * state) {
		this->m_listeners.push_back(state);
	}

	/**
	* Remove a listener
	* @param entity The entity
	*/
	void IEnemyState::RemoveListener(IEnemyState * state) {
		auto it = std::find(this->m_listeners.begin(), this->m_listeners.end(), state);
		if(it != this->m_listeners.end()) {
			this->m_listeners.erase(it);
		}
	}

	/**
	* Reset the state
	* @param entity The entity
	*/
	void IEnemyState::Reset(IEnemyEntity * entity) {
		entity->GetMeshNode()->StopAnimation();
	}

	/**
	* Callback when an entity is elected as the active state
	* @param state The state
	*/
	void IEnemyState::OnStateElected(IEnemyState * state) {
		// Nothing to do
	}

	/**
	* Callback when an entity interrupt its process
	* @param state The state
	*/
	void IEnemyState::OnStateInterrupted(IEnemyState * state) {
		// Nothing to do
	}

	/**
	* Callback when an entity is dismissed from the active state
	* @param state The state
	*/
	void IEnemyState::OnStateDismissed(IEnemyState * state) {
		// Nothing to do
	}

	/**
	* Callback when the entity take damage
	*/
	void IEnemyState::OnEntityTakeDamage() {
		// Nothing to do
	}

	/**
	* Get the priority
	* @return The priority
	*/
	const unsigned int IEnemyState::GetPriority() const {
		return this->m_priority;
	}

	/**
	* Process the level collisions (movement + gravity) based on the shape.
	* The collision depends on the frequencies (in ms) set
	* @param entity The enemy entity
	* @param movement The raw movement in unit per second
	* @param time The elapsed time
	*/
	void IEnemyState::PerformEnemyLevelCollisions(IEnemyEntity * entity, DE::CVector3F movement, const float time) {
		entity->PerformLevelCollisions(movement, time);
	}

	/**
	* Process the entity collisions (movement + gravity) based on the shape.
	* The collision depends on the frequencies (in ms) set
	* @param entity The enemy entity
	* @param movement The raw movement in unit per second
	* @param time The elapsed time
	*/
	void IEnemyState::PerformEnemyEntityCollisions(IEnemyEntity * entity, DE::CVector3F movement, const float time) {
		entity->PerformEntityCollisions(movement, time);
	}
}
