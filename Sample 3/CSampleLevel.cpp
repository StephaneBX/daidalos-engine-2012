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

#include "CSampleLevel.h"
#include "Level/ILevel.h"
#include "CFPSCamera.h"
#include "3D/Trigger/CFlashlightEntity.h"
#include "CHealthEntity.h"

namespace SAMPLE3 {

	/**
	* Update the level
	* @param time The elapsed time
	*/
	void CSampleLevel::Update(const float time) {
		ILevel::Update(time);

	}

	/**
	* Do pre level loading process
	*/
	void CSampleLevel::PreLoadLevel() {

	}

	/**
	* Do post level loading process
	*/
	void CSampleLevel::PostLoadLevel() {
		this->m_entities.clear();

		// Load the camera
		this->m_player = std::unique_ptr<SAMPLES::CFPSCamera>(new SAMPLES::CFPSCamera(this, this->m_activeCamera, this->m_scene->GetNode("flashlight")));

		// Load the flashlight
		SAMPLES::CFlashlightEntity * flashlight = new SAMPLES::CFlashlightEntity(this, this->m_scene->GetNode("flashlight_entity"));
		this->m_entities.push_back(std::unique_ptr<SAMPLES::CFlashlightEntity>(flashlight));

		// Load the health bonus
		std::vector<DE::ISceneNode*> health = this->m_scene->GetNodeArray("health_entity");

		for(auto it = health.begin(); it != health.end(); it++) {
			CHealthEntity * entity = new CHealthEntity(this, (DE::CAnimatedMeshSceneNode*)(*it));
			this->m_entities.push_back(std::unique_ptr<CHealthEntity>(entity));
		}

		// End of loading callback
		for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
			(*it)->OnLevelLoaded();
		}
	}
}
