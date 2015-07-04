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

#include "../../Core/IRenderer.h"
#include "../../Core/Logger/ILogger.h"
#include "../../Scene/3D/I3DScene.h"
#include "../../Scene/3D/COrthographicCameraSceneNode.h"

namespace DE {
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
		ICameraSceneNode::Update();

		// Update the projection
		UpdateProjection();
	}

	/**
	* Update The projection by giving custom bounds and distances
	*/
	void COrthographicCameraSceneNode::UpdateProjection() {
		LOG_TRACE();

		// Update the orthographic information
		this->m_projMatrix.OrthographicView(this->m_bounds.GetWidth(), this->m_bounds.GetHeight(), this->m_nearDistance, this->m_farDistance);
		//this->m_projMatrix.OrthographicView(this->m_bounds.GetLeft(), this->m_bounds.GetRight(), this->m_bounds.GetBottom(), this->m_bounds.GetTop(), this->m_nearDistance, this->m_farDistance);

		// Update the view proj matrix
		this->m_viewProjMatrix = this->m_viewMatrix * this->m_projMatrix;

		/**
		* Update the view frustum information
		*/

		// Z axis camera
		this->m_frustum.forward = (this->m_absoluteLookAt - this->m_absolutePosition).Normalize();

		// Find the absolute points
		CMatrix viewProjMatrixInv = this->m_viewProjMatrix.Inverse();
		CVector4F frustumCorner[FRUSTUM_POINT_SIZE];

		// Screen space coordinates
		frustumCorner[FRUSTUM_POINT_NEAR_TOP_LEFT] = CVector4F(-1.0f, 1.0f, 0.0f, 1.0f);
		frustumCorner[FRUSTUM_POINT_NEAR_TOP_RIGHT] = CVector4F(1.0f, 1.0f, 0.0f, 1.0f);
		frustumCorner[FRUSTUM_POINT_NEAR_BOTTOM_LEFT] = CVector4F(-1.0f, -1.0f, 0.0f, 1.0f);
		frustumCorner[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT] = CVector4F(1.0f, -1.0f, 0.0f, 1.0f);
		frustumCorner[FRUSTUM_POINT_FAR_TOP_LEFT] = CVector4F(-1.0f, 1.0f, 1.0f, 1.0f);
		frustumCorner[FRUSTUM_POINT_FAR_TOP_RIGHT] = CVector4F(1.0f, 1.0f, 1.0f, 1.0f);
		frustumCorner[FRUSTUM_POINT_FAR_BOTTOM_LEFT] = CVector4F(-1.0f, -1.0f, 1.0f, 1.0f);
		frustumCorner[FRUSTUM_POINT_FAR_BOTTOM_RIGHT] = CVector4F(1.0f, -1.0f, 1.0f, 1.0f);

		// Screen space to world space
		for(int i = 0; i < FRUSTUM_POINT_SIZE; ++i) {
			frustumCorner[i] = viewProjMatrixInv.Transform(frustumCorner[i]);
			frustumCorner[i].x /= frustumCorner[i].w;
			frustumCorner[i].y /= frustumCorner[i].w;
			frustumCorner[i].z /= frustumCorner[i].w;

			this->m_frustum.absolutePoints[i] = CVector3F(frustumCorner[i].x, frustumCorner[i].y, frustumCorner[i].z) + this->m_absolutePosition;
		}

		// Find the relative points
		for(int i = 0; i < FRUSTUM_POINT_SIZE; ++i) {
			this->m_frustum.relativePoints[i] = this->m_frustum.absolutePoints[i] - this->m_absolutePosition;
		}

		// Table creation for the frustum planes
		CVector3F leftFrustrum[3] = { this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_LEFT] };
		CVector3F rightFrustrum[3] = { this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT] };
		CVector3F topFrustrum[3] = { this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_RIGHT] };
		CVector3F bottomFrustrum[3] = { this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT] };
		CVector3F nearFrustrum[3] = { this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT] };
		CVector3F farFrustrum[3] = { this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT] };

		// Update the frustum planes
		this->m_frustum.planes[FRUSTUM_PLANE_LEFT].Build(leftFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_RIGHT].Build(rightFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_TOP].Build(topFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_BOTTOM].Build(bottomFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_NEAR].Build(nearFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_FAR].Build(farFrustrum);

		// Update the debug buffer

		// Check the vertex buffer size
		if((int)this->m_debugBuffer.GetSize() < DEBUG_LINES_STRIDE * DEBUG_LINES_COUNT * 2) {
			// Create a new one if the current one can't hold the data
			this->m_debugBuffer = IRenderer::GetRenderer()->CreateVertexBuffer(DEBUG_LINES_STRIDE, DEBUG_LINES_COUNT * 2, BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);
		}

		CVector3F pointsData[DEBUG_LINES_COUNT * 2] = {
			this->m_absolutePosition, this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT],
			this->m_absolutePosition, this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT],
			this->m_absolutePosition, this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT],
			this->m_absolutePosition, this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_LEFT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_RIGHT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_RIGHT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT],
			this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_LEFT]
		};

		unsigned char * lock = (unsigned char*)this->m_debugBuffer.Lock(DEBUG_LINES_STRIDE, 0, 0, LOCK_WRITEONLY);
		memcpy(lock, pointsData, sizeof(pointsData));
		this->m_debugBuffer.Unlock();
	}

	const bool COrthographicCameraSceneNode::FrustumCull(IShape3DF * shape) const {
		LOG_TRACE();

		return false;
	}

	const CRectangleF & COrthographicCameraSceneNode::GetBounds() const {
		LOG_TRACE();

		return this->m_bounds;
	}
}
