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

#include "../Enums.h"
#include "ILevel.h"

namespace SAMPLES {

#define PLAYER_NODE_NAME "player"

	/**
	* Load the level. Should be called from a specific thread.
	* @param filename The filename
	*/
	void ILevel::LoadLevel(const std::string & filename) {
		this->m_view = std::unique_ptr<DE::C2DScene>(new DE::C2DScene());

		this->m_scene = std::unique_ptr<DE::C3DScene>(new DE::C3DScene());
		this->m_scene->LoadScene(filename);
		this->m_activeCamera = (DE::ICameraSceneNode*)this->m_scene->GetNode(PLAYER_NODE_NAME);

		this->m_scene->GetRootNode()->SetMeshShadingType(DE::MESH_SHADING_FLAT);
		this->m_scene->SetActiveCamera(this->m_activeCamera);
		this->m_scene->UpdateAll(0);

		this->m_player = nullptr;

		PostLoadLevel();
	}

	/**
	* Add a listener
	* @param listener The listener
	*/
	void ILevel::AddListener(ILevelListener * listener) {
		this->m_listeners.push_back(listener);
	}

	/**
	* Set the active camera
	* @param activeCamera The active camera
	*/
	void ILevel::SetActiveCamera(DE::ICameraSceneNode * activeCamera) {
		this->m_activeCamera = activeCamera;
	}

	/**
	* Update the level
	* @param time The elapsed time
	*/
	void ILevel::Update(const float time) {
		// Update the player
		if(this->m_player != nullptr) {
			this->m_player->Update(time);
		}
		
		// Update the 3D scene
		this->m_scene->SetActiveCamera(this->m_activeCamera);
		this->m_scene->UpdateAll(time);

		// Update the hud
		this->m_view->Update(time);

		// Update the entities
		for(auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
			(*it)->Update(time);
		}
	}

	/**
	* Get the scene graph
	* @return The scene graph
	*/
	DE::C3DScene * ILevel::GetScene() {
		return this->m_scene.get();
	}

	/**
	* Get the 2d view
	* @return The 2d view
	*/
	DE::C2DScene * ILevel::GetView() {
		return this->m_view.get();
	}

	/**
	* Get the active camera
	* @return The active camera
	*/
	DE::ICameraSceneNode * ILevel::GetActiveCamera() {
		return this->m_activeCamera;
	}

	/**
	* Get the player
	* @return The player
	*/
	IPlayer * ILevel::GetPlayer() {
		return this->m_player.get();
	}

	/**
	* Get the entities
	* @return The entities
	*/
	std::vector<std::unique_ptr<IEntity> > & ILevel::GetEntities() {
		return this->m_entities;
	}

	/**
	* Get the entities with a given id
	* @return The entities with the given id
	*/
	std::vector<IEntity*> ILevel::GetEntities(const int id) const {
		std::vector<IEntity*> entities;

		for(auto it = this->m_entities.begin(); it != this->m_entities.end(); ++it) {
			if((*it)->GetId() == id) {
				entities.push_back((*it).get());
			}
		}
		return entities;
	}

	/**
	* Get the trigger entities with a given id
	* @return The entities with the given id
	*/
	std::vector<ITriggerEntity*> ILevel::GetTriggerEntities(const int id) const {
		std::vector<ITriggerEntity*> entities;

		for(auto it = this->m_entities.begin(); it != this->m_entities.end(); ++it) {
			if((*it)->GetId() == id && ((*it)->GetFlag() & ENTITY_FLAG_TRIGGER) != 0) {
				entities.push_back((ITriggerEntity*)(*it).get());
			}
		}
		return entities;
	}

	/**
	* Callback when the cinematic event started
	* @param cinematicEvent The event
	*/
	void ILevel::OnCinematicEventStarted(CCinematicEvent * cinematicEvent) {
		
	}

	/**
	* Callback when the cinematic event paused
	* @param cinematicEvent The event
	*/
	void ILevel::OnCinematicEventPaused(CCinematicEvent * cinematicEvent) {

	}

	/**
	* Callback when the cinematic event stopped
	* @param cinematicEvent The event
	*/
	void ILevel::OnCinematicEventStopped(CCinematicEvent * cinematicEvent) {

	}

	/**
	* Callback when the cinematic event ended
	* @param cinematicEvent The event
	*/
	void ILevel::OnCinematicEventEnded(CCinematicEvent * cinematicEvent) {

	}
}
