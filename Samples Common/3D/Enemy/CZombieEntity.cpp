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

#include <Scene/3D/CEllipse.h>
#include "../../Level/ILevel.h"
#include "CZombieEntity.h"
#include "States/CEnemyStateWait.h"
#include "States/CEnemyStatePatrol.h"
#include "States/CEnemyStateFollow.h"
#include "States/CEnemyStateAttack.h"
#include "States/CEnemyStateDie.h"

namespace SAMPLES {
	#define GRAVITY -20.0f
	#define COLLISION_SHAPE_WIDTH 1.0f
	#define COLLISION_SHAPE_HEIGHT 2.0f
	#define VIEW_ANGLE 30.0f
	#define VIEW_DISTANCE 35.0f
	#define ATTACK_RANGE 4.0f
	#define ATTACK_POWER 12

	#define ANIMATION_WALK_START 22
	#define ANIMATION_WALK_END 36
	#define ANIMATION_WALK_SPEED 8.0f
	#define WALK_SPEED 2.0f
	#define ROTATION_SPEED 30.0f

	#define ANIMATION_WAIT_START 137
	#define ANIMATION_WAIT_END 168
	#define ANIMATION_WAIT_SPEED 10.0f
	#define WAIT_TIME 6000.0f

	#define ANIMATION_DIE_START 91
	#define ANIMATION_DIE_END 103
	#define ANIMATION_DIE_SPEED 9.0f

	#define ANIMATION_ATTACK_START 116
	#define ANIMATION_ATTACK_END 128
	#define ANIMATION_ATTACK_SPEED 8.0f

	#define FOLLOW_SEARCH_TIME 9000.0f

	/**
	* Constructor
	* @param parent The parent
	* @param node The mesh node
	*/
	CZombieEntity::CZombieEntity(ILevel * parent, DE::CAnimatedMeshSceneNode * node) : IEnemyEntity(parent, node) {
		this->m_gravity = DE::CVector3F(0, GRAVITY, 0);
		this->m_collisionShape = std::unique_ptr<DE::IShape3DF>(new DE::CEllipseF(this->m_meshNode->GetPosition() + DE::CVector3F(0, COLLISION_SHAPE_HEIGHT, 0), DE::CVector2F(COLLISION_SHAPE_WIDTH, COLLISION_SHAPE_HEIGHT)));

		// Read the waypoints
		std::string waypointsString = this->m_meshNode->GetAttribute("patrol_waypoints");
		std::vector<DE::CVector3F> wayPoints;
		if(waypointsString != "") {
			DE::CTokenizer tokenizer;
			tokenizer.AddLine(waypointsString);
			if(tokenizer.Tokenize("|")) {
				for(auto it = tokenizer.GetTokens().begin(); it != tokenizer.GetTokens().end(); ++it) {
					wayPoints.push_back(DE::CVector3F(this->m_meshNode->GetAbsolutePosition() + DE::CVector3F::GetFrom(*it)));
				}
			}
		}
		
		this->m_actionStates.push_back(std::unique_ptr<IEnemyState>(new CEnemyStatePatrol(0, ANIMATION_WALK_START, ANIMATION_WALK_END, ANIMATION_WALK_SPEED, WALK_SPEED, ROTATION_SPEED, wayPoints)));
		this->m_actionStates.push_back(std::unique_ptr<IEnemyState>(new CEnemyStateWait(1, ANIMATION_WAIT_START, ANIMATION_WAIT_END, ANIMATION_WAIT_SPEED, WAIT_TIME)));
		this->m_actionStates.push_back(std::unique_ptr<IEnemyState>(new CEnemyStateFollow(2, ANIMATION_WALK_START, ANIMATION_WALK_END, ANIMATION_WALK_SPEED, this->m_parent->GetPlayer(), WALK_SPEED, ROTATION_SPEED, ATTACK_RANGE, FOLLOW_SEARCH_TIME)));
		this->m_actionStates.push_back(std::unique_ptr<IEnemyState>(new CEnemyStateAttack(3, ANIMATION_ATTACK_START, ANIMATION_ATTACK_END, ANIMATION_ATTACK_SPEED, this->m_parent->GetPlayer(), ATTACK_POWER, ATTACK_RANGE)));
		this->m_actionStates.push_back(std::unique_ptr<IEnemyState>(new CEnemyStateDie(99, ANIMATION_DIE_START, ANIMATION_DIE_END, ANIMATION_DIE_SPEED)));

		// Link the wait state with the patrol interruption callback
		this->m_actionStates.front()->AddListener(this->m_actionStates[1].get());

		this->m_currentActionState = this->m_actionStates.front().get();
		this->m_visiblePosition = this->m_collisionShape->GetPosition();

		// Change the default camera settings
		this->m_cameraNode->SetAngle(DE::DegreeToRadian(VIEW_ANGLE));
		this->m_cameraNode->SetPosition(DE::CVector3F(0, 4, 0));
		this->m_cameraNode->SetFarDistance(VIEW_DISTANCE);
	}

	/**
	* Update the enemy entity
	* @param time Elapsed time
	*/
	void CZombieEntity::Update(float const time) {
		IEnemyEntity::Update(time);

		this->m_meshNode->SetPosition(this->m_visiblePosition - DE::CVector3F(0, COLLISION_SHAPE_HEIGHT, 0));
	}

	/**
	* Determine the next active state
	*/
	void CZombieEntity::SelectNextActionState() {
		if(this->m_currentActionState != nullptr) {

			IEnemyState * nextState = nullptr;
			for(auto it = this->m_actionStates.begin(); it != this->m_actionStates.end(); ++it) {
				if((*it)->IsCandidateForNextAction(this)) {

					if(nextState == nullptr || (*it)->GetPriority() > nextState->GetPriority()) {
						nextState = (*it).get();
					} 
				}
			}

			if(this->m_currentActionState != nextState) {
				// Get the next available state
				this->m_currentActionState = nextState;
				this->m_currentActionState->Reset(this);
			}

			
		}
	}

	/**
	* Callback used to handle event when the entity fall and touch the ground
	* @param gravity The gravity value before touching the ground
	*/
	void CZombieEntity::OnEntityTouchedGround(const DE::CVector3F gravity) {

	}
}
