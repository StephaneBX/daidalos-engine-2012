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

#include <utility>
#include "../../../Scene/Particles/Updaters/CLinearVelocityParticleUpdater.hpp"

namespace daidalosengine {

	/**
	* Constructor
	* @param linearVelocityChanges The linear velocity changes definition
	* @param damping The damping factor
	*/
	CLinearVelocityParticleUpdater::CLinearVelocityParticleUpdater(std::vector<SLinearVelocityParticleChangeUpdater> linearVelocityChanges, const float damping) : IParticleUpdater(PARTICLE_UPDATER_TYPE_VELOCITY), m_damping(damping), m_linearVelocityChanges(std::move(linearVelocityChanges)) {}

	/**
	* Destructor
	*/
	CLinearVelocityParticleUpdater::~CLinearVelocityParticleUpdater() = default;

	/**
	* Update the particle
	* @param time The elapsed time
	* @param buffer The particle buffer
	* @param particles The particles id to initialize
	*/
	void CLinearVelocityParticleUpdater::Update(const float time, CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) {
		const auto timeSecond = time / 1000.0F;

		for (auto it : particles) {
			auto particle = buffer.GetAt(it);

			// Next normalized life
			const auto nextNormalizedLife = (particle->GetLife() + time) / particle->GetLifeMax();

			// Check if a new linear velocity must be applied
			for (auto& linearVelocityChange : this->m_linearVelocityChanges) {
				if (linearVelocityChange.time >= particle->GetLifeNormalized() && linearVelocityChange.time < nextNormalizedLife) {
					// A new linear velocity must be add to the current particle's velocity

					// Create the altered direction
					auto velocity = linearVelocityChange.direction;
					if (linearVelocityChange.variationFactor > 0) {
						velocity.x *= (1.0F - RandomNumber<float>(0, linearVelocityChange.variationFactor));
						velocity.y *= (1.0F - RandomNumber<float>(0, linearVelocityChange.variationFactor));
						velocity.z *= (1.0F - RandomNumber<float>(0, linearVelocityChange.variationFactor));
					}

					// Apply the velocity value
					const auto velocityForce = RandomNumber<float>(linearVelocityChange.velocityMin, linearVelocityChange.velocityMax);
					velocity *= velocityForce;

					// Add the linear velocity
					particle->SetLinearVelocity(particle->GetLinearVelocity() + velocity);
				}
			}

			// Update the particle position
			particle->SetPosition(particle->GetPosition() + particle->GetLinearVelocity() * timeSecond);

			// Apply damping factor
			particle->SetLinearVelocity(particle->GetLinearVelocity() - (particle->GetLinearVelocity() * (this->m_damping * timeSecond)));
		}
	}

	/**
	* Get the damping
	* @return The damping
	*/
	float CLinearVelocityParticleUpdater::GetDamping() const {
		return this->m_damping;
	}

	/**
	* Get the linear velocity changes definition
	* @return The linear velocity changes definition
	*/
	const std::vector<SLinearVelocityParticleChangeUpdater> & CLinearVelocityParticleUpdater::GetLinearVelocityChanges() const {
		return this->m_linearVelocityChanges;
	}

	/**
	* Clone the class
	* @return The cloned class
	*/
	IClonable * CLinearVelocityParticleUpdater::Clone() {
		return new CLinearVelocityParticleUpdater(this->m_linearVelocityChanges, this->m_damping);
	}
}
