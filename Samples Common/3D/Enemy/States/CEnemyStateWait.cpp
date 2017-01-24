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

#include "../IEnemyEntity.h"
#include "CEnemyStateWait.h"

namespace SAMPLES {
	/**
	* Constructor
	* @param priority The priority
	* @param startAnimation The start animation frame
	* @param endAnimation The end animation frame
	* @param animationSpeed The animation speed
	* @param waitTime The wait time
	*/
	CEnemyStateWait::CEnemyStateWait(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, const float waitTime) : IEnemyState(priority) {
		this->m_active = false;
		this->m_waitTime = waitTime;
		this->m_waited = 0;
		this->m_animationFrames.x = startAnimation;
		this->m_animationFrames.y = endAnimation;
		this->m_animationSpeed = animationSpeed;
	}

	/**
	* Reset the state
	* @param entity The entity
	*/
	void CEnemyStateWait::Reset(IEnemyEntity * entity) {
		IEnemyState::Reset(entity);
	}

	/**
	* Callback when an entity interrupt its process
	* @param state The state
	*/
	void CEnemyStateWait::OnStateInterrupted(IEnemyState * state) {
		this->m_active = true;
	}

	/**
	* Update the entity
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CEnemyStateWait::Update(IEnemyEntity * entity, const float time) {

		this->m_waited += time;

		if(entity->GetMeshNode()->GetAnimationState() != DE::MESH_ANIMATION_STATE_PLAYING) {
			entity->GetMeshNode()->SetLooped(true);
			entity->GetMeshNode()->StartAnimation(this->m_animationFrames.x, this->m_animationFrames.y, this->m_animationSpeed);
		}

		PerformEnemyLevelCollisions(entity, DE::CVector3F(), time);

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
	const bool CEnemyStateWait::IsCandidateForNextAction(IEnemyEntity * entity) {
		return this->m_active;
	}
}

