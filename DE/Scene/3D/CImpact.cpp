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

#include "../../Core/Logger/ILogger.hpp"
#include "../../Scene/3D/CImpact.hpp"

namespace DE {
	CImpact::CImpact() {
		LOG_TRACE();

		ClearImpact();
	}

	void CImpact::Initialize(const CVector3F & startPoint, const CVector3F & endPoint) {
		LOG_TRACE();

		this->m_startPoint = startPoint;
		this->m_endPoint = endPoint;
		this->m_distance = (this->m_endPoint - this->m_startPoint);
		ClearImpact();
	}

	void CImpact::ClearImpact() {
		LOG_TRACE();

		this->m_impactFraction = 1;
		this->m_realFraction = 1;
	}

	void CImpact::SetImpact(const CVector3F & planeNormal, const float impactFraction, const float realFraction) {
		LOG_TRACE();

		this->m_planeNormal = planeNormal;
		this->m_impactFraction = impactFraction;
		this->m_realFraction = realFraction;

		// Boundary test - Avoid slugish vertical collisions
		if(this->m_impactFraction < 0) {
			this->m_impactFraction = 0;
		}
	}

	void CImpact::SetNearestImpact(const CVector3F & planeNormal, const float impactFraction, const float realFraction) {
		LOG_TRACE();

		if(realFraction < this->m_realFraction) {
			SetImpact(planeNormal, impactFraction, realFraction);
		}
	}

	const bool CImpact::IsImpact() const {
		LOG_TRACE();

		return this->m_realFraction < 1.0F;
	}

	const float CImpact::GetImpactFraction() const {
		LOG_TRACE();

		return this->m_impactFraction;
	}

	const float CImpact::GetRealFraction() const {
		LOG_TRACE();

		return this->m_realFraction;
	}

	const CVector3F & CImpact::GetDistance() const {
		LOG_TRACE();

		return this->m_distance;
	}

	const CVector3F & CImpact::GetPlaneNormal() const {
		LOG_TRACE();

		return this->m_planeNormal;
	}

	const CVector3F & CImpact::GetStartPoint() const {
		LOG_TRACE();

		return this->m_startPoint;
	}

	const CVector3F & CImpact::GetEndPoint() const {
		LOG_TRACE();

		return this->m_endPoint;
	}
}
