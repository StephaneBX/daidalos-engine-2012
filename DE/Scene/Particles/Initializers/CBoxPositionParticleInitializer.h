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

#ifndef __CBOXPOSITIONPARTICLEINITIALIZER_H
#define __CBOXPOSITIONPARTICLEINITIALIZER_H

#include "../../../Scene/Particles/IParticleInitializer.h"

namespace DE {
	/**
	* The particle position initializer class on a box area
	*/
	class CBoxPositionParticleInitializer : public IParticleInitializer {

	private:
		CVector3F m_positionMin;
		CVector3F m_positionMax;

	public:
		/**
		* Constructor
		* @param positionMin The min position
		* @param positionMax The max position
		*/
		CBoxPositionParticleInitializer(const CVector3F & positionMin, const CVector3F & positionMax);

		/**
		* Destructor
		*/
		virtual ~CBoxPositionParticleInitializer();

		/**
		* Initialize the particle
		* @param buffer The particle buffer
		* @param particles The particles id to initialize
		*/
		virtual void Initialize(CMemoryAllocator<CParticleEntity> & buffer, const std::vector<int> & particles) override;

		/**
		* Get the min position
		* @return The min position
		*/
		const CVector3F & GetPositionMin() const;

		/**
		* Get the max position
		* @return The max position
		*/
		const CVector3F & GetPositionMax() const;

		/**
		* Clone the class
		* @return The cloned class
		*/
		virtual IClonable * Clone() override;
	};
}

#endif
