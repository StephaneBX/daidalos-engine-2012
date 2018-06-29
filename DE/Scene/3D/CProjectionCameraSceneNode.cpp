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
#include "../../Core/IRenderer.hpp"
#include "../../Scene/3D/I3DScene.hpp"
#include "../../Scene/3D/CProjectionCameraSceneNode.hpp"

namespace daidalosengine {
	CProjectionCameraSceneNode::CProjectionCameraSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const float angle, const float ratio, const float nearDistance, const float farDistance) : ICameraSceneNode(scene, parent, name, position, rotation, nearDistance, farDistance) {
		LOG_TRACE();

		SetAngle(angle);
		SetRatio(ratio);
	}

	CProjectionCameraSceneNode::~CProjectionCameraSceneNode() {
		LOG_TRACE();
	}

	void CProjectionCameraSceneNode::SetAngle(const float angle) {
		LOG_TRACE();

		this->m_angle = angle;
	}

	void CProjectionCameraSceneNode::SetRatio(const float ratio) {
		LOG_TRACE();

		this->m_ratio = ratio;
	}

	void CProjectionCameraSceneNode::Update(const float time) {
		LOG_TRACE();

		// Update the camera information
		ICameraSceneNode::Update(time);

		// Update the projection information
		CMatrix projMatrix;
		projMatrix.PerspectiveFOV(this->m_angle, this->m_ratio, GetNearDistance(), GetFarDistance());
		SetProjMatrix(projMatrix);

		// Update the view proj matrix
		SetViewProjMatrix(GetViewMatrix() * GetProjMatrix());

		/**
		* Update the view frustum information
		*/
		float tang;
		CVector3F nearCenter;
		CVector3F farCenter;
		CVector2F nearSize;
		CVector2F farSize;
		CVector3F x;

		tang = this->m_angle * 0.5F;
		nearSize.y = GetNearDistance() * tang;
		nearSize.x = nearSize.y * this->m_ratio;
		farSize.y = GetFarDistance() * tang;
		farSize.x = farSize.y * this->m_ratio;

		// Z axis camera
		SetForwardVector((GetAbsoluteLookAt() - GetAbsolutePosition()).Normalize());

		// X axis camera
		x = GetUpVector().CrossProduct(GetForwardVector()).Normalize();

		// World space calculation

		// Find the center of the planes
		nearCenter = GetAbsolutePosition() + GetForwardVector() * GetNearDistance();
		farCenter = GetAbsolutePosition() + GetForwardVector() * GetFarDistance();

		// Find the absolute points of the near plane
		SetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_LEFT, nearCenter + GetUpVector() * nearSize.y - x * nearSize.x);
		SetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_RIGHT, nearCenter + GetUpVector() * nearSize.y + x * nearSize.x);
		SetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_LEFT, nearCenter - GetUpVector() * nearSize.y - x * nearSize.x);
		SetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_RIGHT, nearCenter - GetUpVector() * nearSize.y + x * nearSize.x);

		// Find the absolute points of the far plane
		SetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_LEFT, farCenter + GetUpVector() * farSize.y - x * farSize.x);
		SetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_RIGHT, farCenter + GetUpVector() * farSize.y + x * farSize.x);
		SetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_LEFT, farCenter - GetUpVector() * farSize.y - x * farSize.x);
		SetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_RIGHT, farCenter - GetUpVector() * farSize.y + x * farSize.x);


		// Local space calculation

		// Find the center of the planes
		nearCenter = GetForwardVector() * GetNearDistance();
		farCenter = GetForwardVector() * GetFarDistance();

		// Find the for points of the near plane
		SetViewFrustumRelativePoint(FRUSTUM_POINT_NEAR_TOP_LEFT, nearCenter + GetUpVector() * nearSize.y - x * nearSize.x);
		SetViewFrustumRelativePoint(FRUSTUM_POINT_NEAR_TOP_RIGHT, nearCenter + GetUpVector() * nearSize.y + x * nearSize.x);
		SetViewFrustumRelativePoint(FRUSTUM_POINT_NEAR_BOTTOM_LEFT, nearCenter - GetUpVector() * nearSize.y - x * nearSize.x);
		SetViewFrustumRelativePoint(FRUSTUM_POINT_NEAR_BOTTOM_RIGHT, nearCenter - GetUpVector() * nearSize.y + x * nearSize.x);

		// Find the four points of the far plane
		SetViewFrustumRelativePoint(FRUSTUM_POINT_FAR_TOP_LEFT, farCenter + GetUpVector() * farSize.y - x * farSize.x);
		SetViewFrustumRelativePoint(FRUSTUM_POINT_FAR_TOP_RIGHT, farCenter + GetUpVector() * farSize.y + x * farSize.x);
		SetViewFrustumRelativePoint(FRUSTUM_POINT_FAR_BOTTOM_LEFT, farCenter - GetUpVector() * farSize.y - x * farSize.x);
		SetViewFrustumRelativePoint(FRUSTUM_POINT_FAR_BOTTOM_RIGHT, farCenter - GetUpVector() * farSize.y + x * farSize.x);


		// Table creation four the frustum planes
		CVector3F leftFrustrum[3] = { GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_LEFT) };
		CVector3F rightFrustrum[3] = { GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_RIGHT) };
		CVector3F topFrustrum[3] = { GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_RIGHT) };
		CVector3F bottomFrustrum[3] = { GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_LEFT) };
		CVector3F nearFrustrum[3] = { GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_LEFT) };
		CVector3F farFrustrum[3] = { GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_RIGHT) };

		// Update the frustum planes
		SetViewFrustumPlane(FRUSTUM_PLANE_LEFT, leftFrustrum);
		SetViewFrustumPlane(FRUSTUM_PLANE_RIGHT, rightFrustrum);
		SetViewFrustumPlane(FRUSTUM_PLANE_TOP, topFrustrum);
		SetViewFrustumPlane(FRUSTUM_PLANE_BOTTOM, bottomFrustrum);
		SetViewFrustumPlane(FRUSTUM_PLANE_NEAR, nearFrustrum);
		SetViewFrustumPlane(FRUSTUM_PLANE_FAR, farFrustrum);

		// Update the debug buffer
		UpdateDebugBuffer();
	}

	const float CProjectionCameraSceneNode::GetAngle() const {
		LOG_TRACE();

		return this->m_angle;
	}

	const float CProjectionCameraSceneNode::GetRatio() const {
		LOG_TRACE();

		return this->m_ratio;
	}
}
