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

#include "../../../Scene/Particles/Initializers/CSizeParticleInitializer.h"

namespace DE {

	/**
	* Constructor
	* @param sizeMin The size min
	* @param sizeMax The size max
	*/
	CSizeParticleInitializer::CSizeParticleInitializer(float sizeMin, float sizeMax) {
		this->m_type = PARTICLE_INITIALIZER_TYPE_MINMAX_SIZE;
		this->m_sizeMin = sizeMin;
		this->m_sizeMax = sizeMax;
	}

	/**
	* Destructor
	*/
	CSizeParticleInitializer::~CSizeParticleInitializer() {

	}

	/**
	* Initialize the particle
	* @param buffer The particle buffer
	* @param particles The particles id to initialize
	*/
	void CSizeParticleInitializer::Initialize(CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) {

		for (auto it = particles.begin(); it != particles.end(); it++) {
			CParticleEntity * particle = buffer.GetAt((*it));
			particle->SetSizeMin(this->m_sizeMin);
			particle->SetSizeMax(this->m_sizeMax);
			particle->SetSize(RandomNumber<float>(this->m_sizeMin, this->m_sizeMax));
		}
	}

	/**
	* Get the size min
	* @return The size min
	*/
	const float CSizeParticleInitializer::GetSizeMin() {
		return this->m_sizeMin;
	}

	/**
	* Get the size max
	* @return The size max
	*/
	const float CSizeParticleInitializer::GetSizeMax() {
		return this->m_sizeMax;
	}

	/**
	* Clone the class
	* @return The cloned class
	*/
	IClonable * CSizeParticleInitializer::Clone() {
		return new CSizeParticleInitializer(this->m_sizeMin, this->m_sizeMax);
	}
}

