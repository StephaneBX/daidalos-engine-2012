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

#ifndef __DAIDALOS_CENEMYSTATEATTACK_HPP
#define __DAIDALOS_CENEMYSTATEATTACK_HPP

#include <3D/Enemy/IEnemyState.hpp>
#include <3D/ICollidableEntity.hpp>

namespace daidalos {
	/**
	* Attack enemy state
	*/
	class CEnemyStateAttack : public daidalosgameengine::IEnemyState {
	private:
		unsigned int m_damage;
		float m_range;
		float m_attackTimer;
		float m_attackDelay;
		daidalosgameengine::ICollidableEntity * m_target;

	public:
		/**
		* Constructor
		* @param priority The priority
		* @param startAnimation The start animation frame
		* @param endAnimation The end animation frame
		* @param animationSpeed The animation speed
		* @param target The target
		* @param damage The damage
		* @param range The range
		* @param attackDelay The attack delay
		*/
		CEnemyStateAttack(const unsigned int priority, const float startAnimation, const float endAnimation, const float animationSpeed, daidalosgameengine::ICollidableEntity * target, const unsigned int damage, const float range, const float attackDelay);

		/**
		* Destructor
		*/
		virtual ~CEnemyStateAttack();

		/**
		* Set the damage.
		* @param damage The damage
		*/
		void SetDamage(const unsigned int damage);

		/**
		* Set the range.
		* @param range The range.
		*/
		void SetRange(const float range);

		/**
		* Set attack timer.
		* @param attackTimer The attack timer
		*/
		void SetAttackTimer(const float attackTimer);

		/**
		* Set the attack delay.
		* @param attackDelay The attack delay
		*/
		void SetAttackDelay(const float attackDelay);

		/**
		* Set the target.
		* @param target The target
		*/
		void SetTarget(daidalosgameengine::ICollidableEntity * target);

		/**
		* Update the entity
		* @param entity The entity
		* @param time Elapsed time
		*/
		virtual void Update(daidalosgameengine::IEnemyEntity * entity, const float time) override;

		/**
		* If the state is candidate for the next action
		* @param entity The entity
		* @return True if candidate
		*/
		virtual bool IsCandidateForNextAction(daidalosgameengine::IEnemyEntity * entity) override;

		/**
		* Get the damage.
		* @return The damage
		*/
		unsigned GetDamage() const;

		/**
		* Get the range.
		* @reurn The range
		*/
		float GetRange() const;

		/**
		* Get the attack timer.
		* @return the attack timer
		*/
		float GetAttackTimer() const;

		/**
		* Get the attack delay.
		* @return The attack delay
		*/
		float GetAttackDelay() const;

		/**
		* Get the target.
		* @return The target
		*/
		daidalosgameengine::ICollidableEntity * GetTarget() const;
	};
}

#endif
