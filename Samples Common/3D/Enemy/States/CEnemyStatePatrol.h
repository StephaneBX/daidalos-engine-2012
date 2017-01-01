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

#ifndef __CENEMYSTATEPATROL_H
#define __CENEMYSTATEPATROL_H

#include "../IEnemyState.h"

namespace SAMPLES {
	/**
	* Patrol enemy state
	*/
	class CEnemyStatePatrol : public IEnemyState {
	protected:
		int m_currentWayPoint;
		float m_walkSpeed;
		float m_rotationSpeed;

	public:
		/**
		* Constructor
		* @param startAnimation The start animation frame
		* @param endAnimation The end animation frame
		* @param animationSpeed The animation speed
		* @param walkSpeed The walk speed
		* @param rotationSpeed The rotation speed
		* @param wayPoints The way points
		*/
		CEnemyStatePatrol(float startAnimation, float endAnimation, float animationSpeed, float walkSpeed, float rotationSpeed, std::vector<DE::CVector3F> & wayPoints);

		/**
		* Update the entity
		* @param entity The entity
		* @param time Elapsed time
		*/
		virtual void Update(IEnemyEntity * entity, float const time);

	private:
		/**
		* Update the entity rotation towards the current waypoint
		* @param entity The entity
		* @param time Elapsed time
		*/
		void UpdateRotation(IEnemyEntity * entity, float const time);
	};
}

#endif
