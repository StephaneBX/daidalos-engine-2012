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
#include "CEnemyStateFollow.hpp"

namespace DAIDALOS {
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
	CEnemyStateFollow::CEnemyStateFollow(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, GE::ICollidableEntity * target, const float moveSpeed, const float rotationSpeed, const float stopDistance, const float searchTime) : IEnemyState(priority) {
		this->SetAnimationFrames(DE::CVector2F(startAnimation, endAnimation));
		this->SetAnimationSpeed(animationSpeed);
		this->m_target = target;
		this->m_moveSpeed = moveSpeed;
		this->m_rotationSpeed = rotationSpeed;
		this->m_stopDistance = stopDistance;
		this->m_searchTimer = searchTime;
		this->m_searched = 0;
	}

	/**
	* Set the move speed.
	* @param moveSpeed The move speed
	*/
	void CEnemyStateFollow::SetMoveSpeed(const float moveSpeed) {
		this->m_moveSpeed = moveSpeed;
	}

	/**
	* Set the rotation speed.
	* @param rotationSpeed The rotation speed
	*/
	void CEnemyStateFollow::SetRotationSpeed(const float rotationSpeed) {
		this->m_rotationSpeed = rotationSpeed;
	}

	/**
	* Set the stop distance.
	* @param stopDistance The stop distance
	*/
	void CEnemyStateFollow::SetStopDistance(const float stopDistance) {
		this->m_stopDistance = stopDistance;
	}

	/**
	* Set the search timer.
	* @param searchTimer The search timer
	*/
	void CEnemyStateFollow::SetSearchTimer(const float searchTimer) {
		this->m_searchTimer = searchTimer;
	}

	/**
	* Set the target.
	* @param target The target
	*/
	void CEnemyStateFollow::SetTarget(GE::ICollidableEntity * target) {
		this->m_target = target;
	}

	/**
	* Reset the state
	* @param entity The entity
	*/
	void CEnemyStateFollow::Reset(GE::IEnemyEntity * entity) {
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
	void CEnemyStateFollow::Update(GE::IEnemyEntity * entity, const float time) {

		// If the entity is in search mode, update the search timer
		if(this->m_searched > 0) {
			this->m_searched -= time;
		}

		DE::CVector3F movement = DE::CVector3F();
		if(entity->GetMeshNode()->GetAnimationState() != DE::MESH_ANIMATION_STATE_PLAYING) {
			entity->GetMeshNode()->SetLooped(true);
			entity->GetMeshNode()->StartAnimation(this->GetAnimationFrames().x, this->GetAnimationFrames().y, this->GetAnimationSpeed());
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
	const bool CEnemyStateFollow::IsCandidateForNextAction(GE::IEnemyEntity * entity) {
		return this->m_target->IsEnabled() && (!entity->GetCameraNode()->FrustumCull(this->m_target->GetCollisionShape()) || (entity->GetCameraNode()->FrustumCull(this->m_target->GetCollisionShape()) && this->m_searched > 0));
	}

	/**
	* Get the move speed.
	* @return The move speed
	*/
	const float CEnemyStateFollow::GetMoveSpeed() const {
		return this->m_moveSpeed;
	}

	/**
	* Get the rotation speed.
	* @return The rotation speed
	*/
	const float CEnemyStateFollow::GetRotationSpeed() const {
		return this->m_rotationSpeed;
	}

	/**
	* Get the stop distance.
	* @return The stop distance
	*/
	const float CEnemyStateFollow::GetStopDistance() const {
		return this->m_stopDistance;
	}

	/**
	* Get the search timer.
	* @return The search timer
	*/
	const float CEnemyStateFollow::GetSearchTimer() const {
		return this->m_searchTimer;
	}

	/**
	* Get the target.
	* @return The target
	*/
	GE::ICollidableEntity * CEnemyStateFollow::GetTarget() const {
		return this->m_target;
	}

	/**
	* Update the entity rotation towards the target
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CEnemyStateFollow::UpdateRotation(GE::IEnemyEntity * entity, const float time) {
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

