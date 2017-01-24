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
#include "CEnemyStateFollow.h"

namespace SAMPLES {
	/**
	* Constructor
	* @param priority The priority
	* @param startAnimation The start animation frame
	* @param endAnimation The end animation frame
	* @param animationSpeed The animation speed
	* @param target The target entity
	* @param moveSpeed The walk speed
	* @param rotationSpeed The rotation speed
	* @param stopDistance the stop distance
	* @param searchTime Time the entity search if the target is lost
	*/
	CEnemyStateFollow::CEnemyStateFollow(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, ICollidableEntity * target, const float moveSpeed, const float rotationSpeed, const float stopDistance, const float searchTime) : IEnemyState(priority) {
		this->m_animationFrames.x = startAnimation;
		this->m_animationFrames.y = endAnimation;
		this->m_animationSpeed = animationSpeed;
		this->m_target = target;
		this->m_moveSpeed = moveSpeed;
		this->m_rotationSpeed = rotationSpeed;
		this->m_stopDistance = stopDistance;
		this->m_searchTimer = searchTime;
		this->m_searched = 0;
	}

	/**
	* Reset the state
	* @param entity The entity
	*/
	void CEnemyStateFollow::Reset(IEnemyEntity * entity) {
		IEnemyState::Reset(entity);
	}

	/**
	* Callback when the entity take damage
	*/
	void CEnemyStateFollow::OnEntityTakeDamage() {
		this->m_searched = this->m_searchTimer;
	}

	/**
	* Update the entity
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CEnemyStateFollow::Update(IEnemyEntity * entity, const float time) {

		// If the entity is in search mode, update the search timer
		if(this->m_searched > 0) {
			this->m_searched -= time;
		}

		DE::CVector3F movement = DE::CVector3F();
		if(entity->GetMeshNode()->GetAnimationState() != DE::MESH_ANIMATION_STATE_PLAYING) {
			entity->GetMeshNode()->SetLooped(true);
			entity->GetMeshNode()->StartAnimation(this->m_animationFrames.x, this->m_animationFrames.y, this->m_animationSpeed);
		}

		// Reset the search timer if the target is in sight
		if(!entity->GetCameraNode()->FrustumCull(this->m_target->GetCollisionShape())) {
			this->m_searched = this->m_searchTimer;
		}

		// Follow the target
		movement = entity->GetCameraNode()->GetForwardVector() * this->m_moveSpeed;

		// Update the rotation
		UpdateRotation(entity, time);

		PerformEnemyLevelCollisions(entity, movement, time);
	}

	/**
	* If the state is candidate for the next action
	* @param entity The entity
	* @return True if candidate
	*/
	const bool CEnemyStateFollow::IsCandidateForNextAction(IEnemyEntity * entity) {
		return this->m_target->IsEnabled() && (!entity->GetCameraNode()->FrustumCull(this->m_target->GetCollisionShape()) || (entity->GetCameraNode()->FrustumCull(this->m_target->GetCollisionShape()) && this->m_searched > 0));
	}

	/**
	* Update the entity rotation towards the target
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CEnemyStateFollow::UpdateRotation(IEnemyEntity * entity, const float time) {
		const float timePerSecond = time / 1000.0f;

		// Update the entity rotation so that it follows the waypoint during the walk
		float rotation = this->m_rotationSpeed * timePerSecond;

		// Check the entity - waypoint angle
		DE::CVector3F entityToTarget = (this->m_target->GetVisiblePosition() - entity->GetVisiblePosition()).Normalize();

		// Horizontal rotation way
		DE::CVector2F entityToWaypointHorizontal = DE::CVector2F(entityToTarget.x, entityToTarget.z).Normalize();
		DE::CVector2F entityForwardHorizontal = DE::CVector2F(entity->GetCameraNode()->GetForwardVector().x, entity->GetCameraNode()->GetForwardVector().z).Normalize();

		float rotationY = entity->GetMeshNode()->GetRotation().y;
		if(atan2f(entityForwardHorizontal.CrossProduct(entityToWaypointHorizontal), entityForwardHorizontal.DotProduct(entityToWaypointHorizontal)) > 0) {
			rotationY -= rotation;
		} else {
			rotationY += rotation;
		}

		// Update the entity rotation
		entity->GetMeshNode()->SetRotation(DE::CVector3F(entity->GetMeshNode()->GetRotation().x, rotationY, entity->GetMeshNode()->GetRotation().z));
	}
}

