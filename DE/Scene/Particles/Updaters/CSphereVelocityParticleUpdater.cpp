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
#include "../../../Scene/Particles/Updaters/CSphereVelocityParticleUpdater.hpp"

namespace daidalosengine {

	/**
	* Constructor
	* @param sphereVelocityChanges The sphere velocity changes definition
	* @param damping The damping factor
	*/
	CSphereVelocityParticleUpdater::CSphereVelocityParticleUpdater(std::vector<SSphereVelocityParticleChangeUpdater> sphereVelocityChanges, const float damping) : IParticleUpdater(PARTICLE_UPDATER_TYPE_VELOCITY), m_damping(damping), m_sphereVelocityChanges(std::move(sphereVelocityChanges)) {}

	/**
	* Destructor
	*/
	CSphereVelocityParticleUpdater::~CSphereVelocityParticleUpdater() = default;

	/**
	* Update the particle
	* @param time The elapsed time
	* @param buffer The particle buffer
	* @param particles The particles id to initialize
	*/
	void CSphereVelocityParticleUpdater::Update(const float time, CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) {
		const auto timeSecond = time / 1000.0F;

		for (auto it : particles) {
			auto particle = buffer.GetAt(it);

			// Next normalized life
			const auto nextNormalizedLife = (particle->GetLife() + time) / particle->GetLifeMax();

			// Check if a new linear velocity must be applied
			for (auto& sphereVelocityChange : this->m_sphereVelocityChanges) {
				if (sphereVelocityChange.time >= particle->GetLifeNormalized() && sphereVelocityChange.time < nextNormalizedLife) {
					// A new linear velocity must be add to the current particle's velocity
					
					// Random direction
					CVector3F velocity(RandomNumber<float>(-1.0F, 1.0F), RandomNumber<float>(-1.0F, 1.0F), RandomNumber<float>(-1.0F, 1.0F));
					velocity = velocity.Normalize();

					// Apply the velocity value
					if (sphereVelocityChange.velocityMin != 0 && sphereVelocityChange.velocityMax != 0) {
						const auto velocityForce = RandomNumber<float>(sphereVelocityChange.velocityMin, sphereVelocityChange.velocityMax);
						velocity *= velocityForce * particle->GetMass();

						// Add the linear velocity
						particle->SetSphereVelocity(particle->GetSphereVelocity() + velocity);
					}
				}
			}

			// Update the particle position
			particle->SetPosition(particle->GetPosition() + particle->GetSphereVelocity() * timeSecond);

			// Apply damping factor
			particle->SetSphereVelocity(particle->GetSphereVelocity() - (particle->GetSphereVelocity() * (this->m_damping * timeSecond)));
		}
	}

	/**
	* Get the damping
	* @return The damping
	*/
	float CSphereVelocityParticleUpdater::GetDamping() const {
		return this->m_damping;
	}

	/**
	* Get the sphere velocity changes definition
	* @return The sphere velocity changes definition
	*/
	const std::vector<SSphereVelocityParticleChangeUpdater> & CSphereVelocityParticleUpdater::GetSphereVelocityChanges() const {
		return this->m_sphereVelocityChanges;
	}

	/**
	* Clone the class
	* @return The cloned class
	*/
	IClonable * CSphereVelocityParticleUpdater::Clone() {
		return new CSphereVelocityParticleUpdater(this->m_sphereVelocityChanges, this->m_damping);
	}
}
