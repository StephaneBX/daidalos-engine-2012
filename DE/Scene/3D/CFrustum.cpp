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
#include "../../Core/Core.hpp"
#include "../../Scene/3D/CFrustum.hpp"

namespace daidalosengine
{
	CFrustum::CFrustum() {
		LOG_TRACE();
	}

	CFrustum::CFrustum(const float angle, const float ratio, const float nearDistance, const float farDistance) {
		LOG_TRACE();

		Set(angle, ratio, nearDistance, farDistance);
	}

	CFrustum::~CFrustum() {
		LOG_TRACE();
	}

	void CFrustum::Set(const float angle, const float ratio, const float nearDistance, const float farDistance) {
		LOG_TRACE();

		this->m_angle = angle;
		this->m_ratio = ratio;
		this->m_nearDistance = nearDistance;
		this->m_farDistance = farDistance;

		this->m_tang = angle * 0.5F;
		this->m_nearSize.y = this->m_nearDistance * this->m_tang;
		this->m_nearSize.x = this->m_nearSize.y * this->m_ratio;
		this->m_farSize.y = this->m_farDistance * this->m_tang;
		this->m_farSize.x = this->m_farSize.y * this->m_ratio;
	}

	void CFrustum::Update(const CVector3F & position, const CVector3F & lookAt) {
		LOG_TRACE();

		this->m_lookAt = lookAt;
		CVector3F up = CVector3F(0,1,0);
		CVector3F nearCenter;
		CVector3F farCenter;
		CVector3F x;
		CVector3F y;
		CVector3F z;

		this->m_position = position;

		// Z axis camera
		z = (lookAt - this->m_position).Normalize();

		// X axis camera
		x = up.CrossProduct(z).Normalize();

		// Y axis camera
		y = z.CrossProduct(x).Normalize();
		this->m_up = y;

		// World space calculation

		// Find the center of the planes
		nearCenter = this->m_position + z * this->m_nearDistance;
		farCenter = this->m_position + z * this->m_farDistance;

		// Find the forward vector
		this->m_forward = (nearCenter - this->m_position).Normalize();

		// Find the for points of the near plane
		this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_TOP_LEFT] = nearCenter + y * this->m_nearSize.y - x * this->m_nearSize.x;
		this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT] = nearCenter + y * this->m_nearSize.y + x * this->m_nearSize.x;
		this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT] = nearCenter - y * this->m_nearSize.y - x * this->m_nearSize.x;
		this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT] = nearCenter - y * this->m_nearSize.y + x * this->m_nearSize.x;

		// Find the for points of the far plane
		this->m_worldSpacePoints[FRUSTUM_POINT_FAR_TOP_LEFT] = farCenter + y * this->m_farSize.y - x * this->m_farSize.x;
		this->m_worldSpacePoints[FRUSTUM_POINT_FAR_TOP_RIGHT] = farCenter + y * this->m_farSize.y + x * this->m_farSize.x;
		this->m_worldSpacePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT] = farCenter - y * this->m_farSize.y - x * this->m_farSize.x;
		this->m_worldSpacePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT] = farCenter - y * this->m_farSize.y + x * this->m_farSize.x;


		// Local space calculation

		// Find the center of the planes
		nearCenter = z * this->m_nearDistance;
		farCenter = z * this->m_farDistance;

		// Find the for points of the near plane
		this->m_localSpacePoints[FRUSTUM_POINT_NEAR_TOP_LEFT] = nearCenter + y * this->m_nearSize.y - x * this->m_nearSize.x;
		this->m_localSpacePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT] = nearCenter + y * this->m_nearSize.y + x * this->m_nearSize.x;
		this->m_localSpacePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT] = nearCenter - y * this->m_nearSize.y - x * this->m_nearSize.x;
		this->m_localSpacePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT] = nearCenter - y * this->m_nearSize.y + x * this->m_nearSize.x;

		// Find the for points of the far plane
		this->m_localSpacePoints[FRUSTUM_POINT_FAR_TOP_LEFT] = farCenter + y * this->m_farSize.y - x * this->m_farSize.x;
		this->m_localSpacePoints[FRUSTUM_POINT_FAR_TOP_RIGHT] = farCenter + y * this->m_farSize.y + x * this->m_farSize.x;
		this->m_localSpacePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT] = farCenter - y * this->m_farSize.y - x * this->m_farSize.x;
		this->m_localSpacePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT] = farCenter - y * this->m_farSize.y + x * this->m_farSize.x;


		// Table creation for the frustum planes
		CVector3F leftFrustrum[3]   = {this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT], this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_worldSpacePoints[FRUSTUM_POINT_FAR_TOP_LEFT]};
		CVector3F rightFrustrum[3]  = {this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT], this->m_worldSpacePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT]};
		CVector3F topFrustrum[3]    = {this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_worldSpacePoints[FRUSTUM_POINT_FAR_TOP_RIGHT]};
		CVector3F bottomFrustrum[3] = {this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT], this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT], this->m_worldSpacePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT]};
		CVector3F nearFrustrum[3]   = {this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_worldSpacePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT]};
		CVector3F farFrustrum[3]    = {this->m_worldSpacePoints[FRUSTUM_POINT_FAR_TOP_LEFT], this->m_worldSpacePoints[FRUSTUM_POINT_FAR_TOP_RIGHT], this->m_worldSpacePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT]};

		// Update the frustum planes
		this->m_frustum[FRUSTUM_PLANE_LEFT].Build(leftFrustrum);
		this->m_frustum[FRUSTUM_PLANE_RIGHT].Build(rightFrustrum);
		this->m_frustum[FRUSTUM_PLANE_TOP].Build(topFrustrum);
		this->m_frustum[FRUSTUM_PLANE_BOTTOM].Build(bottomFrustrum);
		this->m_frustum[FRUSTUM_PLANE_NEAR].Build(nearFrustrum);
		this->m_frustum[FRUSTUM_PLANE_FAR].Build(farFrustrum);
	}

	const bool CFrustum::BackfaceCull(const CVector3F & vertex, const CVector3F & faceNormal) const {
		LOG_TRACE();

		return (this->m_position - vertex).Normalize().DotProduct(faceNormal) <= 0;
	}

	const bool CFrustum::FrustumCull(IShape3DF * shape) const {
		LOG_TRACE();

		bool result = false;

		for(int i = 0; i < FRUSTUM_PLANE_SIZE && !result; i++) {
			float dist = this->m_frustum[i].PointDistance(shape->GetPosition());
			if(dist < -shape->GetInnerDepth(this->m_frustum[FRUSTUM_PLANE_NEAR])) {
				result = true;
			}
		}

		return result;
	}

	const float CFrustum::GetFov() const {
		LOG_TRACE();

		return this->m_angle;
	}

	const float CFrustum::GetNearDistance() const {
		LOG_TRACE();

		return this->m_nearDistance;
	}

	const float CFrustum::GetFarDistance() const {
		LOG_TRACE();

		return this->m_farDistance;
	}

	const CVector3F & CFrustum::GetPosition() const {
		LOG_TRACE();

		return this->m_position;
	}

	const CVector3F & CFrustum::GetForward() const {
		LOG_TRACE();

		return this->m_forward;
	}

	const CVector3F & CFrustum::GetLookAt() const {
		LOG_TRACE();

		return this->m_lookAt;
	}

	const CVector3F & CFrustum::GetUp() const {
		LOG_TRACE();

		return this->m_up;
	}

	const CVector3F * CFrustum::GetLocalSpacePoints() const {
		LOG_TRACE();

		return this->m_localSpacePoints;
	}
}