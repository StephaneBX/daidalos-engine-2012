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

#ifndef __DAIDALOS_CZOMBIESTATEFOLLOW_HPP
#define __DAIDALOS_CZOMBIESTATEFOLLOW_HPP

#include <Medias/CSound.hpp>
#include "CEnemyStateFollow.hpp"

namespace daidalos {
	/**
	* Follow zombie state
	*/
	class CZombieStateFollow : public CEnemyStateFollow {
	private:
		float m_soundTimer;
		float m_soundDelay;
		std::vector<std::unique_ptr<daidalosengine::CSound>> m_sounds;

	public:
		/**
		* Constructor
		* @param priority The priority
		* @param startAnimation The start animation frame
		* @param endAnimation The end animation frame
		* @param animationSpeed The animation speed
		* @param target The target entity
		* @param moveSpeed The move speed
		* @param rotationSpeed The rotation speed
		* @param stopDistance the stop distance
		* @param searchTime Time the entity search if the target is lost
		*/
		CZombieStateFollow(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, daidalosgameengine::ICollidableEntity * target, const float moveSpeed, const float rotationSpeed, const float stopDistance, const float searchTime);

		/**
		* Destructor
		*/
		virtual ~CZombieStateFollow();

		/**
		* Update the entity
		* @param entity The entity
		* @param time Elapsed time
		*/
		virtual void Update(daidalosgameengine::IEnemyEntity * entity, const float time) override;
	};
}

#endif
