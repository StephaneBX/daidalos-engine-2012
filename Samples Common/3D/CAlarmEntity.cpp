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

#include <Scene/3D/CSpotLightSceneNode.h>
#include "CAlarmEntity.h"

#define ROTATION_SPEED 70.0f

namespace SAMPLES {
	/**
	* @param node The alarm node
	* @param scene3D The scene associated to the node
	*/
	CAlarmEntity::CAlarmEntity(DE::CStaticMeshSceneNode * meshNode, DE::C3DScene * scene3D) {

		this->m_type = ENTITY_TYPE_ALARM;
		this->m_node = meshNode;
		this->m_scene3D = scene3D;
		this->m_speed = ROTATION_SPEED;

		// Read the attributes
		if(meshNode->GetAttribute("rotation_speed") != "") {
			std::istringstream ss(meshNode->GetAttribute("rotation_speed"));
			ss >> this->m_speed;
		}

		// Add the lights
		DE::CSpotLightSceneNode * spot1 = new DE::CSpotLightSceneNode(this->m_scene3D->GetScene(), this->m_node, "", DE::CVector3F(0, -2.0f, 0), DE::CVector3F(25,0,0), DE::CColor(0.5f,0.0f,0.0f), 80, 20, 0.7f,0.8f, true, true);
		meshNode->AddChild(spot1);
		DE::CSpotLightSceneNode * spot2 = new DE::CSpotLightSceneNode(this->m_scene3D->GetScene(), this->m_node, "", DE::CVector3F(0, -2.0f, 0), DE::CVector3F(25, 180, 0), DE::CColor(0.5f, 0.0f, 0.0f), 80, 20, 0.7f, 0.8f, true, true);
		meshNode->AddChild(spot2);
	}

	/**
	* Update the door
	* @param time Elapsed time
	*/
	void CAlarmEntity::Update(float const time) {
		const float timePerSecond = time / 1000.0f;

		float rotation = this->m_node->GetRotation().y + this->m_speed * timePerSecond;
		if(rotation > 360.0f) {
			rotation -= 360.0f;
		}
		if(rotation < 0) {
			rotation += 360.0f;
		}
		this->m_node->SetRotation(DE::CVector3F(0, rotation, 0));
	}

	/**
	* Trigger the entity action
	*/
	void CAlarmEntity::DoAction() {}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	const bool CAlarmEntity::CanDoAction() const {
		return false;
	}
}
