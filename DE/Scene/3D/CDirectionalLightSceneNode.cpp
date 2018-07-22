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
#include "../../Scene/3D/CSphere.hpp"
#include "../../Scene/3D/I3DScene.hpp"
#include "../../Scene/3D/CDirectionalLightSceneNode.hpp"
#include "../../Scene/3D/CAABoundingBox.hpp"

#define DISTANCE_FACTOR 2.0f

namespace daidalosengine {
	CDirectionalLightSceneNode::CDirectionalLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & rotation) : ILightSceneNode(scene, parent, name, CVector3F(0, 0, 0), rotation, CColor(1.0F, 1.0F, 1.0F), false, true) {
		LOG_TRACE();

		SetType(SCENE_NODE_DIRECTIONALLIGHT);

		// Camera for the shadow rendering
		this->m_camera = new COrthographicCameraSceneNode(scene, this, "directionallight_camera", CVector3F(), CVector3F(), CRectangleF(), 0.1F, 100);
		this->m_camera->SetSaved(false);
		AddChild(this->m_camera);
	}

	CDirectionalLightSceneNode::CDirectionalLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & rotation, const CColor & color, const bool shadowCaster, const bool enabled) : ILightSceneNode(scene, parent, name, CVector3F(0, 0, 0), rotation, color, shadowCaster, enabled) {
		LOG_TRACE();

		SetType(SCENE_NODE_DIRECTIONALLIGHT);

		// Camera for the shadow rendering
		this->m_camera = new COrthographicCameraSceneNode(scene, this, "directionallight_camera", CVector3F(), CVector3F(), CRectangleF(), 0.1F, 100);
		this->m_camera->SetSaved(false);
		AddChild(this->m_camera);
	}

	CDirectionalLightSceneNode::~CDirectionalLightSceneNode() {
		LOG_TRACE();
	}

	void CDirectionalLightSceneNode::Update(const float time) {
		LOG_TRACE();

		// Get the light direction
		const auto direction = GetAbsoluteRotationMatrix().Transform(CVector3F(0, 0, 1)).Normalize();

		CAABoundingBoxF boundingBox;
		if(GetScene()->GetActiveCamera() != this->m_camera) {
			// If the active camera is not the light camera, use the active camera frustum as a bounding box
			auto boundingBoxMin = CVector3F(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
			auto boundingBoxMax = CVector3F(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

			// Find the min and max
			for(auto i = 0; i < FRUSTUM_POINT_SIZE; ++i) {
				boundingBoxMin = boundingBoxMin.Min(GetScene()->GetActiveCamera()->GetViewFrustumAbsolutePoint((EFrustumPoint)i));
				boundingBoxMax = boundingBoxMax.Max(GetScene()->GetActiveCamera()->GetViewFrustumAbsolutePoint((EFrustumPoint)i));
			}

			// Set the bounding box
			boundingBox.SetMinMax(boundingBoxMin, boundingBoxMax);
		} else {
			// If the active camera is the light camera, use the world bounding box
			boundingBox = GetScene()->GetSceneBoundingBox();
		}

		// Find the centroid
		const auto centroid = boundingBox.GetPosition();

		// Find the distance from the centroid to one corner
		const auto distance = (boundingBox.GetMin()).Distance(centroid);

		// Find the light position based on the centroid
		SetPosition(centroid);

		// Update the absolute position and the camera view matrix
		this->m_camera->SetNearDistance(0);
		this->m_camera->SetFarDistance(distance + 5);
		ILightSceneNode::Update(time);

		// Transform the bounding box to the light view space
		const auto minPosition = this->m_camera->GetViewMatrix().Transform(boundingBox.GetMin()).Min(this->m_camera->GetViewMatrix().Transform(boundingBox.GetMax()));
		const auto maxPosition = this->m_camera->GetViewMatrix().Transform(boundingBox.GetMin()).Max(this->m_camera->GetViewMatrix().Transform(boundingBox.GetMax()));


		// Update the camera rect/near/far values
		this->m_camera->SetBounds(CRectangleF(minPosition.x - 10, minPosition.y - 10, maxPosition.x - minPosition.x + 20, maxPosition.y - minPosition.y + 20));
		//this->m_camera->SetNearDistance(0);
		//this->m_camera->SetFarDistance(distance);
		this->m_camera->UpdateProjection();

		/*const float boundValue = std::max(boundingBox.GetMax().x - boundingBox.GetMin().x, boundingBox.GetMax().y - boundingBox.GetMin().y);
		this->m_camera->SetBounds(CRectangleF(0, 0, boundValue, boundValue));
		this->m_camera->SetNearDistance(minPosition.z);
		this->m_camera->SetFarDistance(maxPosition.z + minPosition.z);
		this->m_camera->UpdateProjection();*/


		// If active and enabled, add it to the active lights list
		if(IsActive() && IsEnabled()) {
			SDirectionalLightsShaderData * lights = GetScene()->GetActiveDirectionalLightsBuffer();

			// Add the light if the limit has not been reached
			if(lights->numberOfLights < MAX_LIGHTS_IN_SHADERS) {
				lights->direction[lights->numberOfLights] = CVector4F(direction, 0);
				lights->position[lights->numberOfLights] = CVector4F(GetAbsolutePosition(), 0);
				lights->color[lights->numberOfLights] = GetColor().ToRGBAVector();
				lights->enabled[lights->numberOfLights] = 1;
				lights->shadowCaster[lights->numberOfLights] = IsShadowCaster() ? 1 : 0;
				lights->node[lights->numberOfLights] = this->m_camera;
				++lights->numberOfLights;
			}
		}
	}

	ICameraSceneNode * CDirectionalLightSceneNode::GetCameraNode(const unsigned int id) const {
		LOG_TRACE();

		return this->m_camera;
	}
}
