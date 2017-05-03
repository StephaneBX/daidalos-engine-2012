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

#include "CCinematicEvent.h"

namespace SAMPLES {
	/**
	* @param parent The parent
	* @param data The name
	*/
	CCinematicEvent::CCinematicEvent(ILevel * parent, const std::string & name) {

		this->m_enabled = false;
		this->m_done = false;

		// Load the data

		// Read the steps - Tmp
		std::vector<SCinematicCameraStep> steps;
		SCinematicCameraStep step;
		step.position = DE::CVector3F(0, 6, 0);
		step.rotation = DE::CVector3F();
		step.speed = 0.15f;
		steps.push_back(step);

		SCinematicCameraStep step2;
		step2.position = DE::CVector3F(0, 6, 80);
		step2.rotation = DE::CVector3F();
		step2.speed = 0.15f;
		steps.push_back(step2);

		SCinematicCameraStep step3;
		step3.position = DE::CVector3F(40, 14, 80);
		step3.rotation = DE::CVector3F(20, -40, 0);
		step3.speed = 0.15f;
		steps.push_back(step3);

		SCinematicCameraStep step4;
		step4.position = DE::CVector3F(40, 18, 130);
		step4.rotation = DE::CVector3F(20, -70, 0);
		step4.speed = 0.15f;
		steps.push_back(step4);

		SCinematicCameraStep step5;
		step5.position = DE::CVector3F(-50, 14, 130);
		step5.rotation = DE::CVector3F(20, -180, 0);
		step5.speed = 0.15f;
		steps.push_back(step5);

		SCinematicCameraStep step6;
		step6.position = DE::CVector3F(00, 10, 100);
		step6.rotation = DE::CVector3F(10, -220, 0);
		step6.speed = 0.15f;
		steps.push_back(step6);

		SCinematicCameraStep step7;
		step7.position = DE::CVector3F(50, 5, 110);
		step7.rotation = DE::CVector3F(10, -270, 0);
		step7.speed = 0.15f;
		steps.push_back(step7);

		SCinematicCameraStep step8;
		step8.position = DE::CVector3F(100, 8, 110);
		step8.rotation = DE::CVector3F(-10, -160, 0);
		step8.speed = 0.15f;
		steps.push_back(step8);

		SCinematicCameraStep step9;
		step9.position = DE::CVector3F(100, 8, 110);
		step9.rotation = DE::CVector3F(10, 0, 0);
		step9.speed = 0.15f;
		steps.push_back(step9);

		this->m_camera = std::auto_ptr<CCinematicCamera>(new CCinematicCamera(parent, steps));

		this->m_listeners.push_back(parent);
	}

	/**
	* Update the event
	* @param time The elapsed time
	*/
	void CCinematicEvent::Update(const float time) {

		if (this->m_enabled) {

			// Update the cinematic elements
			this->m_camera->Update(time);

			// End check
			if (this->m_camera->IsDone()) {
				this->m_done = true;
				this->m_enabled = false;

				// Call the listeners
				for (auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
					(*it)->OnCinematicEventEnded(this);
				}
			}
		}
	}

	/**
	* Start the cinematic
	*/
	void CCinematicEvent::Start() {
		this->m_camera->Start();
		this->m_enabled = true;

		// Call the listeners
		for (auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
			(*it)->OnCinematicEventStarted(this);
		}
	}

	/**
	* Pause the Cinematic
	*/
	void CCinematicEvent::Pause() {
		this->m_camera->Pause();
		this->m_enabled = false;

		// Call the listeners
		for (auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
			(*it)->OnCinematicEventPaused(this);
		}
	}

	/**
	* Stop the Cinematic
	*/
	void CCinematicEvent::Stop() {
		this->m_camera->Stop();
		this->m_enabled = false;

		// Call the listeners
		for (auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
			(*it)->OnCinematicEventStopped(this);
		}
	}

	/**
	* If the event is done
	* @return True if the event is done
	*/
	const bool CCinematicEvent::IsDone() const {
		return this->m_done;
	}

	/**
	* Get the camera node
	* @return The camera node
	*/
	DE::ICameraSceneNode * CCinematicEvent::GetCameraNode() {
		return this->m_camera->GetCameraNode();
	}
}
