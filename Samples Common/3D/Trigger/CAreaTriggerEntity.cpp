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
#include "CAreaTriggerEntity.h"

namespace SAMPLES {
	/**
	* Constructor
	* @param parent The parent
	* @param node The node
	*/
	CAreaTriggerEntity::CAreaTriggerEntity(ILevel * parent, DE::ISceneNode * node) : ITriggerEntity(parent) {

		this->m_type = ENTITY_TYPE_AREA_TRIGGER;
		this->m_node = node;
		
		// Read the attributes
		if(this->m_node->GetAttribute("triggerRadius") != "") {
			std::istringstream ss(m_node->GetAttribute("triggerRadius"));
			ss >> this->m_triggerRadius;
		}
		if(this->m_node->GetAttribute("triggerNodeIds") != "") {
			DE::CTokenizer tokenizer;
			tokenizer.AddLine(this->m_node->GetAttribute("triggerNodeIds"));
			if(tokenizer.Tokenize("|")) {
				for(auto it = tokenizer.GetTokens().begin(); it != tokenizer.GetTokens().end(); ++it) {
					int nodeId;
					std::istringstream ss(*it);
					ss >> nodeId;
					this->m_triggerNodeIds.push_back(nodeId);
				}
			}
		}
	}

	/**
	* Update the armor bonus
	* @param time Elapsed time
	*/
	void CAreaTriggerEntity::Update(const float time) {

		ITriggerEntity::Update(time);
	}

	/**
	* Trigger the entity action
	*/
	void CAreaTriggerEntity::DoAction() {
		this->m_node->SetEnabled(false);

		// Loop through the ids
		for(auto it = this->m_triggerNodeIds.begin(); it != this->m_triggerNodeIds.end(); ++it) {
			std::vector<DE::ISceneNode*> nodes = this->m_parent->GetScene()->GetNodeArray((*it));

			// Loop through the nodes
			for(auto it2 = nodes.begin(); it2 != nodes.end(); ++it2) {
				(*it2)->SetEnabled(!(*it2)->IsEnabled());
			}
		}
	}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	const bool CAreaTriggerEntity::CanDoAction() {
		return this->m_node->IsEnabled() && DE::CVector3F::Distance(this->m_node->GetAbsolutePosition(), this->m_parent->GetPlayer()->GetVisiblePosition()) <= this->m_triggerRadius;
	}
}
