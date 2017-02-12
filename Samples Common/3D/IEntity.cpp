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

#include "IEntity.h"
#include "../Level/ILevel.h"

namespace SAMPLES {

	/**
	* The constructor
	* @param parent The parent
	*/
	IEntity::IEntity(ILevel * parent) {
		this->m_parent = parent;
		this->m_id = 0;
		this->m_flag = 0;
	}

	/**
	* Set the health
	* @param health The health
	*/
	void IEntity::SetHealth(const unsigned int health) {
		this->m_health = health;
	}

	/**
	* Set the max health
	* @param health The max health
	*/
	void IEntity::SetMaxHealth(const unsigned int health) {
		this->m_maxHealth = health;
	}

	/**
	* Damage the entity
	* @param damage The damage
	*/
	void IEntity::TakeDamage(const unsigned int damage) {
		if(this->m_health <= damage) {
			this->m_health = 0;
		} else {
			this->m_health -= damage;
		}
	}

	/**
	* Generate impact particles from a position
	* @param position The emitter position
	*/
	void IEntity::GenerateImpactParticles(const DE::CVector3F & position) {

	}

	/**
	* Get the collision state
	* @return The collision state
	*/
	const bool IEntity::GetCollisionState() const {
		return false;
	}

	/**
	* Check if the entity is enabled
	* @return True if the entity is enabled
	*/
	const bool IEntity::IsEnabled() const {
		return true;
	}

	/**
	* Get the entity id
	* @return The entity id
	*/
	const int IEntity::GetId() const {
		return this->m_id;
	}

	/**
	* Get the entity flag
	* @return The entity flag
	*/
	const int IEntity::GetFlag() const {
		return this->m_flag;
	}

	/**
	* Get the entity type
	* @return The entity type
	*/
	const unsigned int IEntity::GetType() const {
		return this->m_type;
	}

	/**
	* Get the health
	* @return The health
	*/
	const unsigned int IEntity::GetHealth() const {
		return this->m_health;
	}

	/**
	* Get the max health
	* @return The max health
	*/
	const unsigned int IEntity::GetMaxHealth() const {
		return this->m_maxHealth;
	}

	/**
	* Get the visible position
	* @return The visible position
	*/
	const DE::CVector3F & IEntity::GetVisiblePosition() const {
		return this->m_visiblePosition;
	}

	/**
	* Get the entity collision shape
	* @return The entity collision shape
	*/
	DE::IShape3DF * IEntity::GetCollisionShape() {
		return nullptr;
	}
}
