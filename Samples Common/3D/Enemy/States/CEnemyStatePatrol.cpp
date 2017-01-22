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
#include "CEnemyStatePatrol.h"

namespace SAMPLES {
	#define EPSILON_DISTANCE 1.0f

	/**
	* Constructor
	* @param priority The priority
	* @param startAnimation The start animation frame
	* @param endAnimation The end animation frame
	* @param animationSpeed The animation speed
	* @param walkSpeed The walk speed
	* @param rotationSpeed The rotation speed
	* @param wayPoints The way points
	*/
	CEnemyStatePatrol::CEnemyStatePatrol(unsigned int priority, float startAnimation, float endAnimation, float animationSpeed, float walkSpeed, float rotationSpeed, std::vector<DE::CVector3F> & wayPoints) : IEnemyState(priority) {
		this->m_animationFrames.x = startAnimation;
		this->m_animationFrames.y = endAnimation;
		this->m_animationSpeed = animationSpeed;
		this->m_walkSpeed = walkSpeed;
		this->m_rotationSpeed = rotationSpeed;
		this->m_wayPoints = wayPoints;
		this->m_currentWayPoint = 0;
	}

	/**
	* Update the entity
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CEnemyStatePatrol::Update(IEnemyEntity * entity, float const time) {

		DE::CVector3F movement = DE::CVector3F();
		if(this->m_wayPoints.size() > 0) {
			if(entity->GetMeshNode()->GetAnimationState() != DE::MESH_ANIMATION_STATE_PLAYING) {
				entity->GetMeshNode()->SetLooped(true);
				entity->GetMeshNode()->StartAnimation(this->m_animationFrames.x, this->m_animationFrames.y, this->m_animationSpeed);
			}

			// Change direction if necessary
			if((DE::CVector3F(entity->GetVisiblePosition().x, this->m_wayPoints[this->m_currentWayPoint].y, entity->GetVisiblePosition().z) - this->m_wayPoints[this->m_currentWayPoint]).Length() <= EPSILON_DISTANCE) {
				this->m_currentWayPoint = (this->m_currentWayPoint + 1) % this->m_wayPoints.size();
				entity->GetMeshNode()->StopAnimation();

				// Interrupted
				for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
					(*it)->OnStateInterrupted(this);
				}
			}

			// Update the movement
			movement = entity->GetCameraNode()->GetForwardVector() * this->m_walkSpeed;

			// Update the rotation
			UpdateRotation(entity, time);
		} else {
			// Interrupted
			for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
				(*it)->OnStateInterrupted(this);
			}
		}

		PerformEnemyLevelCollisions(entity, movement, time);
	}

	/**
	* If the state is candidate for the next action
	* @param entity The entity
	* @return True if candidate
	*/
	const bool CEnemyStatePatrol::IsCandidateForNextAction(IEnemyEntity * entity) {
		return true;
	}

	/**
	* Update the entity rotation towards the current waypoint
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CEnemyStatePatrol::UpdateRotation(IEnemyEntity * entity, float const time) {
		const float timePerSecond = time / 1000.0f;

		// Update the entity rotation so that it follows the waypoint during the walk
		float rotation = this->m_rotationSpeed * timePerSecond;

		// Check the entity - waypoint angle
		DE::CVector3F entityToWaypoint = (this->m_wayPoints[this->m_currentWayPoint] - entity->GetVisiblePosition()).Normalize();

		// Horizontal rotation way
		DE::CVector2F entityToWaypointHorizontal = DE::CVector2F(entityToWaypoint.x, entityToWaypoint.z).Normalize();
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

