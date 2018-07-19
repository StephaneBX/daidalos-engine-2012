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

#include <Scene/3D/CEllipse.hpp>
#include <Level/ILevel.hpp>
#include "CZombieEntity.hpp"
#include "States/CZombieStateWait.hpp"
#include "States/CEnemyStatePatrol.hpp"
#include "States/CZombieStateFollow.hpp"
#include "States/CEnemyStateAttack.hpp"
#include "States/CEnemyStateDie.hpp"

#define GRAVITY -20.0F
#define COLLISION_SHAPE_WIDTH 1.0F
#define COLLISION_SHAPE_HEIGHT 2.0F
#define VIEW_ANGLE 60.0F
#define VIEW_DISTANCE 35.0F
#define ATTACK_RANGE 4.0F
#define ATTACK_POWER 12U
#define ATTACK_DELAY 800.0F

#define ANIMATION_WALK_START 22.0F
#define ANIMATION_WALK_END 36.0F
#define ANIMATION_WALK_SPEED 8.0F
#define WALK_SPEED 2.0F
#define ROTATION_SPEED 70.0F

#define ANIMATION_WAIT_START 137.0F
#define ANIMATION_WAIT_END 168.0F
#define ANIMATION_WAIT_SPEED 10.0F
#define WAIT_TIME 6000.0F

#define ANIMATION_DIE_START 91.0F
#define ANIMATION_DIE_END 103.0F
#define ANIMATION_DIE_SPEED 9.0F

#define ANIMATION_ATTACK_START 116.0F
#define ANIMATION_ATTACK_END 128.0F
#define ANIMATION_ATTACK_SPEED 8.0F

#define FOLLOW_SEARCH_TIME 9000.0F

#define IMPACT_PARTICLE_LIFE 50
#define IMPACT_PARTICLE_EMITTERS 15

namespace daidalos {

	/**
	* Constructor
	* @param parent The parent
	* @param node The mesh node
	*/
	CZombieEntity::CZombieEntity(daidalosgameengine::ILevel* parent, daidalosengine::CAnimatedMeshSceneNode* node) : IEnemyEntity(parent, node) {

		this->SetGravity(daidalosengine::CVector3F(0, GRAVITY, 0));
		this->SetCollisionShape(new daidalosengine::CEllipseF(this->GetMeshNode()->GetPosition() + daidalosengine::CVector3F(0, COLLISION_SHAPE_HEIGHT, 0),
		                                                      daidalosengine::CVector2F(COLLISION_SHAPE_WIDTH, COLLISION_SHAPE_HEIGHT)));

		// Read the waypoints
		const std::string waypointsString = this->GetMeshNode()->GetAttribute("patrol_waypoints");
		std::vector<daidalosengine::CVector3F> wayPoints;
		if (!waypointsString.empty()) {
			daidalosengine::CTokenizer tokenizer;
			tokenizer.AddLine(waypointsString);
			if (tokenizer.Tokenize("|")) {
				for (auto it = tokenizer.GetTokens().begin(); it != tokenizer.GetTokens().end(); ++it) {
					wayPoints.emplace_back(this->GetMeshNode()->GetAbsolutePosition() + daidalosengine::CVector3F::GetFrom(*it));
				}
			}
		}

		this->GetActionStates().push_back(std::unique_ptr<daidalosgameengine::IEnemyState>(
			std::make_unique<CEnemyStatePatrol>(0, ANIMATION_WALK_START, ANIMATION_WALK_END, ANIMATION_WALK_SPEED, WALK_SPEED, ROTATION_SPEED, wayPoints)));
		this->GetActionStates().push_back(
			std::unique_ptr<daidalosgameengine::IEnemyState
			>(std::make_unique<CZombieStateWait>(1, ANIMATION_WAIT_START, ANIMATION_WAIT_END, ANIMATION_WAIT_SPEED, WAIT_TIME)));
		this->GetActionStates().push_back(std::unique_ptr<daidalosgameengine::IEnemyState>(std::make_unique<CZombieStateFollow>(
			2, ANIMATION_WALK_START, ANIMATION_WALK_END, ANIMATION_WALK_SPEED, this->GetParent()->GetPlayer(), WALK_SPEED, ROTATION_SPEED, ATTACK_RANGE,
			FOLLOW_SEARCH_TIME)));
		this->GetActionStates().push_back(std::unique_ptr<daidalosgameengine::IEnemyState>(std::make_unique<CEnemyStateAttack>(
			3, ANIMATION_ATTACK_START, ANIMATION_ATTACK_END, ANIMATION_ATTACK_SPEED, this->GetParent()->GetPlayer(), ATTACK_POWER, ATTACK_RANGE, ATTACK_DELAY)));
		this->GetActionStates().push_back(
			std::unique_ptr<daidalosgameengine::IEnemyState>(std::make_unique<CEnemyStateDie>(99, ANIMATION_DIE_START, ANIMATION_DIE_END, ANIMATION_DIE_SPEED)));

		// Link the wait state with the patrol interruption callback
		this->GetActionStates().front()->AddListener(this->GetActionStates()[1].get());

		this->SetCurrentActionState(this->GetActionStates().front().get());
		this->SetVisiblePosition(CZombieEntity::GetCollisionShape()->GetPosition());

		// Change the default camera settings
		this->GetCameraNode()->SetAngle(daidalosengine::DegreeToRadian(VIEW_ANGLE));
		this->GetCameraNode()->SetPosition(daidalosengine::CVector3F(0, 4, 0));
		this->GetCameraNode()->SetFarDistance(VIEW_DISTANCE);

		// Initialize the impact particle emitters
		for (int i = 0; i < IMPACT_PARTICLE_EMITTERS; i++) {
			daidalosengine::CParticleEmitter* emitter = this->GetParent()->GetScene()->GetParticleSystem().InstanciateEmitter("default.dept", "fleshImpactEmitter");
			emitter->SetTextureId(1);
			emitter->SetActive(false);
			emitter->SetReusable(true);

			this->GetImpactParticleEmitters().push_back(emitter);
		}
	}

	/**
	* Update the enemy entity
	* @param time Elapsed time
	*/
	void CZombieEntity::Update(const float time) {
		IEnemyEntity::Update(time);

		this->GetMeshNode()->SetPosition(GetVisiblePosition() - daidalosengine::CVector3F(0, COLLISION_SHAPE_HEIGHT, 0));
	}

	/**
	* Determine the next active state
	*/
	void CZombieEntity::SelectNextActionState() {
		if (this->GetCurrentActionState() != nullptr) {

			daidalosgameengine::IEnemyState* nextState = nullptr;
			for (auto& it : this->GetActionStates()) {
				if (it->IsCandidateForNextAction(this)) {

					if (nextState == nullptr || it->GetPriority() > nextState->GetPriority()) {
						nextState = it.get();
					}
				}
			}

			if (this->GetCurrentActionState() != nextState) {
				// Get the next available state
				this->SetCurrentActionState(nextState);
				this->GetCurrentActionState()->Reset(this);
			}
		}
	}

	/**
	* Generate impact particles from a position
	* @param position The emitter position
	*/
	void CZombieEntity::GenerateImpactParticles(const daidalosengine::CVector3F& position) {
		bool freeParticleEmitterFound = false;
		for (unsigned int index = 0; index < GetImpactParticleEmitters().size() && !freeParticleEmitterFound; index++) {
			if (!GetImpactParticleEmitters()[index]->IsActive()) {
				freeParticleEmitterFound = true;
				GetImpactParticleEmitters()[index]->SetLife(0);
				GetImpactParticleEmitters()[index]->SetLifeMax(IMPACT_PARTICLE_LIFE);
				GetImpactParticleEmitters()[index]->SetActive(true);
				GetImpactParticleEmitters()[index]->SetPosition(position);
			}
		}
	}

	/**
	* Callback used to handle event when the entity fall and touch the ground
	* @param gravity The gravity value before touching the ground
	*/
	void CZombieEntity::OnEntityTouchedGround(const daidalosengine::CVector3F gravity) {
		// Nothing to do
	}
}
