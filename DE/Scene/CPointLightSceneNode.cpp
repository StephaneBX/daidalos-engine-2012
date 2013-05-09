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

		// Cameras for the shadow rendering
		AddCameras();
	}

	CPointLightSceneNode::CPointLightSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position, const CColor & color, const float range, const float falloff, const bool shadowCaster, const bool enabled) : IBoundedLightSceneNode(parent, name, position, CVector3F(0,0,0), color, shadowCaster, range, falloff, enabled)
	{
		this->m_type = SCENE_NODE_POINTLIGHT;

		// Cameras for the shadow rendering
		AddCameras();
	}

	CPointLightSceneNode::~CPointLightSceneNode() {}

	void CPointLightSceneNode::Update(const float time)
	{
		CSceneManager * scene = CSceneManager::GetInstance();

		// Update the data
		IBoundedLightSceneNode::Update(time);

		// If active, add it to the active lights list
		if(this->m_active)
		{
			SSPotLightsShaderData * lights = scene->GetActiveSpotLightsBuffer();

			// Add the light if the limit has not been reached
			if(lights->numberOfLights + 6 <= MAX_LIGHTS_IN_SHADERS)
			{
				for(unsigned int i = 0 ; i < this->m_cameras.size() ; i++)
				{
					lights->position[lights->numberOfLights] = CVector4F(this->m_absolutePosition,0);
					lights->direction[lights->numberOfLights] = CVector4F(this->m_cameras[i]->GetForwardVector(), 0);
					lights->color[lights->numberOfLights] = this->m_color.ToRGBAVector();
					lights->rangeRadiusFalloffTightness[lights->numberOfLights] = CVector4F(this->m_range, 90, this->m_falloff, 1);
					lights->enabled[lights->numberOfLights] = 1;
					lights->shadowCaster[lights->numberOfLights] = this->m_shadowCaster ? 1 : 0;
					lights->scissor[lights->numberOfLights] = this->m_scissor;
					lights->node[lights->numberOfLights] = this->m_cameras[i];
					++lights->numberOfLights;
				}	
			}
		}
	}

	void CPointLightSceneNode::AddCameras() 
	{
		CProjectionCameraSceneNode * cameraLeft = new  CProjectionCameraSceneNode(this, "", CVector3F(), CVector3F(0, -90, 0), DegreeToRadian(90), 1, 0.1f, this->m_range);
		this->m_cameras[0] = cameraLeft;
		AddChild(cameraLeft);

		CProjectionCameraSceneNode * cameraForward = new  CProjectionCameraSceneNode(this, "", CVector3F(), CVector3F(), DegreeToRadian(90), 1, 0.1f, this->m_range);
		this->m_cameras[1] = cameraForward;
		AddChild(cameraForward);

		CProjectionCameraSceneNode * cameraRight = new  CProjectionCameraSceneNode(this, "", CVector3F(), CVector3F(0, 90, 0), DegreeToRadian(90), 1, 0.1f, this->m_range);
		this->m_cameras[2] = cameraRight;
		AddChild(cameraRight);

		CProjectionCameraSceneNode * cameraBackward = new  CProjectionCameraSceneNode(this, "", CVector3F(), CVector3F(0, 180, 0), DegreeToRadian(90), 1, 0.1f, this->m_range);
		this->m_cameras[3] = cameraBackward;
		AddChild(cameraBackward);

		CProjectionCameraSceneNode * cameraUp = new  CProjectionCameraSceneNode(this, "", CVector3F(), CVector3F(-90, 0, 0), DegreeToRadian(90), 1, 0.1f, this->m_range);
		this->m_cameras[4] = cameraUp;
		AddChild(cameraUp);

		CProjectionCameraSceneNode * cameraDown = new  CProjectionCameraSceneNode(this, "", CVector3F(), CVector3F(90, 0, 0), DegreeToRadian(90), 1, 0.1f, this->m_range);
		this->m_cameras[5] = cameraDown;
		AddChild(cameraDown);
	}
}
