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

#ifndef __CFANENTITY_H
#define __CFANENTITY_H

#include <Scene/3D/CAnimatedMeshSceneNode.h>
#include <Scene/3D/C3DScene.h>
#include <Medias/CSound.h>
#include "IEntity.h"
#include "../IPlayer.h"

namespace SAMPLES {

	enum EFanState {
		FAN_STATE_STOPPED,
		FAN_STATE_STOPPING,
		FAN_STATE_STARTED,
		FAN_STATE_STARTING
	};

	/**
	* Door entity
	*/
	class CFanEntity : public IEntity {
	private:
		DE::CAnimatedMeshSceneNode * m_node;
		DE::C3DScene * m_scene3D;
		IPlayer * m_player;

		EFanState m_state;
		float m_speed;

	public:
		/**
		* @param node The fan node
		* @param player The player
		* @param scene3D The scene associated to the node
		*/
		CFanEntity(DE::CAnimatedMeshSceneNode * meshNode, IPlayer * player);

		/**
		* Update the door
		* @param time Elapsed time
		*/
		void Update(float const time = 0);

		/**
		* Trigger the entity action
		*/
		void DoAction();

		/**
		* Check if the action can be triggered
		* @return True if the action can be triggered, false otherwise
		*/
		const bool CanDoAction() const;

	private:
		/**
		* Start the fan
		*/
		void Start();

		/**
		* Stop the fan
		*/
		void Stop();
	};
}

#endif
