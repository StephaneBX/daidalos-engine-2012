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

#ifndef __ILEVEL_H
#define __ILEVEL_H

#include <set>
#include <Scene/3D/C3DScene.h>
#include <Scene/2D/C2DScene.h>
#include "ILevelListener.h"
#include "../IPlayer.h"
#include "../3D/IEntity.h"

namespace SAMPLES {
	/**
	* The level abstract class
	*/
	class ILevel {
	protected:
		std::unique_ptr<DE::C3DScene> m_scene;
		std::unique_ptr<DE::C2DScene> m_hud;
		std::vector<std::unique_ptr<IEntity> > m_entities;
		std::vector<ILevelListener *> m_listeners;
		std::unique_ptr<IPlayer> m_player;
		DE::ICameraSceneNode * m_activeCamera;

	public:
		/**
		* Load the level. Should be called from a specific thread.
		* @param filename The filename
		*/
		void LoadLevel(const std::string & filename);

		/**
		* Add a listener
		* @param listener The listener
		*/
		void AddListener(ILevelListener * listener);

		/**
		* Set the active camera
		* @param activeCamera The active camera
		*/
		void SetActiveCamera(DE::ICameraSceneNode * activeCamera);

		/**
		* Update the level
		* @param time The elapsed time
		*/
		virtual void Update(const float time);

		/**
		* Get the scene graph
		*/
		DE::C3DScene * GetScene();

		/**
		* Get the hud
		*/
		DE::C2DScene * GetHud();

		/**
		* Get the active camera
		*/
		DE::ICameraSceneNode * GetActiveCamera();

		/**
		* Get the player
		*/
		IPlayer * GetPlayer();

	protected:
		/**
		* Do pre level loading process
		*/
		virtual void PreLoadLevel() = 0;

		/**
		* Do post level loading process
		*/
		virtual void PostLoadLevel() = 0;
	};
}

#endif
