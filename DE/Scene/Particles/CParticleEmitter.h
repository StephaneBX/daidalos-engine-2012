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

#ifndef __CPARTICLEEMITTER_H
#define __CPARTICLEEMITTER_H

#include <vector>
#include <memory>
#include "../../Core/Utility/CVector3.h"
#include "../../Scene/3D/CSphere.h"
#include "../../Scene/Particles/IParticleInitializer.h"
#include "../../Scene/Particles/IParticleUpdater.h"

#include "../../Scene/Particles/Initializers/CBoxPositionParticleInitializer.h"
#include "../../Scene/Particles/Initializers/CColorParticleInitializer.h"
#include "../../Scene/Particles/Initializers/CLifeParticleInitializer.h"
#include "../../Scene/Particles/Initializers/CLinearVelocityParticleInitializer.h"
#include "../../Scene/Particles/Initializers/CRadiusPositionParticleInitializer.h"
#include "../../Scene/Particles/Initializers/CRotationParticleInitializer.h"
#include "../../Scene/Particles/Initializers/CSizeParticleInitializer.h"
#include "../../Scene/Particles/Initializers/CSphereVelocityParticleInitializer.h"

#include "../../Scene/Particles/Updaters/CColorParticleUpdater.h"
#include "../../Scene/Particles/Updaters/CGravityParticleUpdater.h"
#include "../../Scene/Particles/Updaters/CLifeParticleUpdater.h"
#include "../../Scene/Particles/Updaters/CRotationParticleUpdater.h"
#include "../../Scene/Particles/Updaters/CSizeParticleUpdater.h"
#include "../../Scene/Particles/Updaters/CVelocityParticleUpdater.h"

namespace DE {
	/**
	* A particle emitter
	*/
	class CParticleEmitter {
	protected:
		bool m_active;
		bool m_reusable;
		int m_textureId;
		int m_blendMode;
		float m_life;
		float m_lifeMax;
		float m_emissionRate;
		float m_pendingTime;
		CVector3F m_position;
		CSphereF m_visibilityShape;
		std::vector<int> m_particleIndex;
		std::vector<std::unique_ptr<IParticleInitializer> > m_initializers;
		std::vector<std::unique_ptr<IParticleUpdater> > m_updaters;

	public:
		/**
		* The constructor
		*/
		CParticleEmitter();

		/**
		* Set the active state
		* @param active If the emitter is active
		*/
		void SetActive(const bool active);

		/**
		* Set if it can be reused. Will not be automatically removed by the particle system.
		* @param reusable If the emitter is reusable
		*/
		void SetReusable(const bool reusable);

		/**
		* Set the texture id
		* @param textureId The texture id
		*/
		void SetTextureId(const int textureId);

		/**
		* Set the blend mode
		* @param blendMode The blend mode
		*/
		void SetBlendMode(const int blendMode);

		/**
		* Set the life
		* @param life The life
		*/
		void SetLife(const float life);

		/**
		* Set the maximum life
		* @param lifeMax The maximum life
		*/
		void SetLifeMax(const float maxLife);

		/**
		* Set the emission rate (particles per second)
		* @param emissionRate The emission rate
		*/
		void SetEmissionRate(const float emissionRate);

		/**
		* Set the absolute position
		* @param position The position
		*/
		void SetPosition(const CVector3F & position);

		/**
		* Add an initializer to the emitter
		* @param initializer The initializer to add
		*/
		void AddInitializer(IParticleInitializer * initializer);

		/**
		* Add an updater to the emitter
		* @param updater The updater to add
		*/
		void AddUpdater(IParticleUpdater * updater);

		/**
		* Update the emitter
		* @param time the elapsed time
		* @param buffer the particle buffer
		*/
		void Update(const float time, CMemoryAllocator<CParticleEntity> & buffer);

		/**
		* Get the active state
		* @return The active state
		*/
		const bool IsActive() const;

		/**
		* If the emitter is reusable
		* @return If the emitter is reusable
		*/
		const bool IsReusable() const;

		/**
		* Get the texture id
		* @return The texture id
		*/
		const int GetTextureId() const;

		/**
		* Get the blend mode
		* @return The blend mode
		*/
		const int GetBlendMode() const;

		/**
		* Get the life
		* @return The life
		*/
		const float GetLife() const;

		/**
		* Get the  maximum life
		* @return The maximum life
		*/
		const float GetLifeMax() const;

		/**
		* Get the emission rate
		* @return The emission rate
		*/
		const float GetEmissionRate() const;

		/**
		* Get the emitter visibility sphere
		* @return the visibility sphere
		*/
		CSphereF * GetVisibilitySphere();

		/**
		* Get the index of all active particles
		* @return the index vector
		*/
		const std::vector<int> & GetParticleIndex() const;

		/**
		* Get the particle initializers
		* @return The initializers
		*/
		const std::vector<std::unique_ptr<IParticleInitializer> > & GetInitializers() const;

		/**
		* Get the particle updaters
		* @return The particle updaters
		*/
		const std::vector<std::unique_ptr<IParticleUpdater> > & GetUpdaters() const;
	};
}

#endif
