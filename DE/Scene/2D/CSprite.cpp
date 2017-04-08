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
#include "../../Core/IRenderer.h"
#include "../../Core/Memory/CExceptionAssert.h"
#include "../../Scene/2D//CSprite.h"

namespace DE {
#define PANEL_TEXTURE_NAME "white.png"

	CSprite::CSprite() : IView() {
		LOG_TRACE();

		this->m_changed = true;
		this->m_coordinates = CRectangleF(0, 0, 1, 1);
		this->m_backgroundColor = CColor(1.0f, 1.0f, 1.0f, 1.0f);
		this->m_texture.CreateFromFile(PANEL_TEXTURE_NAME, PIXEL_FORMAT_A8R8G8B8, IRenderer::GetRenderer()->GetDefaultBasicTextureFilter());
		this->m_rotation = 0;
	}

	CSprite::~CSprite() {

	}

	void CSprite::SetRotation(const float rotation) {
		LOG_TRACE();

		this->m_rotation = rotation;
		this->m_changed = true;
	}

	void CSprite::SetTexture(const std::string texture) {
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

		if(this->m_changed) {
			IRenderer * renderer = IRenderer::GetRenderer();

			this->m_data = GenerateQuad(this->m_boundingRectangle, this->m_coordinates, this->m_rotation);

			if(this->m_data.size() > 0) {
				// Create a new buffer if the current one is too small
				if((int)this->m_buffer.GetSize() < this->m_data.size() * sizeof(SGUIData))
					this->m_buffer = renderer->CreateVertexBuffer(sizeof(SGUIData), this->m_data.size(), BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);

				// Lock and copy the data
				unsigned char * lock = (unsigned char*)this->m_buffer.Lock(sizeof(SGUIData), 0, 0, LOCK_WRITEONLY);
				memcpy(lock, &this->m_data[0], this->m_data.size() * sizeof(SGUIData));
				this->m_buffer.Unlock();
			}
		}

		// Update the subviews
		IView::Update(time);
	}

	void CSprite::Render() {
		LOG_TRACE();

		if(this->m_data.size() > 0 && this->m_backgroundColor.GetAlpha() > 0) {
			IRenderer * renderer = IRenderer::GetRenderer();
			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), this->m_parent != nullptr ? this->m_parent->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), this->m_backgroundColor);

			renderer->SetTexture(0, this->m_texture.GetTexture());
			renderer->SetTexture(1, nullptr);
			renderer->SetVertexBuffer(0, this->m_buffer, sizeof(SGUIData));
			renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_data.size() / 3);
		}

		// Render the subviews
		IView::Render();
	}

	const float CSprite::GetRotation() const {
		return this->m_rotation;
	}
}
