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

#ifndef __GAMEENGINE_ILEVEL_HPP
#define __GAMEENGINE_ILEVEL_HPP

#include <Scene/3D/C3DScene.hpp>
#include <Scene/2D/C2DScene.hpp>
#include "ILevelListener.hpp"
#include "../Event/ICinematicEventListener.hpp"
#include "../IPlayer.hpp"
#include "../3D/IEntity.hpp"
#include "../3D/Trigger/ITriggerEntity.hpp"

namespace daidalosgameengine {
	/**
	* The level abstract class
	*/
	class ILevel : public ICinematicEventListener {
	private:
		float m_brightness;
		std::unique_ptr<daidalosengine::C3DScene> m_scene;
		std::unique_ptr<daidalosengine::C2DScene> m_view;
		std::vector<std::unique_ptr<IEntity> > m_entities;
		std::vector<ILevelListener *> m_listeners;
		std::unique_ptr<IPlayer> m_player;
		daidalosengine::ICameraSceneNode * m_activeCamera;

	public:
		/**
		* Destrictor
		*/
		virtual ~ILevel();

		/**
		* Load the level. Should be called from a specific thread.
		* @param filename The filename
		*/
		void LoadLevel(const std::string & filename);

		/**
		* Trigger the next level by calling the listeners
		* @param levelName The level name
		*/
		void TriggerNextLevel(const std::string & levelName);

		/**
		* Set the level brightness
		* @param brightness The brightness
		*/
		void SetBrightness(const float brightness);

		/**
		* Add a listener
		* @param listener The listener
		*/
		void AddListener(ILevelListener * listener);

		/**
		* Set the active camera
		* @param activeCamera The active camera
		*/
		void SetActiveCamera(daidalosengine::ICameraSceneNode * activeCamera);

		/**
		* Set the player
		* @param player The player
		*/
		void SetPlayer(IPlayer * player);

		/**
		* Update the level
		* @param time The elapsed time
		*/
		virtual void Update(const float time);

		/**
		* Return the level brightness
		* @return The level brightness
		*/
		float GetBrightness() const;

		/**
		* Get the scene graph
		* @return The scene graph
		*/
		daidalosengine::C3DScene * GetScene();

		/**
		* Get the 2d view
		* @return The 2d view
		*/
		daidalosengine::C2DScene * GetView();

		/**
		* Get the active camera
		* @eturn The active camera
		*/
		daidalosengine::ICameraSceneNode * GetActiveCamera();

		/**
		* Get the player
		* @return The player
		*/
		IPlayer * GetPlayer();

		/**
		* Get the entities
		* @return The entities
		*/
		std::vector<std::unique_ptr<IEntity> > & GetEntities();

		/**
		* Get the entities with a given id
		* @return The entities with the given id
		*/
		std::vector<IEntity*> GetEntities(const int id) const;

		/**
		* Get the trigger entities with a given id
		* @return The entities with the given id
		*/
		std::vector<ITriggerEntity*> GetTriggerEntities(const int id) const;

		/**
		* Callback when the cinematic event started
		* @param cinematicEvent The event
		*/
		virtual void OnCinematicEventStarted(CCinematicEvent * cinematicEvent) override;

		/**
		* Callback when the cinematic event paused
		* @param cinematicEvent The event
		*/
		virtual void OnCinematicEventPaused(CCinematicEvent * cinematicEvent) override;

		/**
		* Callback when the cinematic event stopped
		* @param cinematicEvent The event
		*/
		virtual void OnCinematicEventStopped(CCinematicEvent * cinematicEvent) override;

		/**
		* Callback when the cinematic event ended
		* @param cinematicEvent The event
		*/
		virtual void OnCinematicEventEnded(CCinematicEvent * cinematicEvent) override;

	protected:
		/**
		* Do pre level loading process
		*/
		virtual void PreLoadLevel() = 0;

		/**
		* Do post level loading process
		*/
		virtual void PostLoadLevel();
	};
}

#endif
