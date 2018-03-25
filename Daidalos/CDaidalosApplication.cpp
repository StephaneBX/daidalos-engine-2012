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

#include <sstream>
#include <Enums.hpp>
#include "Scene/Particles/Initializers/CColorParticleInitializer.hpp"
#include "Scene/Particles/Initializers/CLifeParticleInitializer.hpp"
#include "Scene/Particles/Initializers/CRotationParticleInitializer.hpp"
#include "Scene/Particles/Initializers/CSizeParticleInitializer.hpp"
#include "Scene/Particles/Initializers/CLinearVelocityParticleInitializer.hpp"
#include "Scene/Particles/Initializers/CSphereVelocityParticleInitializer.hpp"
#include "Scene/Particles/Updaters/CColorParticleUpdater.hpp"
#include "Scene/Particles/Updaters/CGravityParticleUpdater.hpp"
#include "Scene/Particles/Updaters/CLifeParticleUpdater.hpp"
#include "Scene/Particles/Updaters/CRotationParticleUpdater.hpp"
#include "Scene/Particles/Updaters/CSizeParticleUpdater.hpp"
#include "Scene/Particles/Updaters/CLinearVelocityParticleUpdater.hpp"
#include "Scene/Particles/Updaters/CSphereVelocityParticleUpdater.hpp"
#include "Level/CDaidalosLevel.hpp"
#include "CDaidalosApplication.hpp"

namespace daidalos {
	CDaidalosApplication::CDaidalosApplication(const HINSTANCE hInstance, const int nCmdShow, const std::string levelName) : daidalosgameengine::IForwardApplication(hInstance, nCmdShow) {

		SetLoading(true);
		GetCursor().SetVisible(true);
		std::thread(&CDaidalosApplication::LoadLevel, this, levelName).detach();

		MainLoop();
	}

	CDaidalosApplication::~CDaidalosApplication() {}

	/**
	* Load a level and show a loading screen
	* Should be called from a new thread
	* @param filename The level filename
	*/
	void CDaidalosApplication::LoadLevel(const std::string & filename) {
		SetLevel(new CDaidalosLevel());
		GetCurrentLevel()->AddListener(this);
		GetCurrentLevel()->LoadLevel(filename);
	}

	void CDaidalosApplication::Update(const float time) {

		daidalosgameengine::IForwardApplication::Update(time);
		if(!IsLoading()) {
			if(this->m_input->IsActionActive(FPS_CAMERA_EXIT) || this->m_input->IsGamepadActionActive(0, FPS_CAMERA_EXIT)) {
				this->m_alive = false;
			}

			GetCurrentLevel()->Update(time);
			GetCursor().Update(time);

		} else {
			GetLoadingScene()->Update(time);
		}
	}

	/**
	* Callback when the level is fully loaded
	*/
	void CDaidalosApplication::OnLevelLoaded() {
		SetLoading(false);
		GetCurrentLevel()->GetView()->SetCursor(&GetCursor());
	}

	/**
	* Callback when the next level is triggered
	*/
	void CDaidalosApplication::OnTriggerNextLevel(const std::string & nextLevel) {
		// Nothing to do
	}
}
