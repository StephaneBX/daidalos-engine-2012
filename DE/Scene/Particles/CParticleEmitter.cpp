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

#include "../../Scene/Particles/CParticleEmitter.h"

namespace DE {
	/**
	* The constructor
	*/
	CParticleEmitter::CParticleEmitter() {
		this->m_pendingTime = 0;
	}

	/**
	* Set the texture id
	* @param textureId The texture id
	*/
	void CParticleEmitter::SetTextureId(const int textureId) {
		this->m_textureId = textureId;
	}

	/**
	* Set the life
	* @param life The life
	*/
	void CParticleEmitter::SetLife(const float life) {
		this->m_life = life;
	}

	/**
	* Set the maximum life
	* @param lifeMax The maximum life
	*/
	void CParticleEmitter::SetLifeMax(const float maxLife) {
		this->m_lifeMax = maxLife;
	}

	/**
	* Set the emission rate (particles per second)
	* @param emissionRate The emission rate
	*/
	void CParticleEmitter::SetEmissionRate(const float emissionRate) {
		this->m_emissionRate = emissionRate;
	}

	/**
	* Set the absolute position
	* @param position The position
	*/
	void CParticleEmitter::SetPosition(const CVector3F & position) {
		this->m_position = position;
	}

	/**
	* Add a generator to the emitter
	* @param generator The generator to add
	*/
	void CParticleEmitter::AddInitializer(const std::shared_ptr<IParticleInitializer> initializer) {
		this->m_initializers.push_back(initializer);
	}

	/**
	* Add an updater to the emitter
	* @param updater The updater to add
	*/
	void CParticleEmitter::AddUpdater(const std::shared_ptr<IParticleUpdater> updater) {
		this->m_updaters.push_back(updater);
	}

	/**
	* Update the emitter
	* @param time the elapsed time
	* @param buffer the particle buffer
	*/
	void CParticleEmitter::Update(const float time, CMemoryAllocator<CParticleEntity> & buffer) {

		// New particle emission
		this->m_pendingTime += time;
		float msPerParticle = 1000.0f / this->m_emissionRate;
		int numberOfNewParticles = (int)(this->m_pendingTime / msPerParticle);

		// We have new particles to create
		if(numberOfNewParticles > 0) {

			std::vector<int> newParticlesIndex;

			for(int i = 0;  i < numberOfNewParticles; i++) {
				// Get a free particle element from the buffer, if possible
				int newParticleId = buffer.New();
				if(newParticleId > -1) {
					this->m_particleIndex.push_back(newParticleId);
					newParticlesIndex.push_back(newParticleId);
				}
			}

			// Reduce the pending time
			this->m_pendingTime -= numberOfNewParticles * msPerParticle;
			if(this->m_pendingTime < 0) {
				this->m_pendingTime = 0;
			}

			// Call the initializers for each new particle
			for(auto it = this->m_initializers.begin(); it != this->m_initializers.end(); it++) {
				(*it)->initialize(buffer, newParticlesIndex);
			}
		}

		// Update the particles
		for(auto it = this->m_updaters.begin(); it != this->m_updaters.end(); it++) {
			(*it)->update(time, buffer, this->m_particleIndex);
		}

		// Update the visibility shape
		this->m_visibilityShape.SetPosition(this->m_position);
		float maxRadius = 0;
		for(auto it = this->m_particleIndex.begin(); it != this->m_particleIndex.end(); it++) {
			CParticleEntity & entity = buffer.GetReferenceAt((*it));
			maxRadius = std::max(maxRadius, (entity.GetPosition() - this->m_position).Length());
		}
		this->m_visibilityShape.SetRadius(maxRadius);
	}

	/**
	* Get the texture id
	* @return The texture id
	*/
	const int CParticleEmitter::GetTextureId() const {
		return this->m_textureId;
	}

	/**
	* Get the life
	* @return The life
	*/
	const float CParticleEmitter::GetLife() const {
		return this->m_life;
	}

	/**
	* Get the  maximum life
	* @return The maximum life
	*/
	const float CParticleEmitter::GetLifeMax() const {
		return this->m_lifeMax;
	}

	/**
	* Get the emission rate
	* @return The emission rate
	*/
	const float CParticleEmitter::GetEmissionRate() const {
		return this->m_emissionRate;
	}
}
