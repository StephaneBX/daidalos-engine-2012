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

#include "../../../Scene/Particles/Initializers/CBoxPositionParticleInitializer.hpp"

namespace DE {

	/**
	* Constructor
	* @param positionMin The min position
	* @param positionMax The max position
	*/
	CBoxPositionParticleInitializer::CBoxPositionParticleInitializer(const CVector3F & positionMin, const CVector3F & positionMax) {
		this->m_type = PARTICLE_INITIALIZER_TYPE_MINMAX_BOX_POSITION;
		this->m_positionMin = positionMin;
		this->m_positionMax = positionMax;
	}

	/**
	* Destructor
	*/
	CBoxPositionParticleInitializer::~CBoxPositionParticleInitializer() {

	}

	/**
	* Initialize the particle
	* @param buffer The particle buffer
	* @param particles The particles id to initialize
	*/
	void CBoxPositionParticleInitializer::Initialize(CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) {

		for(auto it = particles.begin(); it != particles.end(); it++) {
			CParticleEntity * particle = buffer.GetAt((*it));
			CVector3F position = CVector3F(RandomNumber<float>(this->m_positionMin.x, this->m_positionMax.x), RandomNumber<float>(this->m_positionMin.y, this->m_positionMax.y), RandomNumber<float>(this->m_positionMin.z, this->m_positionMax.z));
			// Keep the current position that hold the emitter's position
			particle->SetPosition(particle->GetPosition() + position);
		}
	}

	/**
	* Get the min position
	* @return The min position
	*/
	const CVector3F & CBoxPositionParticleInitializer::GetPositionMin() const {
		return this->m_positionMin;
	}

	/**
	* Get the max position
	* @return The max position
	*/
	const CVector3F & CBoxPositionParticleInitializer::GetPositionMax() const {
		return this->m_positionMax;
	}

	/**
	* Clone the class
	* @return The cloned class
	*/
	IClonable * CBoxPositionParticleInitializer::Clone() {
		return new CBoxPositionParticleInitializer(this->m_positionMin, this->m_positionMax);
	}
}