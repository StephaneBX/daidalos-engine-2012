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

#include <Core/CResourceManager.hpp>
#include <Core/CMediaManager.hpp>
#include "CCinematicEvent.hpp"

namespace GE {
	/**
	* @param parent The parent
	* @param data The name
	*/
	CCinematicEvent::CCinematicEvent(ILevel * parent) {

		this->m_enabled = false;
		this->m_done = false;
		this->m_parent = parent;
		this->m_cinematicEventData = nullptr;
		this->m_listeners.push_back(parent);
	}

	/**
	* Destructor
	*/
	CCinematicEvent::~CCinematicEvent() {
		if (this->m_cinematicEventData != nullptr) {
			DE::CResourceManager::GetInstance()->ReleaseResource(this->m_cinematicEventData->GetName());
		}
	}

	/**
	* Load a font file
	* @param filename The font file name
	*/
	void CCinematicEvent::LoadFromFile(const std::string & filename) {
		// Release previously used resource
		if (this->m_cinematicEventData != nullptr) {
			DE::CResourceManager::GetInstance()->ReleaseResource(this->m_cinematicEventData->GetName());
		}

		// Check the existence of the resource and load it if necessary
		ICinematicEvent * tmp = DE::CResourceManager::GetInstance()->GetResource<ICinematicEvent>(filename);

		if (tmp == nullptr) {
			this->m_cinematicEventData = DE::CMediaManager::GetInstance()->LoadMediaFromFile<ICinematicEvent>(filename);
			DE::CResourceManager::GetInstance()->AddResource(filename, this->m_cinematicEventData);
		} else {
			this->m_cinematicEventData = tmp;
		}

		// Create the camera from the loaded data
		this->m_camera = std::auto_ptr<CCinematicCamera>(new CCinematicCamera(this->m_parent, this->m_cinematicEventData->GetData()->cameraSteps));
	}

	/**
	* Update the event
	* @param time The elapsed time
	*/
	void CCinematicEvent::Update(const float time) {

		if (this->m_cinematicEventData != nullptr && this->m_enabled) {

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
		if (this->m_cinematicEventData != nullptr) {
			this->m_camera->Start();
			this->m_enabled = true;

			// Call the listeners
			for (auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
				(*it)->OnCinematicEventStarted(this);
			}
		}
	}

	/**
	* Pause the Cinematic
	*/
	void CCinematicEvent::Pause() {
		if (this->m_cinematicEventData != nullptr) {
			this->m_camera->Pause();
			this->m_enabled = false;

			// Call the listeners
			for (auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
				(*it)->OnCinematicEventPaused(this);
			}
		}
	}

	/**
	* Stop the Cinematic
	*/
	void CCinematicEvent::Stop() {
		if (this->m_cinematicEventData != nullptr) {
			this->m_camera->Stop();
			this->m_enabled = false;

			// Call the listeners
			for (auto it = this->m_listeners.begin(); it != this->m_listeners.end(); ++it) {
				(*it)->OnCinematicEventStopped(this);
			}
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
