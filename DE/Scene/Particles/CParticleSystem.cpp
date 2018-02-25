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

#include "../../Core/Logger/ILogger.hpp"
#include "../../Core/Utility/CArraySorter.hpp"
#include "../../Scene/Particles/Updaters/CGravityParticleUpdater.hpp"
#include "../../Scene/Particles/CParticleSystem.hpp"

namespace daidalosengine {

	/**
	* The constructor
	*/
	CParticleSystem::CParticleSystem() {
		LOG_TRACE();

		this->m_renderer = IRenderer::GetRenderer();
		
		// Create the particle vertex declaration used by both particle vertex buffers
		daidalosengine::SDeclarationElement particleDecl[] = {
			{ 0, daidalosengine::DECLARATION_USAGE_POSITION0, daidalosengine::DECLARATION_TYPE_FLOAT3 },
			{ 0, daidalosengine::DECLARATION_USAGE_TEXCOORD0, daidalosengine::DECLARATION_TYPE_FLOAT2 },
			{ 1, daidalosengine::DECLARATION_USAGE_POSITION1, daidalosengine::DECLARATION_TYPE_FLOAT4 },
			{ 1, daidalosengine::DECLARATION_USAGE_COLOR0, daidalosengine::DECLARATION_TYPE_FLOAT4 },
			{ 1, daidalosengine::DECLARATION_USAGE_POSITION2, daidalosengine::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_particleVD = this->m_renderer->CreateVertexDeclaration(particleDecl);

		// Create the particle vertex buffer
		SParticleElement pe[] = {
			{ CVector3F(0.5F, -0.5F, 0), CVector2F(1, 1) },
			{ CVector3F(-0.5F, -0.5F, 0), CVector2F(0, 1) },
			{ CVector3F(-0.5F, 0.5F, 0), CVector2F(0, 0) },
			{ CVector3F(0.5F, 0.5F, 0), CVector2F(1, 0) }
		};
		this->m_particleVB = this->m_renderer->CreateVertexBuffer(sizeof(SParticleElement), 4, BUFFER_DYNAMIC | BUFFER_WRITEONLY, pe);

		// Create the particle index buffer
		int indices[] = { 0, 1, 2, 2, 3, 0 };
		this->m_particleIB = this->m_renderer->CreateIndexBuffer(6, 0, indices);

		// Load the shader
		this->m_shader.LoadFromFile("ForwardParticle");
	}

	/**
	* Set the system size
	* @param maxSize The maximum particle system size
	*/
	void CParticleSystem::InitializeSystem(const int maxSize) {
		LOG_TRACE();

		this->m_particleBuffer.Allocate(maxSize);

		// Create the sorting data
		this->m_particleIndex = std::unique_ptr<int>(new int[maxSize]);
		this->m_particleDistance = std::unique_ptr<float>(new float[maxSize]);

		// Create the particle instance vertex buffer
		this->m_particleInstanceVB = this->m_renderer->CreateVertexBuffer(sizeof(SParticleData), maxSize, BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);
	}

	/**
	* Load an emittersTemplate file
	* @param filename The filename
	*/
	void CParticleSystem::LoadEmittersTemplate(const std::string & filename) {
		LOG_TRACE();

		this->m_particleEmittersTemplates.LoadFromFile(filename);
	}

	/**
	* Set the atlas texture
	* @param filename The filename
	* @param size The number of columns and rows
	*/
	void CParticleSystem::SetAtlasTexture(const std::string & filename, const CVector2I & size) {
		LOG_TRACE();

		this->m_atlasTexture.CreateFromFile(filename, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		this->m_atlasSize = size;
	}

	/**
	* Set the system gravity
	* @param gravity The gravity
	*/
	void CParticleSystem::SetGravity(const CVector3F & gravity) {
		LOG_TRACE();

		this->m_gravity = gravity;
	}

	/**
	* Set the active camera
	* @param camera The active camera
	*/
	void CParticleSystem::SetActiveCamera(ICameraSceneNode * camera) {
		LOG_TRACE();

		this->m_activeCamera = camera;
	}

	/**
	* Destroy and remove an emitter
	*/
	void CParticleSystem::RemoveEmitter(CParticleEmitter * emitter) {
		LOG_TRACE();

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
		LOG_TRACE();

		for (auto it = this->m_emitters.begin(); it != this->m_emitters.end();) {

			(*it)->Update(time, this->m_particleBuffer);

			// Emitters with life max achieved are considered inactive
			if((*it)->GetLifeMax() > 0 && (*it)->GetLife() >= (*it)->GetLifeMax()) {
				(*it)->SetActive(false);
			}

			// Remove the dead emitters
			if(!(*it)->IsReusable() && !(*it)->IsActive() && (*it)->GetParticleIndex().size() == 0) {
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
		LOG_TRACE();

		// Set the render states
		this->m_renderer->SetRenderState(RENDER_STATE_TYPE_ALPHABLEND, true);
		
		this->m_renderer->SetBlendOperation(daidalosengine::RENDER_STATE_BLEND_OPERATION_ADD);
		this->m_renderer->SetRenderState(RENDER_STATE_TYPE_ALPHATEST, true);
		this->m_renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_DEPTHTEST, true);
		this->m_renderer->SetRenderState(RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, false);
		this->m_renderer->SetTexture(0, this->m_atlasTexture.GetTexture());

		// Prepare the billboard matrix
		CMatrix billboardMatrix = this->m_activeCamera->GetViewMatrix().GetBillboard();

		// Set the shader attributes
		this->m_renderer->SetShaderProgram(this->m_shader.GetShaderProgram());
		this->m_shader.SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_VIEWPROJ_MATRIX), this->m_activeCamera->GetViewProjMatrix());
		this->m_shader.SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_BILLBOARD_MATRIX), billboardMatrix);
		this->m_shader.SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_TEXTURE_ATLAS_SIZE), CVector2F((float)this->m_atlasSize.x, (float)this->m_atlasSize.y));
		this->m_shader.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_VIEW_DISTANCE), this->m_activeCamera->GetFarDistance());
		this->m_shader.SetParameter(daidalosengine::SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(daidalosengine::SHADERPROGRAM_PARAMETER_FOG_FALLOFF), 0.7F);

		// Steam states
		this->m_renderer->SetVertexDeclaration(this->m_particleVD.get());
		this->m_renderer->SetIndexBuffer(this->m_particleIB.GetBuffer(), 0);
		this->m_renderer->SetVertexBuffer(0, this->m_particleVB.GetBuffer(), sizeof(SParticleElement), 0, 4);
		this->m_renderer->SetStreamSourceFrequency(1, STREAM_SOURCE_TYPE_INSTANCE, 4);

		// Draw for each type of blend
		for(int i = RENDER_STATE_BLEND_FUNCTION_ZERO; i <= RENDER_STATE_BLEND_FUNCTION_INVSRCCOLOR2; i++) {
			FillParticlesData(i);
			if(this->m_particleSize > 0) {
				this->m_renderer->SetBlendFunction(daidalosengine::RENDER_STATE_BLEND_FUNCTION_SRCALPHA, (daidalosengine::ERenderStateBlendFunction)i);
				this->m_renderer->SetVertexBuffer(1, this->m_particleInstanceVB.GetBuffer(), sizeof(SParticleData), 0, this->m_particleSize);
				this->m_renderer->SetStreamSourceFrequency(0, STREAM_SOURCE_TYPE_INDEX, this->m_particleSize * 4);
				this->m_renderer->DrawIndexedPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
			}
		}

		// Cancel the render states
		this->m_renderer->SetRenderState(RENDER_STATE_TYPE_ALPHABLEND, false);
		this->m_renderer->SetRenderState(RENDER_STATE_TYPE_ALPHATEST, false);
		this->m_renderer->SetRenderState(RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, true);
		this->m_renderer->SetTexture(0, nullptr);

		// Cancel the stream states
		this->m_renderer->SetTexture(0, nullptr);
		this->m_renderer->SetVertexDeclaration(nullptr);
		this->m_renderer->SetIndexBuffer(nullptr, 0);
		this->m_renderer->SetVertexBuffer(0, nullptr, 0, 0, 0);
		this->m_renderer->SetVertexBuffer(1, nullptr, 0, 0);
		this->m_renderer->SetStreamSourceFrequency(0, STREAM_SOURCE_TYPE_DEFAULT, 1);
		this->m_renderer->SetStreamSourceFrequency(1, STREAM_SOURCE_TYPE_DEFAULT, 1);

		this->m_renderer->SetShaderProgram(nullptr);
	}

	/**
	* Get the system size
	* @return The system size
	*/
	const int CParticleSystem::GetSystemSize() const {
		LOG_TRACE();

		return this->m_particleBuffer.GetSize();
	}

	/**
	* Get the number of emitters
	* @return The number of emitters
	*/
	const int CParticleSystem::GetNumberOfEmitters() const {
		LOG_TRACE();

		return this->m_emitters.size();
	}

	/**
	* Get the atlas texture name
	* @return The atlas texture name
	*/
	const std::string CParticleSystem::GetAtlasTextureName() const {
		LOG_TRACE();

		return this->m_atlasTexture.GetTexture()->GetName();
	}

	/**
	* Get the atlas texture size
	* @return The atlas texture size
	*/
	const CVector2I & CParticleSystem::GetAtlasTextureSize() const {
		LOG_TRACE();

		return this->m_atlasSize;
	}

	/**
	* Get a vector containing the templates names
	* @return The templates names in a vector
	*/
	const std::vector<std::string> CParticleSystem::GetEmittersTemplatesName() const {
		LOG_TRACE();

		return this->m_particleEmittersTemplates.GetTemplatesName();
	}

	/**
	* Create a particle emitter and return it
	* @return The emitter
	*/
	CParticleEmitter * CParticleSystem::InstanciateEmitter() {
		LOG_TRACE();

		CParticleEmitter * emitter = new CParticleEmitter();

		// Add the gravity updater
		if (!emitter->IsIgnoreGravity()) {
			CGravityParticleUpdater * updater = new CGravityParticleUpdater(this->m_gravity);
			emitter->AddUpdater(updater);
		}

		this->m_emitters.push_back(std::unique_ptr<CParticleEmitter>(emitter));
		return emitter;
	}

	/**
	* Create a particle emitter from a template and return it
	* @param templateName The template name
	* @param emitterName The emitter name
	* @return The emitter
	*/
	CParticleEmitter * CParticleSystem::InstanciateEmitter(const std::string templateName, const std::string emitterName) {
		LOG_TRACE();

		CParticleEmitter * emitter = this->m_particleEmittersTemplates.InstanciateEmitter(templateName, emitterName);
		if(emitter == nullptr) {
			emitter = new CParticleEmitter();
		}

		// Add the gravity updater
		if (!emitter->IsIgnoreGravity()) {
			CGravityParticleUpdater * updater = new CGravityParticleUpdater(this->m_gravity);
			emitter->AddUpdater(updater);
		}

		this->m_emitters.push_back(std::unique_ptr<CParticleEmitter>(emitter));
		return emitter;
	}

	/**
	* Get all the live and visible particles for a given blend mode and store them sorted into the buffer.
	* @param blendMode The blend mode
	*/
	void CParticleSystem::FillParticlesData(const int blendMode) {
		LOG_TRACE();

		// Get the live particles for each visible emitter
		this->m_particleSize = 0;
		for(auto it = this->m_emitters.begin(); it != this->m_emitters.end(); it++) {
			// If the particle is not culled
			if((*it)->GetBlendMode() == blendMode && !this->m_activeCamera->FrustumCull((*it)->GetVisibilitySphere())) {
				// Get the particle index and the distance to camera
				for(auto it2 = (*it)->GetParticleIndex().begin(); it2 != (*it)->GetParticleIndex().end(); it2++) {
					this->m_particleIndex.get()[this->m_particleSize] = *it2;
					this->m_particleDistance.get()[this->m_particleSize] = (this->m_particleBuffer.GetAt((*it2))->GetPosition() - this->m_activeCamera->GetPosition()).Length();
					this->m_particleSize++;
				}
			}
		}

		if(this->m_particleSize > 0) {
			// Sort depending on the distance to the active camera
			CArraySorter<float> distanceSorter;
			distanceSorter.QuickSort(this->m_particleDistance.get(), this->m_particleIndex.get(), 0, this->m_particleSize - 1, ARRAY_SORTER_ORDER_DESC);

			// Fill the instance vertex buffer
			char * buffer = static_cast<char*>(this->m_particleInstanceVB.Lock(sizeof(SParticleData), this->m_particleSize, 0, 0));
			for(unsigned int i = 0; i < this->m_particleSize; i++) {
				auto particleData = this->m_particleBuffer.GetAt(this->m_particleIndex.get()[i])->GetData();
				memcpy(buffer + (sizeof(SParticleData) * i), particleData, sizeof(SParticleData));
			}
			this->m_particleInstanceVB.Unlock();
		}
	}
}
