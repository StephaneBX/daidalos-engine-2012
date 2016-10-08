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

#ifndef __IENTITY_H
#define __IENTITY_H

#include <set>

namespace SAMPLES {

	enum EEntityType {
		ENTITY_TYPE_ALARM,
		ENTITY_TYPE_DOOR,
		ENTITY_TYPE_FAN,
		ENTITY_TYPE_SWITCH,
		ENTITY_TYPE_TELEPORTER,
		ENTITY_TYPE_ARMOR,
		ENTITY_TYPE_HEALTH,
		ENTITY_TYPE_FLASHLIGHT
	};

	/**
	* Abstract entity
	*/
	class IEntity {
	protected:
		EEntityType m_type;
		std::set<IEntity*> m_targetsEntity;

	public:
		/**
		* Add a target entity
		* @param entity The target entity
		*/
		void AddTarget(IEntity * entity);

		/**
		* Remove a target entity
		* @param entity The target entity
		*/
		void RemoveTarget(IEntity * entity);

		/**
		* Get the entity type
		* @return The entity type
		*/
		const EEntityType GetType() const;

		/**
		* Update the entity
		* @param time Elapsed time
		*/
		virtual void Update(float const time) = 0;

		/**
		* Trigger the entity action
		*/
		virtual void DoAction() = 0;

		/**
		* Check if the action can be triggered
		* @return True if the action can be triggered, false otherwise
		*/
		virtual const bool CanDoAction() const = 0;
	};
}

#endif
