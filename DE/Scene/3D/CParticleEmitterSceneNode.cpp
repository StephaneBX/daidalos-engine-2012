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

#include "../../Core/Logger/ILogger.h"
#include "../../Scene/3D/CParticleEmitterSceneNode.h"

namespace DE {
	/**
	* Constructor
	* @param scene The associated scene
	* @param parent The parent
	* @param name The node name
	* @param emitter The emitter
	*/
	CParticleEmitterSceneNode::CParticleEmitterSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, CParticleEmitter * emitter) : ISceneNode(scene, parent, name) {
		this->m_emitter = emitter;
		this->m_type = SCENE_NODE_PARTICLE_EMITTER;
	}

	CParticleEmitterSceneNode::~CParticleEmitterSceneNode() {

	}

	/**
	* Set the particle emitter active state
	* @param active The active state
	*/
	void CParticleEmitterSceneNode::SetActive(const bool active) {
		LOG_TRACE();

		this->m_emitter->SetActive(active);
	}

	/**
	* Set the particle emitter texture id
	* @param textureId The texture id
	*/
	void CParticleEmitterSceneNode::SetTextureId(const int textureId) {
		this->m_emitter->SetTextureId(textureId);
	}

	/**
	* Update the absolute position
	* @param time The elapsed time
	*/
	void CParticleEmitterSceneNode::Update(const float time) {
		LOG_TRACE();

		ISceneNode::Update(time);

		// Update the emitter position
		this->m_emitter->SetPosition(GetAbsolutePosition());
	}

	/**
	* Set the template name
	* @param templateName The template name
	*/
	void CParticleEmitterSceneNode::SetTemplateName(const std::string & templateName) {
		LOG_TRACE();

		this->m_templateName = templateName;
	}

	/**
	* Set the emitter name
	* @param emitterName The emitter name
	*/
	void CParticleEmitterSceneNode::SetEmitterName(const std::string & emitterName) {
		LOG_TRACE();

		this->m_emitterName = emitterName;
	}

	/**
	* Get the template name
	* @return The template name
	*/
	const std::string & CParticleEmitterSceneNode::GetTemplateName() const {
		LOG_TRACE();

		return this->m_templateName;
	}

	/**
	* Get the emitter name
	* @return The emitter name
	*/
	const std::string & CParticleEmitterSceneNode::GetEmitterName() const {
		LOG_TRACE();

		return this->m_emitterName;
	}

	/**
	* Get the particle emitter
	* @return The particle emitter
	*/
	CParticleEmitter * CParticleEmitterSceneNode::GetEmitter() const {
		LOG_TRACE();

		return this->m_emitter;
	}
}
