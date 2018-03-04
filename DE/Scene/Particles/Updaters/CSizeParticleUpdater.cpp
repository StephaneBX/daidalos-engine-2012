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

#include "../../../Scene/Particles/Updaters/CSizeParticleUpdater.hpp"

namespace daidalosengine {

	/**
	* Constructor
	* @param sizeChanges The size changes definbitions
	*/
	CSizeParticleUpdater::CSizeParticleUpdater(const std::vector<SSizeParticleChangeUpdater> & sizeChanges) : m_sizeChanges(sizeChanges), IParticleUpdater(PARTICLE_UPDATER_TYPE_SIZE) {
		this->m_sizeChanges = sizeChanges;
	}

	/**
	* Destructor
	*/
	CSizeParticleUpdater::~CSizeParticleUpdater() {

	}

	/**
	* Update the particle
	* @param time The elapsed time
	* @param buffer The particle buffer
	* @param particles The particles id to initialize
	*/
	void CSizeParticleUpdater::Update(const float time, CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) {

		for(auto it = particles.begin(); it != particles.end(); it++) {
			CParticleEntity * particle = buffer.GetAt(*it);

			SSizeParticleChangeUpdater * currentSize = nullptr;
			SSizeParticleChangeUpdater * targetSize = nullptr;

			// Search for the right sizes
			for (auto it = this->m_sizeChanges.begin(); it != this->m_sizeChanges.end(); ++it) {

				// Current size
				if ((*it).time <= particle->GetLifeNormalized()) {
					currentSize = &(*it);
				}

				// Target size
				if (targetSize == nullptr && (*it).time > particle->GetLifeNormalized()) {
					targetSize = &(*it);
				}
			}

			// Set the interpolated size
			if (currentSize != nullptr && targetSize != nullptr) {
				float size = currentSize->size + (targetSize->size - currentSize->size) * (particle->GetLifeNormalized() - currentSize->time) / (targetSize->time - currentSize->time);
				particle->SetSize(size);
			} else if (currentSize != nullptr) {
				// Current size
				particle->SetSize(currentSize->size);
			}
		}
	}

	/**
	* Clone the class
	* @return The cloned class
	*/
	IClonable * CSizeParticleUpdater::Clone() {
		return new CSizeParticleUpdater(this->m_sizeChanges);
	}
}
