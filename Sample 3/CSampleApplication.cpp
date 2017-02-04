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

#include "CSampleApplication.h"
#include "CSampleLevel.h"

namespace SAMPLE3 {
	CSampleApplication::CSampleApplication(const HINSTANCE hInstance, const int nCmdShow) : SAMPLES::IForwardSampleApplication(hInstance, nCmdShow) {

		this->m_loading = true;
		std::thread(&CSampleApplication::LoadLevel, this, "sample3.des").detach();

		MainLoop();
	}

	CSampleApplication::~CSampleApplication() {}

	/**
	* Load a level and show a loading screen
	* Should be called from a new thread
	* @param filename The level filename
	*/
	void CSampleApplication::LoadLevel(const std::string & filename) {
		this->m_currentLevel = std::unique_ptr<SAMPLES::ILevel>(new CSampleLevel());
		this->m_currentLevel->AddListener(this);
		this->m_currentLevel->LoadLevel(filename);
	}

	void CSampleApplication::Update(const float time) {
		
		IForwardSampleApplication::Update(time);
		if(!this->m_loading) {
			if(this->m_input->IsActionActive(SAMPLES::FPS_CAMERA_EXIT) || this->m_input->IsGamepadActionActive(0, SAMPLES::FPS_CAMERA_EXIT)) {
				this->m_alive = false;
			}

			this->m_currentLevel->Update(time);
			this->m_cursor.Update(time);
		} else {
			this->m_loadingScene->Update(time);
		}
	}

	/**
	* Callback when the level is fully loaded
	*/
	void CSampleApplication::OnLevelLoaded() {
		this->m_loading = false;
	}

	/**
	* Callback when the next level is triggered
	*/
	void CSampleApplication::OnTriggerNextLevel(const std::string & nextLevel) {
		
	}
}
