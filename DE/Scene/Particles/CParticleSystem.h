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

#ifndef __CPARTICLESYSTEM_H
#define __CPARTICLESYSTEM_H

#include <vector>
#include <memory>
#include "../../Medias/CTexture2D.h"
#include "../../Medias/CShaderProgram.h"
#include "../../Core/Utility/CVector2.h"
#include "../../Core/Memory/CMemoryAllocator.h"
#include "../../Scene/Particles/CParticleEntity.h"
#include "../../Scene/Particles/CParticleEmitter.h"
#include "../../Scene/3D/ICameraSceneNode.h"
#include "../../Core/IRenderer.h"

namespace DE {
	/**
	* Particle quad element
	*/
	struct SParticleElement {
		CVector3F vertex;
		CVector2F texcoord;
	};

	/**
	* The particle system class that handle the creation and the management of particles and emitters
	*/
	class CParticleSystem {
	protected:
		// Particle system data
		CTexture2D m_atlasTexture;
		CVector2I m_atlasSize;
		CMemoryAllocator<CParticleEntity> m_particleBuffer;
		std::vector<std::unique_ptr<CParticleEmitter> > m_emitters;

		// Data used to sort the particles
		std::unique_ptr<float> m_particleDistance;
		std::unique_ptr<int> m_particleIndex;
		unsigned int m_particleSize;

		// Rendering data
		IRenderer * m_renderer;
		DeclarationPtr m_particleVD;
		CBuffer m_particleVB;
		CBuffer m_particleIB;
		CBuffer m_particleInstanceVB;
		CShaderProgram m_shader;
		ICameraSceneNode * m_activeCamera;

	public:
		/**
		* The constructor
		*/
		CParticleSystem();

		/**
		* Set the system size
		* @param maxSize The maximum particle system size
		*/
		void InitializeSystem(const int maxSize);

		/**
		* Set the atlas texture
		* @param filename The filename
		* @param size The number of columns and rows
		*/
		void SetAtlasTexture(const std::string & filename, const CVector2I & size);

		/**
		* Set the active camera
		* @param camera The active camera
		*/
		void SetActiveCamera(ICameraSceneNode * camera);

		/**
		* Destroy and remove an emitter
		*/
		void RemoveEmitter(CParticleEmitter * emitter);

		/**
		* Update the particle system
		* @param time The elapsed time
		*/
		void Update(const float time);

		/**
		* Render the particle system
		*/
		void RenderAll();

		/**
		* Create a particle emitter and return it
		* @return The emitter
		*/
		CParticleEmitter * InstanciateEmitter();

	private:
		/**
		* Get all the live and visible particles and store them sorted into the buffer
		*/
		void FillParticlesData();
	};
}

#endif
