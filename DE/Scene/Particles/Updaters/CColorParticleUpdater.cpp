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

#include "../../../Scene/Particles/Updaters/CColorParticleUpdater.h"

namespace DE {
	
	/**
	* Constructor
	* @param colorChanges The color changes definition
	*/
	CColorParticleUpdater::CColorParticleUpdater(const std::vector<SColorParticleChangeUpdater> & colorChanges) {
		this->m_type = PARTICLE_UPDATER_TYPE_COLOR;
		this->m_colorChanges = colorChanges;
	}

	/**
	* Destructor
	*/
	CColorParticleUpdater::~CColorParticleUpdater() {

	}

	/**
	* Update the particle
	* @param time The elapsed time
	* @param buffer The particle buffer
	* @param particles The particles id to initialize
	*/
	void CColorParticleUpdater::Update(const float time, CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) {

		for(auto it = particles.begin(); it != particles.end(); it++) {
			CParticleEntity * particle = buffer.GetAt((*it));

			SColorParticleChangeUpdater * currentColor = nullptr;
			SColorParticleChangeUpdater * targetColor = nullptr;

			// Search for the right colors
			for (auto it = this->m_colorChanges.begin(); it != this->m_colorChanges.end(); ++it) {

				// Current color
				if ((*it).time <= particle->GetLifeNormalized()) {
					currentColor = &(*it);
				}

				// Target color
				if (targetColor == nullptr && (*it).time > particle->GetLifeNormalized()) {
					targetColor = &(*it);
				}
			}

			// Set the interpolated color
			if (currentColor != nullptr && targetColor != nullptr) {
				CVector4F color = currentColor->color + (targetColor->color - currentColor->color) * (particle->GetLifeNormalized() - currentColor->time) / (targetColor->time - currentColor->time);
				particle->SetColor(CColor(color.x, color.y, color.z, color.w));
			} else if (currentColor != nullptr) {
				// Current color
				particle->SetColor(CColor(currentColor->color.x, currentColor->color.y, currentColor->color.z, currentColor->color.w));
			}
		}
	}

	/**
	* Clone the class
	* @return The cloned class
	*/
	IClonable * CColorParticleUpdater::Clone() {
		return new CColorParticleUpdater(this->m_colorChanges);
	}
}
