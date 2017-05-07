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
#include <Level/ILevel.hpp>
#include <Enums.hpp>
#include "CSpotEntity.hpp"

namespace DAIDALOS {
	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The alarm node
	*/
	CSpotEntity::CSpotEntity(GE::ILevel * parent, DE::CStaticMeshSceneNode * meshNode) : IEntity(parent) {

		this->m_id = meshNode->GetId();
		this->m_type = GE::ENTITY_TYPE_SPOT;
		this->m_node = meshNode;

		// Default values
		bool enabled = true;
		bool shadowCaster = false;
		float lightDecalY = 0;
		float lightRange = 15.0f;
		float lightRadius = 20.0f;
		float lightFallOf = 0.5f;
		float lightTightness = 0.5f;
		DE::CColor lightColor(1.0f, 1.0f, 1.0f, 1.0f);

		// Read the attributes
		if(meshNode->GetAttribute("lightDecalY") != "") {
			std::istringstream ss(meshNode->GetAttribute("lightDecalY"));
			ss >> lightDecalY;
		}
		if(meshNode->GetAttribute("lightEnabled") != "") {
			enabled = meshNode->GetAttribute("lightEnabled") == "true" ? true : false;
		}
		if(meshNode->GetAttribute("lightShadow_caster") != "") {
			shadowCaster = meshNode->GetAttribute("lightShadow_caster") == "true" ? true : false;
		}
		if(meshNode->GetAttribute("lightColor") != "") {
			lightColor = DE::CColor::GetFrom(meshNode->GetAttribute("lightColor"));
		}
		if(meshNode->GetAttribute("lightRange") != "") {
			std::istringstream ss(meshNode->GetAttribute("lightRange"));
			ss >> lightRange;
		}
		if(meshNode->GetAttribute("lightRadius") != "") {
			std::istringstream ss(meshNode->GetAttribute("lightRadius"));
			ss >> lightRadius;
		}
		if(meshNode->GetAttribute("lightFalloff") != "") {
			std::istringstream ss(meshNode->GetAttribute("lightFalloff"));
			ss >> lightFallOf;
		}
		if(meshNode->GetAttribute("lightTightness") != "") {
			std::istringstream ss(meshNode->GetAttribute("lightTightness"));
			ss >> lightTightness;
		}

		// Add the light
		DE::CSpotLightSceneNode * spot = new DE::CSpotLightSceneNode(this->m_parent->GetScene()->GetScene(), this->m_node, "", DE::CVector3F(0, lightDecalY, 0), DE::CVector3F(90, 0, 0), lightColor, lightRange, lightRadius, lightFallOf, lightTightness, shadowCaster, enabled);
		meshNode->AddChild(spot);
	}

	/**
	* Update the door
	* @param time Elapsed time
	*/
	void CSpotEntity::Update(const float time) {}

	/**
	* Trigger the entity action
	*/
	void CSpotEntity::DoAction() {}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	const bool CSpotEntity::CanDoAction() const {
		return false;
	}
}
