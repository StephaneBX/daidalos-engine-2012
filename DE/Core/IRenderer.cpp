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

#include <stdio.h>
#include "../Core/Logger/ILogger.h"
#include "../Core/Core.h"
#include "../Core/IRenderer.h"

namespace DE {
	IRenderer * IRenderer::s_instance = nullptr;

	std::string IRenderer::s_shaderProgramParameters[] =
	{
		"worldMatrix",
		"rotationMatrix",
		"projMatrix",
		"viewProjMatrix",
		"billboardMatrix",

		"cameraPositionDistance",
		"viewDistance",
		"fogFalloff",

		"materialDiffuse",
		"materialAmbient",
		"materialSpecular",
		"materialEmission",
		"materialNs",
		"ambientColor",

		"shadowMapSize",
		"shadowCaster",

		"spotLightPosition",
		"spotLightDirection",
		"spotLightRRFT",
		"spotLightColor",
		"spotLightViewProj",

		"directionalLightPosition",
		"directionalLightDirection",
		"directionalLightColor",
		"directionalLightViewProj",

		"textColor",
		"translation",

		"skyboxSize",
		"skyboxPosition",
		"skyAmbientColor",

		"textureAtlasSize"
	};

	IRenderer::~IRenderer() {
		LOG_TRACE();
	}

	CBuffer IRenderer::CreateVertexBuffer(const unsigned long stride, const unsigned long count, const unsigned long flags, const void * data) const {
		LOG_TRACE();

		CBuffer buffer(CreateVB(count, stride, flags));
		if(data)
			buffer.Fill(data, stride, count);

		return buffer;
	}

	CBuffer IRenderer::CreateIndexBuffer(unsigned long count, unsigned long flags, const int * data) const {
		LOG_TRACE();

		CBuffer buffer(CreateIB(count, sizeof(int), flags));
		if(data)
			buffer.Fill(data, sizeof(int), count);

		return buffer;
	}

	IShaderProgramBase * IRenderer::GetCurrentShaderProgram() const {
		LOG_TRACE();

		return this->m_currentShaderProgram;
	}

	SScreenQuadElement * IRenderer::GetScreenQuad() {
		LOG_TRACE();

		return &this->m_screenQuadElement[0];
	}

	STexture2DFilterPolicy * IRenderer::GetDefaultTextureFilter() {
		LOG_TRACE();

		return &this->m_defaultTextureFilter;
	}

	const CBuffer & IRenderer::GetQuadIB() const {
		LOG_TRACE();

		return this->m_quadIB;
	}

	const CVector2I & IRenderer::GetResolution() const {
		LOG_TRACE();

		return this->m_resolution;
	}

	const CVector2I & IRenderer::GetInternalResolution() const {
		LOG_TRACE();

		return this->m_internalResolution;
	}

	const SScreenQuadElement & IRenderer::GetScreenQuadElement(EScreenQuadPoint point) const {
		LOG_TRACE();

		return this->m_screenQuadElement[point];
	}


	void IRenderer::SetDisplayBounds(const CRectangleI & rect) {
		LOG_TRACE();

		this->m_displayBounds = rect;
	}

	void IRenderer::SetResolution(const CVector2I & resolution) {
		LOG_TRACE();

		this->m_resolution = resolution;
	}

	const bool IRenderer::HasCapability(ECapability capability) const {
		LOG_TRACE();

		return this->m_capabilities.at(capability);
	}

	void IRenderer::SetRenderer(IRenderer * renderer) {
		LOG_TRACE();

		s_instance = renderer;
	}

	void IRenderer::Kill() {
		LOG_TRACE();

		SAFE_DELETE(s_instance);
	}

	IRenderer * IRenderer::GetRenderer() {
		LOG_TRACE();

		return s_instance;
	}

	const std::string & IRenderer::GetShaderProgramParameterString(EShaderProgramParameter parameter) {
		LOG_TRACE();

		return s_shaderProgramParameters[parameter];
	}

	IRenderer::IRenderer(const bool verticalSynchro, const int anisotropicFiltering, const int antialiasing) {
		LOG_TRACE();

		this->m_currentShaderProgram = nullptr;

		this->m_verticalSynchro = verticalSynchro;
		this->m_anisotropicFiltering = anisotropicFiltering;
		this->m_antialiasing = antialiasing;

		this->m_defaultTextureFilter.minFilter = DE::TEXTURE_FILTER_TYPE_ANISOTROPIC;
		this->m_defaultTextureFilter.magFilter = DE::TEXTURE_FILTER_TYPE_LINEAR;
		this->m_defaultTextureFilter.mipFilter = DE::TEXTURE_FILTER_TYPE_LINEAR;
		this->m_defaultTextureFilter.addressU = DE::TEXTURE_ADDRESS_WRAP;
		this->m_defaultTextureFilter.addressV = DE::TEXTURE_ADDRESS_WRAP;
		this->m_defaultTextureFilter.maxAnisotropy = anisotropicFiltering;
	}
}
