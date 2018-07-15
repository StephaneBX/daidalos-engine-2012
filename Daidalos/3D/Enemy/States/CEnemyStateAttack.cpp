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
#include "CEnemyStateAttack.hpp"

namespace daidalos {
	/**
	* Constructor
	* @param priority The priority
	* @param startAnimation The start animation frame
	* @param endAnimation The end animation frame
	* @param animationSpeed The animation speed
	* @param target The target
	* @param damage The damage
	* @param The range
	*/
	CEnemyStateAttack::CEnemyStateAttack(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, daidalosgameengine::ICollidableEntity * target, const unsigned int damage, const float range, const float attackDelay) : m_damage(damage),  m_range(range), m_target(target), m_attackDelay(attackDelay) ,IEnemyState(priority) {

		this->SetAnimationFrames(daidalosengine::CVector2F(startAnimation, endAnimation));
		this->SetAnimationSpeed(animationSpeed);
		this->m_attackTimer = 0;
	}

	/**
	* Destructor
	*/
	CEnemyStateAttack::~CEnemyStateAttack() = default;

	/**
	* Set the damage.
	* @param damage The damage
	*/
	void CEnemyStateAttack::SetDamage(const unsigned int damage) {
		this->m_damage = damage;
	}

	/**
	* Set the range.
	* @param range The range.
	*/
	void CEnemyStateAttack::SetRange(const float range) {
		this->m_range = range;
	}

	/**
	* Set attack timer.
	* @param attackTimer The attack timer
	*/
	void CEnemyStateAttack::SetAttackTimer(const float attackTimer) {
		this->m_attackTimer = attackTimer;
	}

	/**
	* Set the attack delay.
	* @param attackDelay The attack delay
	*/
	void CEnemyStateAttack::SetAttackDelay(const float attackDelay) {
		this->m_attackDelay = attackDelay;
	}

	/**
	* Set the target.
	* @param target The target
	*/
	void CEnemyStateAttack::SetTarget(daidalosgameengine::ICollidableEntity * target) {
		this->m_target = target;
	}

	/**
	* Update the entity
	* @param entity The entity
	* @param time Elapsed time
	*/
	void CEnemyStateAttack::Update(daidalosgameengine::IEnemyEntity * entity, const float time) {

		if(entity->GetMeshNode()->GetAnimationState() != daidalosengine::MESH_ANIMATION_STATE_PLAYING && this->m_attackTimer == 0) {
			entity->GetMeshNode()->SetLooped(false);
			entity->GetMeshNode()->StartAnimation(this->GetAnimationFrames().x, this->GetAnimationFrames().y, this->GetAnimationSpeed());
			this->m_target->TakeDamage(this->m_damage);
			this->m_attackTimer = this->m_attackDelay;
		} else {
			this->m_attackTimer -= time;
			if(this->m_attackTimer < 0) {
				this->m_attackTimer = 0;
			}
		}

		PerformEnemyLevelCollisions(entity, daidalosengine::CVector3F(), time);
	}

	/**
	* If the state is candidate for the next action
	* @param entity The entity
	* @return True if candidate
	*/
	bool CEnemyStateAttack::IsCandidateForNextAction(daidalosgameengine::IEnemyEntity * entity) {
		return  this->m_attackTimer > 0 || (this->m_target->IsEnabled() && !entity->GetCameraNode()->FrustumCull(this->m_target->GetCollisionShape()) && (entity->GetVisiblePosition() - this->m_target->GetVisiblePosition()).Length() <= this->m_range);
	}

	/**
	* Get the damage.
	* @return The damage
	*/
	unsigned CEnemyStateAttack::GetDamage() const {
		return this->m_damage;
	}

	/**
	* Get the range.
	* @reurn The range
	*/
	float CEnemyStateAttack::GetRange() const {
		return this->m_range;
	}

	/**
	* Get the attack timer.
	* @return the attack timer
	*/
	float CEnemyStateAttack::GetAttackTimer() const {
		return this->m_attackTimer;
	}

	/**
	* Get the attack delay.
	* @return The attack delay
	*/
	float CEnemyStateAttack::GetAttackDelay() const {
		return this->m_attackDelay;
	}

	/**
	* Get the target.
	* @return The target
	*/
	daidalosgameengine::ICollidableEntity * CEnemyStateAttack::GetTarget() const {
		return this->m_target;
	}
}

