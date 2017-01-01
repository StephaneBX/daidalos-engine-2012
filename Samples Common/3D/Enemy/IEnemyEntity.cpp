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

#include "IEnemyEntity.h"

namespace SAMPLES {

	#define DEFAULT_HEALTH 1
	#define DEFAULT_REACTION_TIME 60.0f

	/**
	* Constructor
	* @param node The mesh node
	* @param player The player
	* @param scene3D The scene
	*/
	IEnemyEntity::IEnemyEntity(DE::CAnimatedMeshSceneNode * node, IPlayer * player, DE::C3DScene * scene3D) : ICollidableEntity() {
		this->m_health = DEFAULT_HEALTH;
		this->m_maxHealth = DEFAULT_HEALTH;
		this->m_reactionTime = DEFAULT_REACTION_TIME;
		this->m_reactionTimer = 0;

		this->m_meshNode = node;
		this->m_player = player;
		this->m_scene = scene3D;
		this->m_currentActionState = nullptr;
	}

	/**
	* Update the enemy entity
	* @param time Elapsed time
	*/
	void IEnemyEntity::Update(float const time) {

		// Update the reaction timer
		this->m_reactionTimer += time;

		// If the entity can decide its next action
		if(this->m_reactionTimer >= this->m_reactionTime) {
			SelectNextActionState();
			this->m_reactionTimer = 0;
		}

		// Update the action state and perform collision state
		if(this->m_currentActionState != nullptr) {
			this->m_currentActionState->Update(this, time);
		}
	}

	/**
	* Set the health
	* @param health The health
	*/
	void IEnemyEntity::SetHealth(unsigned int health) {
		this->m_health = health;
	}

	/**
	* Set the max health
	* @param health The max health
	*/
	void IEnemyEntity::SetMaxHealth(unsigned int health) {
		this->m_maxHealth = health;
	}

	/**
	* Set the reaction time
	*/
	void IEnemyEntity::SetReactionTime(float reactionTime) {
		this->m_reactionTime = reactionTime;
	}

	/**
	* Get the health
	* @return The health
	*/
	const unsigned int IEnemyEntity::GetHealth() const {
		return this->m_health;
	}

	/**
	* Get the max health
	* @return The max health
	*/
	const unsigned int IEnemyEntity::GetMaxHealth() const {
		return this->m_maxHealth;
	}

	/**
	* Get the reaction time
	* @return The reaction time
	*/
	const float IEnemyEntity::GetReactionTime() const {
		return this->m_reactionTime;
	}

	/**
	* Get the entity view
	* @return The entity view
	*/
	DE::CFrustum * IEnemyEntity::GetView() {
		return &this->m_view;
	}

	/**
	* Get the player
	* @return The player
	*/
	IPlayer * IEnemyEntity::GetPlayer() {
		this->m_player;
	}

	/**
	* Get the mesh node
	* @return The mesh node
	*/
	DE::CAnimatedMeshSceneNode * IEnemyEntity::GetMeshNode() {
		return this->m_meshNode;
	}
}