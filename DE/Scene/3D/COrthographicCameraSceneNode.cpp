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

#include "../../Core/IRenderer.hpp"
#include "../../Core/Logger/ILogger.hpp"
#include "../../Scene/3D/I3DScene.hpp"
#include "../../Scene/3D/COrthographicCameraSceneNode.hpp"

namespace daidalosengine {
	COrthographicCameraSceneNode::COrthographicCameraSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CRectangleF & bounds, const float nearDistance, const float farDistance) : ICameraSceneNode(scene, parent, name, position, rotation, nearDistance, farDistance) {
		LOG_TRACE();

		SetBounds(bounds);
	}

	COrthographicCameraSceneNode::~COrthographicCameraSceneNode() {
		LOG_TRACE();
	}

	void COrthographicCameraSceneNode::SetBounds(const CRectangleF & bounds) {
		LOG_TRACE();

		this->m_bounds = bounds;
	}

	void COrthographicCameraSceneNode::Update(const float time) {
		LOG_TRACE();

		// Update the camera information
		ICameraSceneNode::Update(time);

		// Update the projection
		UpdateProjection();
	}

	/**
	* Update The projection by giving custom bounds and distances
	*/
	void COrthographicCameraSceneNode::UpdateProjection() {
		LOG_TRACE();

		// Update the orthographic information
		CMatrix projMatrix;
		projMatrix.OrthographicView(this->m_bounds.GetWidth(), this->m_bounds.GetHeight(), GetNearDistance(), GetFarDistance());
		SetProjMatrix(projMatrix);

		// Update the view proj matrix
		SetViewProjMatrix(GetViewMatrix() * GetProjMatrix());

		/**
		* Update the view frustum information
		*/

		// Z axis camera
		SetForwardVector((GetAbsoluteLookAt() - GetAbsolutePosition()).Normalize());

		// Find the absolute points
		CMatrix viewProjMatrixInv = GetViewProjMatrix().Inverse();
		CVector4F frustumCorner[FRUSTUM_POINT_SIZE];

		// Screen space coordinates
		frustumCorner[FRUSTUM_POINT_NEAR_TOP_LEFT] = CVector4F(-1.0F, 1.0F, 0.0F, 1.0F);
		frustumCorner[FRUSTUM_POINT_NEAR_TOP_RIGHT] = CVector4F(1.0F, 1.0F, 0.0F, 1.0F);
		frustumCorner[FRUSTUM_POINT_NEAR_BOTTOM_LEFT] = CVector4F(-1.0F, -1.0F, 0.0F, 1.0F);
		frustumCorner[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT] = CVector4F(1.0F, -1.0F, 0.0F, 1.0F);
		frustumCorner[FRUSTUM_POINT_FAR_TOP_LEFT] = CVector4F(-1.0F, 1.0F, 1.0F, 1.0F);
		frustumCorner[FRUSTUM_POINT_FAR_TOP_RIGHT] = CVector4F(1.0F, 1.0F, 1.0F, 1.0F);
		frustumCorner[FRUSTUM_POINT_FAR_BOTTOM_LEFT] = CVector4F(-1.0F, -1.0F, 1.0F, 1.0F);
		frustumCorner[FRUSTUM_POINT_FAR_BOTTOM_RIGHT] = CVector4F(1.0F, -1.0F, 1.0F, 1.0F);

		// Screen space to world space
		for(int i = 0; i < FRUSTUM_POINT_SIZE; ++i) {
			frustumCorner[i] = viewProjMatrixInv.Transform(frustumCorner[i]);
			frustumCorner[i].x /= frustumCorner[i].w;
			frustumCorner[i].y /= frustumCorner[i].w;
			frustumCorner[i].z /= frustumCorner[i].w;

			SetViewFrustumAbsolutePoint((EFrustumPoint) i, CVector3F(frustumCorner[i].x, frustumCorner[i].y, frustumCorner[i].z) + GetAbsolutePosition());
		}

		// Find the relative points
		for(int i = 0; i < FRUSTUM_POINT_SIZE; ++i) {
			SetViewFrustumRelativePoint((EFrustumPoint)i, GetViewFrustumAbsolutePoint((EFrustumPoint)i) - GetAbsolutePosition());
		}

		// Table creation for the frustum planes
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

	bool COrthographicCameraSceneNode::FrustumCull(IShape3DF * shape) const {
		LOG_TRACE();

		return false;
	}

	const CRectangleF & COrthographicCameraSceneNode::GetBounds() const {
		LOG_TRACE();

		return this->m_bounds;
	}
}
