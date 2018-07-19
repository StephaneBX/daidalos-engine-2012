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

#ifndef __DAIDALOS_CENEMYSTATEPATROL_HPP
#define __DAIDALOS_CENEMYSTATEPATROL_HPP

#include <3D/Enemy/IEnemyState.hpp>

namespace daidalos {
	/**
	* Patrol enemy state
	*/
	class CEnemyStatePatrol : public daidalosgameengine::IEnemyState {
	private:
		int m_currentWayPoint;
		float m_walkSpeed;
		float m_rotationSpeed;

	public:
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
		CEnemyStatePatrol(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, const float walkSpeed, const float rotationSpeed, std::vector<daidalosengine::CVector3F> & wayPoints);

		/**
		* Destructor
		*/
		virtual ~CEnemyStatePatrol();

		/**
		* Set the current way point.
		* @param wayPoint The current way point
		*/
		void SetCurrentWayPoint(const int wayPoint);

		/**
		* Set the walk speed.
		* @param walkSpeed The walk speed
		*/
		void SetWalkSpeed(const float walkSpeed);

		/**
		* Set the rotation speed.
		* @param rotationSpeed The rotation speed
		*/
		void SetRotationSpeed(const float rotationSpeed);

		/**
		* Update the entity
		* @param entity The entity
		* @param time Elapsed time
		*/
		virtual void Update(daidalosgameengine::IEnemyEntity * entity, const float time) override;

		/**
		* If the state is candidate for the next action
		* @param entity The entity
		* @return True if candidate
		*/
		virtual bool IsCandidateForNextAction(daidalosgameengine::IEnemyEntity * entity) override;

		/**
		* Get the current way point.
		* @return The current way point
		*/
		int GetCurrentWayPoint() const;

		/**
		* Get the walk speed.
		* @return The walk speed
		*/
		float GetWalkSpeed() const;

		/**
		* Get the rotation speed.
		* @return The rotation speed
		*/
		float GetRotationSpeed() const;

	private:
		/**
		* Update the entity rotation towards the current waypoint
		* @param entity The entity
		* @param time Elapsed time
		*/
		void UpdateRotation(daidalosgameengine::IEnemyEntity * entity, const float time);
	};
}

#endif
