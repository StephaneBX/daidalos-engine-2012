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

#include "IEnemyEntity.hpp"
#include "../../Enums.hpp"
#include "../../Level/ILevel.hpp"

#define DEFAULT_HEALTH 1
#define DEFAULT_CAMERA_ANGLE 20.0f
#define DEFAULT_CAMERA_RATIO 1.2f
#define DEFAULT_CAMERA_NEAR 0.1f
#define DEFAULT_CAMERA_FAR 20.0f

#define ATTRIBUTE_HEALTH "health"
#define ATTRIBUTE_MAXHEALTH "max_health"

namespace GE {
	/**
	* Constructor
	* @param parent The parent
	* @param meshNode The mesh node
	*/
	IEnemyEntity::IEnemyEntity(ILevel * parent, DE::CAnimatedMeshSceneNode * meshNode) : ICollidableEntity(parent) {

		SetId(meshNode->GetId());
		SetFlag(GetFlag() | ENTITY_FLAG_ENEMY);

		SetHealth(DEFAULT_HEALTH);
		SetMaxHealth(DEFAULT_HEALTH);

		this->m_meshNode = meshNode;
		this->m_currentActionState = nullptr;

		// The camera node
		this->m_cameraNode = new DE::CProjectionCameraSceneNode(GetParent()->GetScene()->GetScene(), this->m_meshNode, "enemy_entity_camera", DE::CVector3F(), DE::CVector3F(), DE::DegreeToRadian(DEFAULT_CAMERA_ANGLE), DEFAULT_CAMERA_RATIO, DEFAULT_CAMERA_NEAR, DEFAULT_CAMERA_FAR);
		this->m_cameraNode->SetSaved(false);
		this->m_meshNode->AddChild(this->m_cameraNode);

		// Read the attributes
		if(this->m_meshNode->GetAttribute(ATTRIBUTE_HEALTH) != "") {
			std::istringstream ss(this->m_meshNode->GetAttribute(ATTRIBUTE_HEALTH));
			int health = 0;
			ss >> health;
			SetHealth(health);
		}
		if(this->m_meshNode->GetAttribute(ATTRIBUTE_MAXHEALTH) != "") {
			std::istringstream ss(this->m_meshNode->GetAttribute(ATTRIBUTE_MAXHEALTH));
			int maxHealth = 0;
			ss >> maxHealth;
			SetMaxHealth(maxHealth);
		}
	}

	/**
	* Damage the entity
	* @param damage The damage
	*/
	void IEnemyEntity::TakeDamage(const unsigned int damage) {
		IEntity::TakeDamage(damage);

		// Entity states callback
		for(auto it = this->m_actionStates.begin(); it != this->m_actionStates.end(); ++it) {
			(*it)->OnEntityTakeDamage();
		}
	}

	/**
	* Update the enemy entity
	* @param time Elapsed time
	*/
	void IEnemyEntity::Update(const float time) {

		if(this->m_meshNode->IsEnabled()) {
			// Select the next action
			SelectNextActionState();

			// Update the action state and perform collision state
			if(this->m_currentActionState != nullptr) {
				this->m_currentActionState->Update(this, time);
			}
		}
	}

	/**
	* Check if the entity is enabled
	* @return True if the entity is enabled
	*/
	const bool IEnemyEntity::IsEnabled() const {
		return this->m_meshNode != nullptr && this->m_meshNode->IsEnabled();
	}

	/**
	* Get the entity camera node
	* @return The entity camera node
	*/
	DE::CProjectionCameraSceneNode * IEnemyEntity::GetCameraNode() {
		return this->m_cameraNode;
	}

	/**
	* Get the player
	* @return The player
	*/
	IPlayer * IEnemyEntity::GetPlayer() {
		return GetParent()->GetPlayer();
	}

	/**
	* Get the mesh node
	* @return The mesh node
	*/
	DE::CAnimatedMeshSceneNode * IEnemyEntity::GetMeshNode() {
		return this->m_meshNode;
	}
}