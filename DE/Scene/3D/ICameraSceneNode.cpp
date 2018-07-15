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
#include "../../Core/Memory/CExceptionAssert.hpp"
#include "../../Scene/3D/ICameraSceneNode.hpp"

namespace daidalosengine {
	ICameraSceneNode::~ICameraSceneNode() {
		LOG_TRACE();
	}

	void ICameraSceneNode::SetNearDistance(const float nearDistance) {
		LOG_TRACE();

		this->m_nearDistance = nearDistance;
	}

	void ICameraSceneNode::SetFarDistance(const float farDistance) {
		LOG_TRACE();

		this->m_farDistance = farDistance;
	}

	void ICameraSceneNode::Update(const float time) {
		LOG_TRACE();

		// Update the absolute position
		ISceneNode::Update(time);

		// Update the camera
		this->m_localLookAt = CVector3F(0, 0, this->m_farDistance);
		this->m_localLookAt = GetTransformationMatrix().Transform(this->m_localLookAt);
		this->m_localUp = GetRotationMatrix().Transform(CVector3F(0, 1, 0)).Normalize();

		if(GetParent() != nullptr) {
			this->m_absoluteLookAt = (GetParent()->GetAbsoluteRotationMatrix() * GetParent()->GetAbsoluteTranslationMatrix()).Transform(this->m_localLookAt);
			this->m_absoluteUp = CMatrix().RotationQuaternion(GetParent()->GetAbsoluteTransformationMatrix().TransformRotation()).Transform(this->m_localUp).Normalize();
		} else {
			this->m_absoluteLookAt = this->m_localLookAt;
			this->m_absoluteUp = this->m_localUp;
		}
		this->m_viewMatrix.LookAt(GetAbsolutePosition(), this->m_absoluteLookAt, this->m_absoluteUp);
	}

	void ICameraSceneNode::RenderDebugInformation() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();
		renderer->SetVertexBuffer(0, this->m_debugBuffer.GetBuffer(), DEBUG_LINES_STRIDE);
		renderer->DrawPrimitives(PRIMITIVE_TYPE_LINE_LIST, 0, DEBUG_LINES_COUNT);

		ISceneNode::RenderDebugInformation();
	}

	bool ICameraSceneNode::FrustumCull(IShape3DF * shape) const {
		LOG_TRACE();

		int count = 0;

		for(int i = 0; i < FRUSTUM_PLANE_SIZE && count < 1; i++) {
			float dist = GetViewFrustumPlane((EFrustumPlane)i).PointDistance(shape->GetPosition());
			if(dist < -shape->GetInnerDepth(GetViewFrustumPlane(FRUSTUM_PLANE_NEAR))) {
				count++;
			}
		}

		return count >= 1 && count < FRUSTUM_PLANE_SIZE;
	}

	float ICameraSceneNode::GetNearDistance() const {
		LOG_TRACE();

		return this->m_nearDistance;
	}

	float ICameraSceneNode::GetFarDistance() const {

		LOG_TRACE();
		return this->m_farDistance;
	}

	const CMatrix & ICameraSceneNode::GetViewMatrix() const {
		LOG_TRACE();

		return this->m_viewMatrix;
	}

	const CMatrix & ICameraSceneNode::GetProjMatrix() const {
		LOG_TRACE();

		return this->m_projMatrix;
	}

	const CMatrix & ICameraSceneNode::GetViewProjMatrix() const {
		LOG_TRACE();

		return this->m_viewProjMatrix;
	}

	const CVector3F & ICameraSceneNode::GetViewFrustumRelativePoint(EFrustumPoint point) const {
		LOG_TRACE();

		return this->m_frustum.relativePoints[point];
	}

	const CVector3F & ICameraSceneNode::GetViewFrustumAbsolutePoint(EFrustumPoint point) const {
		LOG_TRACE();

		return this->m_frustum.absolutePoints[point];
	}

	const CPlane3DF & ICameraSceneNode::GetViewFrustumPlane(EFrustumPlane plane) const {
		LOG_TRACE();

		return this->m_frustum.planes[plane];
	}

	const CVector3F & ICameraSceneNode::GetForwardVector() const {
		LOG_TRACE();

		return this->m_frustum.forward;
	}

	const CVector3F & ICameraSceneNode::GetUpVector() const {
		LOG_TRACE();

		return this->m_absoluteUp;
	}

	/**
	* Get the absolute look at position
	* @return The absolute look at position
	*/
	const CVector3F & ICameraSceneNode::GetAbsoluteLookAt() const {
		LOG_TRACE();

		return this->m_absoluteLookAt;
	}

	ICameraSceneNode::ICameraSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const float nearDistance, const float farDistance) : ISceneNode(scene, parent, name, position, rotation) {
		LOG_TRACE();

		SetType(SCENE_NODE_CAMERA);
		this->m_nearDistance = nearDistance;
		this->m_farDistance = farDistance;
		this->m_localUp = CVector3F(0, 1, 0);
	}

	/**
	* Set the projection matrix
	* @param matrix The view projection matrix
	*/
	void ICameraSceneNode::SetProjMatrix(const CMatrix & matrix) {
		LOG_TRACE();

		this->m_projMatrix = matrix;
	}

	/**
	* Set the view projection matrix
	* @param matrix The view projection matrix
	*/
	void ICameraSceneNode::SetViewProjMatrix(const CMatrix & matrix) {
		LOG_TRACE();

		this->m_viewProjMatrix = matrix;
	}

	/**
	* Set a view frustum point
	* @param point The point to get
	* @param value The value
	*/
	void ICameraSceneNode::SetViewFrustumRelativePoint(EFrustumPoint point, const CVector3F & value) {
		LOG_TRACE();

		this->m_frustum.relativePoints[point] = value;
	}

	/**
	* Set a view frustum absolute point
	* @param point The point to get
	* @param value The value
	*/
	void ICameraSceneNode::SetViewFrustumAbsolutePoint(EFrustumPoint point, const CVector3F & value) {
		LOG_TRACE();

		this->m_frustum.absolutePoints[point] = value;
	}

	/**
	* Build the view frustum planes
	* @param plane The plane
	* @param planePoints The plane definition as a list of points
	*/
	void ICameraSceneNode::SetViewFrustumPlane(EFrustumPlane plane, CVector3F * planePoints) {
		LOG_TRACE();

		this->m_frustum.planes[plane].Build(planePoints);
	}

	/**
	* Set the forward vector
	* @param vector The forward vector
	*/
	void ICameraSceneNode::SetForwardVector(const CVector3F & vector) {
		LOG_TRACE();

		this->m_frustum.forward = vector;
	}

	/**
	* Update the debug buffer
	*/
	void ICameraSceneNode::UpdateDebugBuffer() {
		// Check the vertex buffer size
		if ((int)this->m_debugBuffer.GetSize() < DEBUG_LINES_STRIDE * DEBUG_LINES_COUNT * 2) {
			// Create a new one if the current one can't hold the data
			this->m_debugBuffer = IRenderer::GetRenderer()->CreateVertexBuffer(DEBUG_LINES_STRIDE, DEBUG_LINES_COUNT * 2, BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);
		}

		// Set the data
		CVector3F pointsData[DEBUG_LINES_COUNT * 2] = {
			GetAbsolutePosition(), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_LEFT),
			GetAbsolutePosition(), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_RIGHT),
			GetAbsolutePosition(), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_RIGHT),
			GetAbsolutePosition(), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_LEFT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_RIGHT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_RIGHT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_LEFT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_LEFT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_LEFT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_TOP_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_RIGHT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_RIGHT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_NEAR_BOTTOM_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_LEFT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_RIGHT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_RIGHT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_RIGHT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_LEFT),
			GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_BOTTOM_LEFT), GetViewFrustumAbsolutePoint(FRUSTUM_POINT_FAR_TOP_LEFT)
		};

		// Update the buffer
		unsigned char * lock = (unsigned char*)this->m_debugBuffer.Lock(DEBUG_LINES_STRIDE, 0, 0, LOCK_WRITEONLY);
		memcpy(lock, pointsData, sizeof(pointsData));
		this->m_debugBuffer.Unlock();
	}
}
