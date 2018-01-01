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

#ifndef __GAMEENGINE_IENTITY_HPP
#define __GAMEENGINE_IENTITY_HPP

#include <set>
#include <Scene/3D/IShape3D.hpp>
#include <Scene/Particles/CParticleEmitter.hpp>

namespace GE {
	class ILevel;

	/**
	* Abstract entity
	*/
	class IEntity {
	private:
		int m_id;
		int m_flag;
		unsigned int m_type;
		unsigned int m_health;
		unsigned int m_maxHealth;
		DE::CVector3F m_visiblePosition;
		ILevel * m_parent;
		std::vector<DE::CParticleEmitter*> m_impactParticleEmitters;

	public:
		/**
		* The constructor
		* @param parent The parent
		*/
		IEntity(ILevel * parent);

		/**
		* Set the id
		* @param id The id
		*/
		void SetId(const int id);

		/**
		* Set the flag
		* @param flag The flag
		*/
		void SetFlag(const int flag);

		/**
		* Set the type
		* @param type The type
		*/
		void SetType(const unsigned int type);

		/**
		* Set the visible position
		* @param position The visible position
		*/
		void SetVisiblePosition(const DE::CVector3F & position);

		/**
		* Set the player active state
		* @param enabled The player active state
		*/
		virtual void SetEnabled(const bool enabled);

		/**
		* Set the health
		* @param health The health
		*/
		virtual void SetHealth(const unsigned int health);

		/**
		* Set the max health
		* @param health The max health
		*/
		virtual void SetMaxHealth(const unsigned int health);

		/**
		* Damage the entity
		* @param damage The damage
		*/
		virtual void TakeDamage(const unsigned int damage);

		/**
		* Generate impact particles from a position
		* @param position The emitter position
		*/
		virtual void GenerateImpactParticles(const DE::CVector3F & position);

		/**
		* Get the collision state
		* @return The collision state
		*/
		virtual const bool GetCollisionState() const;

		/**
		* Check if the entity is enabled
		* @return True if the entity is enabled
		*/
		virtual const bool IsEnabled() const;

		/**
		* Get the entity id
		* @return The entity id
		*/
		const int GetId() const;

		/**
		* Get the entity flag
		* @return The entity flag
		*/
		const int GetFlag() const;

		/**
		* Get the entity type
		* @return The entity type
		*/
		const unsigned int GetType() const;

		/**
		* Get the parent
		* @return The parent
		*/
		ILevel * GetParent() const;

		/**
		* Get the impact particle emitters
		* @return The impact particle emitters
		*/
		std::vector<DE::CParticleEmitter*> & GetImpactParticleEmitters();

		/**
		* Get the health
		* @return The health
		*/
		virtual const unsigned int GetHealth() const;

		/**
		* Get the max health
		* @return The max health
		*/
		virtual const unsigned int GetMaxHealth() const;

		/**
		* Get the visible position
		* @return The visible position
		*/
		virtual const DE::CVector3F & GetVisiblePosition() const;

		/**
		* Get the entity collision shape
		* @return The entity collision shape
		*/
		virtual DE::IShape3DF * GetCollisionShape();

		/**
		* Update the entity
		* @param time Elapsed time
		*/
		virtual void Update(const float time) = 0;
	};
}

#endif
