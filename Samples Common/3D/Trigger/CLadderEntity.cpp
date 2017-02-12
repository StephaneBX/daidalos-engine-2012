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

#include "../../Enums.h"
#include "../../Level/ILevel.h"
#include "CLadderEntity.h"

namespace SAMPLES {
	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The ladder node
	*/
	CLadderEntity::CLadderEntity(ILevel * parent, DE::CStaticMeshSceneNode * meshNode) : ITriggerEntity(parent) {

		this->m_id = meshNode->GetId();
		this->m_type = ENTITY_TYPE_LADDER;
		this->m_node = meshNode;
		this->m_node->SetMeshShadingType(DE::MESH_SHADING_SMOOTH);

		// Read the attributes
		DE::CVector3F boundingBoxSize = DE::CVector3F(1, 1, 1);
		DE::CVector3F boundingBoxPosition = DE::CVector3F(0, 0, 0);
		if(meshNode->GetAttribute("boundingBox_size") != "") {
			boundingBoxSize = boundingBoxSize.GetFrom(meshNode->GetAttribute("boundingBox_size"), boundingBoxSize);
		}
		if(meshNode->GetAttribute("boundingBox_position") != "") {
			boundingBoxPosition = boundingBoxPosition.GetFrom(meshNode->GetAttribute("boundingBox_position"), boundingBoxPosition);
		}

		// Set the bounding box
		this->m_boundingBox.SetPosition(this->m_node->GetAbsolutePosition() + boundingBoxPosition);
		this->m_boundingBox.SetSize(boundingBoxSize);
	}

	/**
	* Update the entity
	* @param time Elapsed time
	*/
	void CLadderEntity::Update(const float time) {
		ITriggerEntity::Update(time);
	}

	/**
	* Trigger the entity action
	*/
	void CLadderEntity::DoAction() {
		if(this->m_triggerParam != 0) {
			this->m_parent->GetPlayer()->SetGravityState(false);
		}
	}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	const bool CLadderEntity::CanDoAction() {

		this->m_triggerParam = this->m_boundingBox.IsCollision(this->m_parent->GetPlayer()->GetCollisionShape()) ? 1 : 0;
		return true;
	}
}
