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

#ifndef __IENEMYENTITY_H
#define __IENEMYENTITY_H

#include <Scene/3D/CFrustum.h>
#include <Scene/3D/CAnimatedMeshSceneNode.h>
#include <Scene/3D/CProjectionCameraSceneNode.h>
#include "IEnemyState.h"
#include "../ICollidableEntity.h"
#include "../../IPlayer.h"

namespace SAMPLES {
	/**
	* Abstract enemy entity
	*/
	class IEnemyEntity : public ICollidableEntity {
		friend class IEnemyState;
	protected:
		std::vector<std::unique_ptr<IEnemyState> > m_actionStates;
		IEnemyState * m_currentActionState;

		DE::CAnimatedMeshSceneNode * m_meshNode;
		DE::CProjectionCameraSceneNode * m_cameraNode;

	public:
		/**
		* Constructor
		* @param parent The parent
		* @param meshNode The mesh node
		*/
		IEnemyEntity(ILevel * parent, DE::CAnimatedMeshSceneNode * meshNode);

		/**
		* Damage the entity
		* @param damage The damage
		*/
		virtual void TakeDamage(const unsigned int damage);

		/**
		* Update the enemy entity
		* @param time Elapsed time
		*/
		virtual void Update(const float time);

		/**
		* Determine the next active state
		*/
		virtual void SelectNextActionState() = 0;

		/**
		* Check if the entity is enabled
		* @return True if the entity is enabled
		*/
		virtual const bool IsEnabled() const;

		/**
		* Get the entity camera node
		* @return The entity camera node
		*/
		DE::CProjectionCameraSceneNode * GetCameraNode();

		/**
		* Get the player
		* @return The player
		*/
		IPlayer * GetPlayer();

		/**
		* Get the mesh node
		* @return The mesh node
		*/
		DE::CAnimatedMeshSceneNode * GetMeshNode();
	};
}

#endif
