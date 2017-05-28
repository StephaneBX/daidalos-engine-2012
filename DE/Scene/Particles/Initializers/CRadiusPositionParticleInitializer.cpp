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

#include "../../../Scene/Particles/Initializers/CRadiusPositionParticleInitializer.h"

namespace DE {

	/**
	* Constructor
	* @param innerRadius The inner radius
	* @param outerRadius The outer radius
	*/
	CRadiusPositionParticleInitializer::CRadiusPositionParticleInitializer(const float innerRadius, const float outerRadius) {
		this->m_type = PARTICLE_INITIALIZER_TYPE_RADIUS_POSITION;
		this->m_innerRadius = innerRadius;
		this->m_outerRadius = outerRadius;
	}

	/**
	* Destructor
	*/
	CRadiusPositionParticleInitializer::~CRadiusPositionParticleInitializer() {

	}

	/**
	* Initialize the particle
	* @param buffer The particle buffer
	* @param particles The particles id to initialize
	*/
	void CRadiusPositionParticleInitializer::Initialize(CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) {

		for(auto it = particles.begin(); it != particles.end(); it++) {
			CParticleEntity * particle = buffer.GetAt((*it));

			// Randomized direction
			CVector3F direction = CVector3F(RandomNumber<float>(-1, 1), 0, RandomNumber<float>(-1, 1)).Normalize();

			// Keep the current position that hold the emitter's position
			particle->SetPosition(particle->GetPosition() + (direction * RandomNumber<float>(-this->m_innerRadius, this->m_outerRadius)));
		}
	}

	/**
	* Get the inner radius
	* @return The inner radius
	*/
	const float CRadiusPositionParticleInitializer::GetInnerRadius() const {
		return this->m_innerRadius;
	}

	/**
	* Get the outer radius
	* @return The outer radius
	*/
	const float CRadiusPositionParticleInitializer::GetOuterRadius() const {
		return this->m_outerRadius;
	}

	/**
	* Clone the class
	* @return The cloned class
	*/
	IClonable * CRadiusPositionParticleInitializer::Clone() {
		return new CRadiusPositionParticleInitializer(this->m_innerRadius, this->m_outerRadius);
	}
}