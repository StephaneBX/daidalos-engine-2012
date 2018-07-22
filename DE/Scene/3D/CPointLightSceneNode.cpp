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
#include "../../Scene/3D/I3DScene.hpp"
#include "../../Scene/3D/CPointLightSceneNode.hpp"

namespace daidalosengine {
	CPointLightSceneNode::CPointLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position) : IBoundedLightSceneNode(scene, parent, name, position, CVector3F(0, 0, 0), CColor(1.0F, 1.0F, 1.0F), false, 100, 0.8F, true) {
		LOG_TRACE();

		SetType(SCENE_NODE_POINTLIGHT);
	}

	CPointLightSceneNode::CPointLightSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CColor & color, const float range, const float falloff, const bool shadowCaster, const bool enabled) : IBoundedLightSceneNode(scene, parent, name, position, CVector3F(0, 0, 0), color, shadowCaster, range, falloff, enabled) {
		LOG_TRACE();

		SetType(SCENE_NODE_POINTLIGHT);
	}

	CPointLightSceneNode::~CPointLightSceneNode() {
		LOG_TRACE();
	}

	void CPointLightSceneNode::Update(const float time) {
		LOG_TRACE();


		// Update the data
		IBoundedLightSceneNode::Update(time);

		// If active and enabled, add it to the active lights list
		if(IsActive() && IsEnabled()) {
			auto lights = GetScene()->GetActivePointLightsBuffer();

			// Add the light if the limit has not been reached
			if(lights->numberOfLights < MAX_LIGHTS_IN_SHADERS) {
				lights->positionRange[lights->numberOfLights] = CVector4F(GetAbsolutePosition(), GetRange());
				lights->colorFalloff[lights->numberOfLights] = CVector4F(GetColor().ToRGBVector(), GetFalloff());
				lights->enabled[lights->numberOfLights] = 1;
				lights->shadowCaster[lights->numberOfLights] = IsShadowCaster() ? 1 : 0;
				lights->scissor[lights->numberOfLights] = GetScissor();
				++lights->numberOfLights;
			}
		}
	}

	ICameraSceneNode * CPointLightSceneNode::GetCameraNode(const unsigned int id) const {
		LOG_TRACE();

		return nullptr;
	}
}
