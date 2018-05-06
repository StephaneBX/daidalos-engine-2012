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

#include <3D/Enemy/IEnemyEntity.hpp>
#include "CEnemyStateWait.hpp"

namespace daidalos {
	/**
	* Constructor
	* @param priority The priority
	* @param startAnimation The start animation frame
	* @param endAnimation The end animation frame
	* @param animationSpeed The animation speed
	* @param waitTime The wait time
	*/
	CEnemyStateWait::CEnemyStateWait(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, const float waitTime) : m_waitTime(waitTime), IEnemyState(priority) {
		this->m_active = false;
		this->m_waited = 0;
		this->SetAnimationFrames(daidalosengine::CVector2F(startAnimation, endAnimation));
		this->SetAnimationSpeed(animationSpeed);
	}

	/**
	* Destructor
	*/
	CEnemyStateWait::~CEnemyStateWait() {}

	/**
	* Set the active status.
	* @param active The active status
	*/
	void CEnemyStateWait::SetActive(const bool active) {
		this->m_active = active;
	}

	/**
	* Set the wait time.
	* @param waitTime The wait time
	*/
	void CEnemyStateWait::SetWaitTime(const float waitTime) {
		this->m_waitTime = waitTime;
	}

	/**
	* Callback when an entity interrupt its process
	* @param state The state
	*/
	void CEnemyStateWait::OnStateInterrupted(daidalosgameengine::IEnemyState * state) {
		this->m_active = true;
	}

	/**
	* Update the entity
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CEnemyStateWait::Update(daidalosgameengine::IEnemyEntity * entity, const float time) {

		this->m_waited += time;

		if(entity->GetMeshNode()->GetAnimationState() != daidalosengine::MESH_ANIMATION_STATE_PLAYING) {
			entity->GetMeshNode()->SetLooped(true);
			entity->GetMeshNode()->StartAnimation(this->GetAnimationFrames().x, this->GetAnimationFrames().y, this->GetAnimationSpeed());
		}

		PerformEnemyLevelCollisions(entity, daidalosengine::CVector3F(), time);

		// End wait state
		if(this->m_waited >= this->m_waitTime) {
			entity->GetMeshNode()->StopAnimation();

			this->m_active = false;
			this->m_waited = 0;
		}
	}

	/**
	* If the state is candidate for the next action
	* @param entity The entity
	* @return True if candidate
	*/
	const bool CEnemyStateWait::IsCandidateForNextAction(daidalosgameengine::IEnemyEntity * entity) {
		return this->m_active;
	}

	/**
	* Get the active status.
	* @return The active status
	*/
	const bool CEnemyStateWait::GetActive() {
		return this->m_active;
	}

	/**
	* Get the wait time.
	* @return The wait time
	*/
	const float CEnemyStateWait::GetWaitTime() {
		return this->m_waitTime;
	}
}

