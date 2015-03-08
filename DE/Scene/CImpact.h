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

#ifndef __CIMPACT_H
#define __CIMPACT_H

#include "../Core/Utility/CVector3.h"

namespace DE {
	/**
	* Handle an impact for collision detection
	*/
	class CImpact {
	private:
		float m_impactFraction;
		float m_realFraction;
		CVector3F m_startPoint;
		CVector3F m_endPoint;
		CVector3F m_distance;
		CVector3F m_planeNormal;

	public:
		/**
		* Constructor
		*/
		CImpact();

		/**
		* Initialize the impact
		* @param startPoint The starting point
		* @param endPoint The ending point
		*/
		void Initialize(const CVector3F & startPoint, const CVector3F & endPoint);

		/**
		* Clear the impact fraction values
		*/
		void ClearImpact();

		/**
		* Set impact values
		* @param planeNormal The plane normal
		* @param impactFraction The impact fraction
		* @param realFraction The real faction
		*/
		void SetImpact(const CVector3F & planeNormal, const float impactFraction, const float realFraction);

		/**
		* Set the nearest impact between the current and the given one
		* @param planeNormal The plane normal
		* @param impactFraction The impact fraction
		* @param realFraction The real faction
		*/
		void SetNearestImpact(const CVector3F & planeNormal, const float impactFraction, const float realFraction);

		/**
		* Check if an impact has happended (real fraction < 1)
		* @return True if an impact has happenened
		*/
		const bool IsImpact() const;

		/**
		* Get the impact fraction
		* @return The impact fraction
		*/
		const float GetImpactFraction() const;

		/**
		* Get the real fraction
		* @return The real fraction
		*/
		const float GetRealFraction() const;

		/**
		* Return the distance to perform
		* @return The distance to perform
		*/
		const CVector3F & GetDistance() const;

		/**
		* Get the plane normal
		* @return The plane normal
		*/
		const CVector3F & GetPlaneNormal() const;

		/**
		* Get the starting point
		* @return The starting point
		*/
		const CVector3F & GetStartPoint() const;

		/**
		* Get the ending point
		* @return The ending point
		*/
		const CVector3F & GetEndPoint() const;
	};
}

#endif
