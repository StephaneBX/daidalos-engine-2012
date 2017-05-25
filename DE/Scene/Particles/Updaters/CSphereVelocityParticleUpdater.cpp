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

#include "../../../Scene/Particles/Updaters/CSphereVelocityParticleUpdater.hpp"

namespace DE {

	/**
	* Constructor
	* @param sphereVelocityChanges The sphere velocity changes definition
	* @param damping The damping factor
	*/
	CSphereVelocityParticleUpdater::CSphereVelocityParticleUpdater(const std::vector<SSphereVelocityParticleChangeUpdater> & sphereVelocityChanges, const float damping) {
		this->m_type = PARTICLE_UPDATER_TYPE_VELOCITY;
		this->m_damping = damping;
		this->m_sphereVelocityChanges = sphereVelocityChanges;
	}

	/**
	* Destructor
	*/
	CSphereVelocityParticleUpdater::~CSphereVelocityParticleUpdater() {

	}

	/**
	* Update the particle
	* @param time The elapsed time
	* @param buffer The particle buffer
	* @param particles The particles id to initialize
	*/
	void CSphereVelocityParticleUpdater::Update(const float time, CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) {
		const float timeSecond = time / 1000.0f;

		for (auto it = particles.begin(); it != particles.end(); it++) {
			CParticleEntity * particle = buffer.GetAt((*it));

			// Next normalized life
			float nextNormalizedLife = (particle->GetLife() + time) / particle->GetLifeMax();

			// Check if a new linear velocity must be applied
			for (auto it = this->m_sphereVelocityChanges.begin(); it != this->m_sphereVelocityChanges.end(); ++it) {
				if ((*it).time >= particle->GetLifeNormalized() && (*it).time < nextNormalizedLife) {
					// A new linear velocity must be add to the current particle's velocity
					
					// Random direction
					CVector3F velocity(RandomNumber<float>(-1.0f, 1.0f), RandomNumber<float>(-1.0f, 1.0f), RandomNumber<float>(-1.0f, 1.0f));
					velocity = velocity.Normalize();

					// Apply the velocity value
					if ((*it).velocityMin != 0 && (*it).velocityMax != 0) {
						float velocityForce = RandomNumber<float>((*it).velocityMin, (*it).velocityMax);
						velocity *= velocityForce;

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
	const float CSphereVelocityParticleUpdater::GetDamping() const {
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
