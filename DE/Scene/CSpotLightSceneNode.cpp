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

#include "../Scene/CSphere.h"
#include "../Scene/CSceneManager.h"
#include "../Scene/CSpotLightSceneNode.h"

namespace DE {
	CSpotLightSceneNode::CSpotLightSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation) : IBoundedLightSceneNode(parent, name, position, rotation, CColor(1.0f, 1.0f, 1.0f), false, 100, 0.8f, true) {
		this->m_radius = DegreeToRadian(30);
		this->m_tightness = 0.7f;
		this->m_type = SCENE_NODE_SPOTLIGHT;

		// Camera for the shadow rendering
		this->m_camera = new CProjectionCameraSceneNode(this, "", CVector3F(), CVector3F(), this->m_radius * 2, 1, 0.1f, this->m_range);
		AddChild(this->m_camera);
	}

	CSpotLightSceneNode::CSpotLightSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CColor & color, const float range, const float radius, const float falloff, const float tightness, const bool shadowCaster, const bool enabled) : IBoundedLightSceneNode(parent, name, position, rotation, color, shadowCaster, range, falloff, enabled) {
		this->m_radius = DegreeToRadian(radius);
		this->m_tightness = tightness;
		this->m_type = SCENE_NODE_SPOTLIGHT;

		// Camera for the shadow rendering
		this->m_camera = new CProjectionCameraSceneNode(this, "", CVector3F(), CVector3F(), DegreeToRadian(radius) * 2, 1, 0.1f, this->m_range);
		AddChild(this->m_camera);
	}

	CSpotLightSceneNode::~CSpotLightSceneNode() {}

	void CSpotLightSceneNode::SetRadius(const float radius) {
		this->m_radius = DegreeToRadian(radius);
		this->m_camera->SetAngle(DegreeToRadian(radius));
	}

	void CSpotLightSceneNode::SetTightness(const float tightness) {
		this->m_tightness = tightness;
	}

	void CSpotLightSceneNode::Update(const float time) {
		CSceneManager * scene = CSceneManager::GetInstance();

		// Update the absolute position
		IBoundedLightSceneNode::Update(time);

		// If active and enabled, add it to the active lights list
		if(this->m_active && this->m_enabled) {
			SSPotLightsShaderData * lights = scene->GetActiveSpotLightsBuffer();

			// Check if the light hasn't already been added
			bool lightFound = false;
			for(int i = 0; i < lights->numberOfLights && !lightFound; i++) {
				lightFound = lights->node[i] == this->m_camera;
			}

			// Add the light if the limit has not been reached and the light is not already present
			if(lights->numberOfLights < MAX_LIGHTS_IN_SHADERS && !lightFound) {
				lights->position[lights->numberOfLights] = CVector4F(this->m_absolutePosition, 0);
				lights->direction[lights->numberOfLights] = CVector4F(this->m_camera->GetForwardVector(), 0);
				lights->color[lights->numberOfLights] = this->m_color.ToRGBAVector();
				lights->rangeRadiusFalloffTightness[lights->numberOfLights] = CVector4F(this->m_visibilityShape->GetInnerDepth(), RadianToDegree(this->m_radius), this->m_falloff, this->m_tightness);
				lights->enabled[lights->numberOfLights] = 1;
				lights->shadowCaster[lights->numberOfLights] = this->m_shadowCaster ? 1 : 0;
				lights->scissor[lights->numberOfLights] = this->m_scissor;
				lights->node[lights->numberOfLights] = this->m_camera;
				++lights->numberOfLights;
			}
		}
	}

	const float CSpotLightSceneNode::GetRadius() const {
		return this->m_radius;
	}

	const float CSpotLightSceneNode::GetTightness() const {
		return this->m_tightness;
	}

	ICameraSceneNode * CSpotLightSceneNode::GetCameraNode(const unsigned int id) const {
		return this->m_camera;
	}
}
