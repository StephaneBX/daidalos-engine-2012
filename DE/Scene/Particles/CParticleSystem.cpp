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

#include "../../Scene/Particles/CParticleSystem.h"

namespace DE {

	/**
	* The constructor
	*/
	CParticleSystem::CParticleSystem(const unsigned int maxSize) {
		this->m_particleBuffer.Allocate(maxSize);
		this->m_renderer = IRenderer::GetRenderer();
	}

	/**
	* Set the atlas texture
	* @param filename The filename
	* @param size The number of columns and rows
	*/
	void CParticleSystem::SetAtlasTexture(const std::string & filename, const CVector2I & size) {
		this->m_atlasTexture.CreateFromFile(filename, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		this->m_atlasSize = size;
	}

	/**
	* Destroy and remove an emitter
	*/
	void CParticleSystem::RemoveEmitter(CParticleEmitter * emitter) {

		bool found = false;
		for (auto it = this->m_emitters.begin(); it != this->m_emitters.end() && !found; it++) {
			if (emitter == (*it).get()) {
				this->m_emitters.erase(it);
				found = true;
			}
		}
	}

	/**
	* Update the particle system
	* @param time The elapsed time
	*/
	void CParticleSystem::Update(const float time) {

		for (auto it = this->m_emitters.begin(); it != this->m_emitters.end();) {
			(*it)->Update(time, this->m_particleBuffer);

			// Remove dead emitters
			if ((*it)->GetLife() >= (*it)->GetLifeMax()) {
				it = this->m_emitters.erase(it);
			} else {
				it++;
			}
		}
	}

	/**
	* Render the particle system
	*/
	void CParticleSystem::RenderAll() {

	}

	/**
	* Create a particle emitter and return it
	* @return The emitter
	*/
	CParticleEmitter * CParticleSystem::InstanciateEmitter() {
		CParticleEmitter * emitter = new CParticleEmitter();
		this->m_emitters.push_back(std::unique_ptr<CParticleEmitter>(emitter));
		return emitter;
	}
}
