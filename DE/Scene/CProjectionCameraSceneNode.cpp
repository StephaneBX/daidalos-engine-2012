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

#include "../Core/IRenderer.h"
#include "../Scene/CProjectionCameraSceneNode.h"

namespace DE
{
	CProjectionCameraSceneNode::CProjectionCameraSceneNode(ISceneNode * parent, const std::string & name,  const CVector3F & position, const CVector3F & rotation, const float angle, const float ratio, const float nearDistance, const float farDistance) : ICameraSceneNode(parent, name, position, rotation, nearDistance, farDistance)
	{
		SetAngle(angle);
		SetRatio(ratio);
	}

	CProjectionCameraSceneNode::~CProjectionCameraSceneNode() {}

	void CProjectionCameraSceneNode::SetAngle(const float angle)
	{
		this->m_angle = angle;
	}

	void CProjectionCameraSceneNode::SetRatio(const float ratio)
	{
		this->m_ratio = ratio;
	}

	void CProjectionCameraSceneNode::Update(const float time)
	{
		// Update the camera information
		ICameraSceneNode::Update();

		// Update the projection information
		this->m_projMatrix.PerspectiveFOV(this->m_angle, this->m_ratio, this->m_nearDistance, this->m_farDistance);

		// Update the view proj matrix
		this->m_viewProjMatrix = this->m_viewMatrix * this->m_projMatrix;

		/**
		* Update the view frustum information
		*/
		float tang;
		CVector3F nearCenter, farCenter;
		CVector2F nearSize, farSize;
		CVector3F x;

		tang = this->m_angle * 0.5f;
		nearSize.y = this->m_nearDistance * tang;
		nearSize.x = nearSize.y * this->m_ratio;
		farSize.y = this->m_farDistance * tang;
		farSize.x = farSize.y * this->m_ratio;

		// Z axis camera
		this->m_frustum.forward = (this->m_absoluteLookAt - this->m_absolutePosition).Normalize();

		// X axis camera
		x = this->m_absoluteUp.CrossProduct(this->m_frustum.forward).Normalize();

		// World space calculation

		// Find the center of the planes
		nearCenter = this->m_absolutePosition + this->m_frustum.forward * this->m_nearDistance;
		farCenter = this->m_absolutePosition + this->m_frustum.forward * this->m_farDistance;

		// Find the absolute points of the near plane
		this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT] = nearCenter + this->m_absoluteUp * nearSize.y - x * nearSize.x;
		this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT] = nearCenter + this->m_absoluteUp * nearSize.y + x * nearSize.x;
		this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT] = nearCenter - this->m_absoluteUp * nearSize.y - x * nearSize.x;
		this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT] = nearCenter - this->m_absoluteUp * nearSize.y + x * nearSize.x;

		// Find the absolute points of the far plane
		this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_LEFT] = farCenter + this->m_absoluteUp * farSize.y - x * farSize.x;
		this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_RIGHT] = farCenter + this->m_absoluteUp * farSize.y + x * farSize.x;
		this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT] = farCenter - this->m_absoluteUp * farSize.y - x * farSize.x;
		this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT] = farCenter - this->m_absoluteUp * farSize.y + x * farSize.x;


		// Local space calculation

		// Find the center of the planes
		nearCenter = this->m_frustum.forward * this->m_nearDistance;
		farCenter = this->m_frustum.forward * this->m_farDistance;

		// Find the for points of the near plane
		this->m_frustum.relativePoints[FRUSTUM_POINT_NEAR_TOP_LEFT] = nearCenter + this->m_absoluteUp * nearSize.y - x * nearSize.x;
		this->m_frustum.relativePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT] = nearCenter + this->m_absoluteUp * nearSize.y + x * nearSize.x;
		this->m_frustum.relativePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT] = nearCenter - this->m_absoluteUp * nearSize.y - x * nearSize.x;
		this->m_frustum.relativePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT] = nearCenter - this->m_absoluteUp * nearSize.y + x * nearSize.x;

		// Find the four points of the far plane
		this->m_frustum.relativePoints[FRUSTUM_POINT_FAR_TOP_LEFT] = farCenter + this->m_absoluteUp * farSize.y - x * farSize.x;
		this->m_frustum.relativePoints[FRUSTUM_POINT_FAR_TOP_RIGHT] = farCenter + this->m_absoluteUp * farSize.y + x * farSize.x;
		this->m_frustum.relativePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT] = farCenter - this->m_absoluteUp * farSize.y - x * farSize.x;
		this->m_frustum.relativePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT] = farCenter - this->m_absoluteUp * farSize.y + x * farSize.x;


		// Table creation four the frustum planes
		CVector3F leftFrustrum[3]   = {this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_LEFT]};
		CVector3F rightFrustrum[3]  = {this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT]};
		CVector3F topFrustrum[3]    = {this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_RIGHT]};
		CVector3F bottomFrustrum[3] = {this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_LEFT]};
		CVector3F nearFrustrum[3]   = {this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_NEAR_BOTTOM_LEFT]};
		CVector3F farFrustrum[3]    = {this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_LEFT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_TOP_RIGHT], this->m_frustum.absolutePoints[FRUSTUM_POINT_FAR_BOTTOM_RIGHT]};

		// Update the frustum planes
		this->m_frustum.planes[FRUSTUM_PLANE_LEFT].Build(leftFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_RIGHT].Build(rightFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_TOP].Build(topFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_BOTTOM].Build(bottomFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_NEAR].Build(nearFrustrum);
		this->m_frustum.planes[FRUSTUM_PLANE_FAR].Build(farFrustrum);


		// Update the debug buffer

		// Check the vertex buffer size
		if((int)this->m_debugBuffer.GetSize() < DEBUG_LINES_STRIDE * DEBUG_LINES_COUNT * 2)
		{
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

	const float CProjectionCameraSceneNode::GetAngle() const
	{
		return this->m_angle;
	}

	const float CProjectionCameraSceneNode::GetRatio() const
	{
		return this->m_ratio;
	}
}
