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
#include "../../Core/IRenderer.hpp"
#include "../../Core/Memory/CExceptionAssert.hpp"
#include "../../Scene/2D//CSprite.hpp"

#define PANEL_TEXTURE_NAME "white.png"

namespace daidalosengine {
	CSprite::CSprite()  {
		LOG_TRACE();

		CSprite::SetChanged(true);
		this->m_coordinates = CRectangleF(0, 0, 1, 1);
		this->m_backgroundColor = CColor(1.0F, 1.0F, 1.0F, 1.0F);
		this->m_texture.CreateFromFile(PANEL_TEXTURE_NAME, PIXEL_FORMAT_A8R8G8B8, IRenderer::GetRenderer()->GetDefaultBasicTextureFilter());
		this->m_rotation = 0;
	}

	CSprite::~CSprite() = default;

	/**
	* Initialize the content
	*/
	void CSprite::Initialize() {
		// Nothing to do
	}

	void CSprite::SetRotation(const float rotation) {
		LOG_TRACE();

		this->m_rotation = rotation;
		SetChanged(true);
	}

	void CSprite::SetTexture(const std::string& texture) {
		LOG_TRACE();

		this->m_texture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, IRenderer::GetRenderer()->GetDefaultBasicTextureFilter());
	}

	void CSprite::SetCoordinates(const CRectangleF & coordinates) {
		LOG_TRACE();

		this->m_coordinates = coordinates;
	}

	void CSprite::SetBackgroundColor(const CColor & color) {
		LOG_TRACE();

		this->m_backgroundColor = color;
	}

	void CSprite::Update(const float time) {
		LOG_TRACE();

		if (IsChanged()) {
			SetData(GenerateQuad(GetBoundingRectangle(), this->m_coordinates, this->m_rotation));
			UpdateBuffer();
		}

		// Update the subviews
		IView::Update(time);
	}

	void CSprite::Render() {
		LOG_TRACE();

		if (GetDataSize() > 0 && this->m_backgroundColor.GetAlpha() > 0) {
			auto* renderer = IRenderer::GetRenderer();

			auto* shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), GetParent() != nullptr ? GetParent()->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), this->m_backgroundColor);

			renderer->SetTexture(0, this->m_texture.GetTexture());
			renderer->SetTexture(1, nullptr);
			RenderPrimitives();
		}

		// Render the subviews
		IView::Render();
	}

	float CSprite::GetRotation() const {
		return this->m_rotation;
	}
}
