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

#include <3D/Enemy/IEnemyEntity.hpp>
#include "CZombieStateFollow.hpp"

#define SOUND_DELAY 1500.0F

namespace daidalos {

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
	CZombieStateFollow::CZombieStateFollow(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, daidalosgameengine::ICollidableEntity * target, const float moveSpeed, const float rotationSpeed, const float stopDistance, const float searchTime) : CEnemyStateFollow(priority, startAnimation, endAnimation, animationSpeed, target, moveSpeed, rotationSpeed, stopDistance, searchTime) {
		this->m_soundTimer = 0;
		this->m_soundDelay = SOUND_DELAY;

		// Initialize the sounds
		for(int i = 1; i <= 3; ++i) {
			this->m_sounds.push_back(std::unique_ptr<daidalosengine::CSound>(new daidalosengine::CSound()));
			this->m_sounds[i - 1]->LoadFromFile("zombie_groan" + std::to_string(i) + ".wav", daidalosengine::SOUND_TYPE_STATIC, daidalosengine::SOUND_CATEGORY_ENVIRONMENT);
			this->m_sounds[i - 1]->SetRelativeToListener(false);
		}
	}

	/**
	* Destructor
	*/
	CZombieStateFollow::~CZombieStateFollow() {}

	/**
	* Update the entity
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CZombieStateFollow::Update(daidalosgameengine::IEnemyEntity * entity, const float time) {
		CEnemyStateFollow::Update(entity, time);

		if(this->m_sounds.size() > 0) {
			this->m_soundTimer -= time;
			if(this->m_soundTimer < 0) {
				this->m_soundTimer = this->m_soundDelay;

				// Groaning sound (10% chance)
				if(daidalosengine::RandomNumber<float>(0, 100.0f) < 10.0F) {
					// Play one of the sounds
					int soundIndex = daidalosengine::RandomNumber<int>(0, this->m_sounds.size() - 1);
					this->m_sounds[soundIndex]->SetPosition(entity->GetVisiblePosition());
					this->m_sounds[soundIndex]->SetStatus(daidalosengine::SOUND_STATUS_PLAY);
				}
			}
		}
	}
}
