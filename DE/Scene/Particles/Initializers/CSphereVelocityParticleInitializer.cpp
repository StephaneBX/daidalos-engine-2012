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

#include "../../../Scene/Particles/Initializers/CSphereVelocityParticleInitializer.hpp"

namespace daidalosengine {

	/**
	* Constructor
	* @param velocityMin The velocity min
	* @param velocityMax The velocity max
	*/
	CSphereVelocityParticleInitializer::CSphereVelocityParticleInitializer(const float velocityMin, const float velocityMax) : IParticleInitializer(PARTICLE_INITIALIZER_TYPE_MINMAX_SPHERE_VELOCITY), m_velocityMin(velocityMin), m_velocityMax(velocityMax) {}

	/**
	* Destructor
	*/
	CSphereVelocityParticleInitializer::~CSphereVelocityParticleInitializer() = default;

	/**
	* Initialize the particle
	* @param buffer The particle buffer
	* @param particles The particles id to initialize
	*/
	void CSphereVelocityParticleInitializer::Initialize(CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) {

		for (auto it : particles) {
			auto particle = buffer.GetAt(it);

			// Get the normalized vector
			CVector3F velocity(RandomNumber<float>(-1.0F, 1.0F), RandomNumber<float>(-1.0F, 1.0F), RandomNumber<float>(-1.0F, 1.0F));
			velocity = velocity.Normalize();
			
			// Get the velocity value
			const auto velocityForce = RandomNumber<float>(this->m_velocityMin, this->m_velocityMax);
			velocity = velocity * velocityForce;

			particle->SetLinearVelocity(velocity);
		}
	}

	/**
	* Get the velocity min
	* @return The velocity min
	*/
	float CSphereVelocityParticleInitializer::GetVelocityMin() const {
		return this->m_velocityMin;
	}

	/**
	* Get the velocity max
	* @return The velocity max
	*/
	float CSphereVelocityParticleInitializer::GetVelocityMax() const {
		return this->m_velocityMax;
	}

	/**
	* Clone the class
	* @return The cloned class
	*/
	IClonable * CSphereVelocityParticleInitializer::Clone() {
		return new CSphereVelocityParticleInitializer(this->m_velocityMin, this->m_velocityMax);
	}
}

