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

#include "../../Enums.hpp"
#include "../../Level/ILevel.hpp"
#include "CCinematicEntity.hpp"

namespace daidalosgameengine {
	/**
	* Constructor
	* @param parent The parent
	* @param node The node
	*/
	CCinematicEntity::CCinematicEntity(ILevel * parent, daidalosengine::ISceneNode * node) : m_node(node), ITriggerEntity(parent) {

		this->m_triggered = false;
		SetId(node->GetId());
		SetType(ENTITY_TYPE_AREA_TRIGGER);
		std::string cinematicFile;

		// Read the attributes
		if (this->m_node->GetAttribute("triggerRadius") != "") {
			std::istringstream ss(m_node->GetAttribute("triggerRadius"));
			ss >> this->m_triggerRadius;
		}
		if (this->m_node->GetAttribute("cinematic") != "") {
			std::istringstream ss(m_node->GetAttribute("cinematic"));
			ss >> cinematicFile;
		}

		this->m_cinematicEvent = std::auto_ptr<CCinematicEvent>(new CCinematicEvent(parent));
		this->m_cinematicEvent->LoadFromFile(cinematicFile);
	}

	/**
	* Update the entity
	* @param time Elapsed time
	*/
	void CCinematicEntity::Update(const float time) {

		ITriggerEntity::Update(time);

		if (this->m_triggered) {
			// If over, enable back the player
			if (this->m_cinematicEvent->IsDone()) {
				this->m_triggered = false;
				GetParent()->GetPlayer()->SetEnabled(true);
				GetParent()->SetActiveCamera(GetParent()->GetPlayer()->GetCameraNode());
			} else {
				this->m_cinematicEvent->Update(time);
			}
		}
	}

	/**
	* Trigger the entity action
	*/
	void CCinematicEntity::DoAction() {
		this->m_triggered = true;
		this->m_node->SetEnabled(false);

		// Disable the player
		GetParent()->GetPlayer()->SetEnabled(false);
		//this->m_parent->GetHud()->

		// Attach the camera
		GetParent()->SetActiveCamera(this->m_cinematicEvent->GetCameraNode());

		// Start the cinematic
		this->m_cinematicEvent->Start();
	}

	/**
	* Check if the action can be triggered
	* @return True if the action can be triggered, false otherwise
	*/
	const bool CCinematicEntity::CanDoAction() {
		return this->m_node->IsEnabled() && daidalosengine::CVector3F::Distance(this->m_node->GetAbsolutePosition(), GetParent()->GetPlayer()->GetVisiblePosition()) <= this->m_triggerRadius;
	}
}
