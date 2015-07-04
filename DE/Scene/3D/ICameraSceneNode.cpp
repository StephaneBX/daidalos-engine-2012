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
#include "../../Core/Memory/CExceptionAssert.h"
#include "../../Scene/3D/ICameraSceneNode.h"

namespace DE {
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
		this->m_localLookAt = this->m_localMatrix.Transform(this->m_localLookAt);
		this->m_localUp = this->m_localRotationMatrix.Transform(CVector3F(0, 1, 0)).Normalize();

		if(this->m_parent != nullptr) {
			this->m_absoluteLookAt = this->m_parent->GetAbsoluteTransformationMatrix().Transform(this->m_localLookAt);
			this->m_absoluteUp = CMatrix().RotationQuaternion(this->m_parent->GetAbsoluteTransformationMatrix().TransformRotation()).Transform(this->m_localUp).Normalize();
		} else {
			this->m_absoluteLookAt = this->m_localLookAt;
			this->m_absoluteUp = this->m_localUp;
		}
		this->m_viewMatrix.LookAt(this->m_absolutePosition, this->m_absoluteLookAt, this->m_absoluteUp);
	}

	void ICameraSceneNode::Render() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();
		renderer->SetVertexBuffer(0, this->m_debugBuffer, DEBUG_LINES_STRIDE);
		renderer->DrawPrimitives(PRIMITIVE_TYPE_LINE_LIST, 0, DEBUG_LINES_COUNT);
	}

	const bool ICameraSceneNode::FrustumCull(IShape3DF * shape) const {
		LOG_TRACE();

		bool result = false;

		for(int i = 0; i < FRUSTUM_PLANE_SIZE && !result; i++) {
			float dist = GetViewFrustumPlane((EFrustumPlane)i).PointDistance(shape->GetPosition());
			if(dist < -shape->GetInnerDepth(GetViewFrustumPlane(FRUSTUM_PLANE_NEAR)))
				result = true;
		}

		return result;
	}

	const float ICameraSceneNode::GetNearDistance() const {
		LOG_TRACE();

		return this->m_nearDistance;
	}

	const float ICameraSceneNode::GetFarDistance() const {

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

	ICameraSceneNode::ICameraSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const float nearDistance, const float farDistance) : ISceneNode(scene, parent, name, position, rotation) {
		LOG_TRACE();

		this->m_type = SCENE_NODE_CAMERA;
		this->m_nearDistance = nearDistance;
		this->m_farDistance = farDistance;
		this->m_localUp = CVector3F(0, 1, 0);
	}
}
