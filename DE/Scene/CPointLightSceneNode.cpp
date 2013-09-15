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
#include "../Scene/CPointLightSceneNode.h"

namespace DE
{
	CPointLightSceneNode::CPointLightSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position) : IBoundedLightSceneNode(parent, name, position, CVector3F(0,0,0), CColor(1.0f, 1.0f, 1.0f), false, 100, 0.8f, true)
	{
		this->m_type = SCENE_NODE_POINTLIGHT;
	}

	CPointLightSceneNode::CPointLightSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position, const CColor & color, const float range, const float falloff, const bool shadowCaster, const bool enabled) : IBoundedLightSceneNode(parent, name, position, CVector3F(0,0,0), color, shadowCaster, range, falloff, enabled)
	{
		this->m_type = SCENE_NODE_POINTLIGHT;
	}

	CPointLightSceneNode::~CPointLightSceneNode() {}

	void CPointLightSceneNode::Update(const float time)
	{
		CSceneManager * scene = CSceneManager::GetInstance();

		// Update the data
		IBoundedLightSceneNode::Update(time);

		// If active and enabled, add it to the active lights list
		if(this->m_active && this->m_enabled)
		{
			SPointLightsShaderData * lights = scene->GetActivePointLightsBuffer();

			// Add the light if the limit has not been reached
			if(lights->numberOfLights <= MAX_LIGHTS_IN_SHADERS)
			{
				lights->positionRange[lights->numberOfLights] = CVector4F(this->m_absolutePosition, this->m_range);
				lights->colorFalloff[lights->numberOfLights] = CVector4F(this->m_color.ToRGBVector(), this->m_falloff);
				lights->enabled[lights->numberOfLights] = 1;
				lights->shadowCaster[lights->numberOfLights] = this->m_shadowCaster ? 1 : 0;
				lights->scissor[lights->numberOfLights] = this->m_scissor;
				++lights->numberOfLights;	
			}
		}
	}

	ICameraSceneNode * CPointLightSceneNode::GetCameraNode(const unsigned int id) const
	{
		return nullptr;
	}
}
