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
#include "../../Scene/3D/CSpotLightSceneNode.hpp"

namespace daidalosengine {
	CSpotLightSceneNode::CSpotLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation) : IBoundedLightSceneNode(scene, parent, name, position, rotation, CColor(1.0F, 1.0F, 1.0F), false, 100, 0.8F, true) {
		LOG_TRACE();

		this->m_radius = DegreeToRadian(30);
		this->m_tightness = 0.7F;
		SetType(SCENE_NODE_SPOTLIGHT);

		// Camera for the shadow rendering
		this->m_camera = new CProjectionCameraSceneNode(scene, this, "spotlight_camera", CVector3F(), CVector3F(), this->m_radius * 2, 1, 0.1F, GetRange());
		this->m_camera->SetSaved(false);
		AddChild(this->m_camera);
	}

	CSpotLightSceneNode::CSpotLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CColor & color, const float range, const float radius, const float falloff, const float tightness, const bool shadowCaster, const bool enabled) : IBoundedLightSceneNode(scene, parent, name, position, rotation, color, shadowCaster, range, falloff, enabled) {
		LOG_TRACE();

		this->m_radius = DegreeToRadian(radius);
		this->m_tightness = tightness;
		SetType(SCENE_NODE_SPOTLIGHT);

		// Camera for the shadow rendering
		this->m_camera = new CProjectionCameraSceneNode(scene, this, "spotlight_camera", CVector3F(), CVector3F(), DegreeToRadian(radius) * 2, 1, 0.1F, GetRange());
		this->m_camera->SetSaved(false);
		AddChild(this->m_camera);
	}

	CSpotLightSceneNode::~CSpotLightSceneNode() = default;

	void CSpotLightSceneNode::SetRadius(const float radius) {
		LOG_TRACE();

		this->m_radius = DegreeToRadian(radius);
		this->m_camera->SetAngle(DegreeToRadian(radius));
	}

	void CSpotLightSceneNode::SetTightness(const float tightness) {
		LOG_TRACE();

		this->m_tightness = tightness;
	}

	void CSpotLightSceneNode::Update(const float time) {
		LOG_TRACE();

		// Update the absolute position
		IBoundedLightSceneNode::Update(time);

		// If active and enabled, add it to the active lights list
		if(IsActive() && IsEnabled()) {
			auto* lights = GetScene()->GetActiveSpotLightsBuffer();

			// Check if the light hasn't already been added
			auto lightFound = false;
			for(auto i = 0; i < lights->numberOfLights && !lightFound; i++) {
				lightFound = lights->node[i] == this->m_camera;
			}

			// Add the light if the limit has not been reached and the light is not already present
			if(lights->numberOfLights < MAX_LIGHTS_IN_SHADERS && !lightFound) {
				lights->position[lights->numberOfLights] = CVector4F(GetAbsolutePosition(), 0);
				lights->direction[lights->numberOfLights] = CVector4F(this->m_camera->GetForwardVector(), 0);
				lights->color[lights->numberOfLights] = GetColor().ToRGBAVector();
				lights->rangeRadiusFalloffTightness[lights->numberOfLights] = CVector4F(GetVisibilitySphere()->GetInnerDepth(), RadianToDegree(this->m_radius), GetFalloff(), this->m_tightness);
				lights->enabled[lights->numberOfLights] = 1;
				lights->shadowCaster[lights->numberOfLights] = IsShadowCaster() ? 1 : 0;
				lights->scissor[lights->numberOfLights] = GetScissor();
				lights->node[lights->numberOfLights] = this->m_camera;
				++lights->numberOfLights;
			}
		}
	}

	float CSpotLightSceneNode::GetRadius() const {
		LOG_TRACE();

		return this->m_radius;
	}

	float CSpotLightSceneNode::GetTightness() const {
		LOG_TRACE();

		return this->m_tightness;
	}

	ICameraSceneNode * CSpotLightSceneNode::GetCameraNode(const unsigned int id) const {
		LOG_TRACE();

		return this->m_camera;
	}
}
