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
#include "CZombieStateWait.hpp"

namespace daidalos {
	/**
	* Constructor
	* @param priority The priority
	* @param startAnimation The start animation frame
	* @param endAnimation The end animation frame
	* @param animationSpeed The animation speed
	* @param waitTime The wait time
	*/
	CZombieStateWait::CZombieStateWait(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, const float waitTime) : CEnemyStateWait(priority, startAnimation, endAnimation, animationSpeed, waitTime) {
		this->m_moanSound.LoadFromFile("zombie_moan.wav", daidalosengine::SOUND_TYPE_STATIC, daidalosengine::SOUND_CATEGORY_ENVIRONMENT);
		this->m_moanSound.SetRelativeToListener(false);
	}

	/**
	* Update the entity
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CZombieStateWait::Update(daidalosgameengine::IEnemyEntity * entity, const float time) {

		// Play the moan sound (33% chance)
		if(entity->GetMeshNode()->GetAnimationState() == daidalosengine::MESH_ANIMATION_STATE_STOPPED && daidalosengine::RandomNumber<int>(1, 3) == 1) {
			this->m_moanSound.SetPosition(entity->GetVisiblePosition());
			this->m_moanSound.SetStatus(daidalosengine::SOUND_STATUS_PLAY);
		}

		CEnemyStateWait::Update(entity, time);
		this->m_moanSound.SetPosition(entity->GetVisiblePosition());
	}
}

