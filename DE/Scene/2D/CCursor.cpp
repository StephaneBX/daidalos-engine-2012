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
#include "../../Core/CInputDevice.hpp"
#include "../../Scene/2D/CCursor.hpp"

namespace daidalosengine {
	CCursor::CCursor(const std::string & texture) {
		LOG_TRACE();

		Initialize();
	}

	CCursor::~CCursor() {

	}

	/**
	* Initialize the content
	*/
	void CCursor::Initialize() {
		this->m_changed = true;

		// Add the cursor actions
		CInputDevice * inputDevice = CInputDevice::GetInstance();
		SInputActionDeclaration cursorActions[] = {
			{ GUI_VIEW_LEFT_CLICK, -1 },
			{ GUI_VIEW_WHEEL_UP, -1 },
			{ GUI_VIEW_WHEEL_DOWN, -1 }
		};
		inputDevice->AddAction(cursorActions[0]);
		inputDevice->AddAction(cursorActions[1]);
		inputDevice->AddAction(cursorActions[2]);

		inputDevice->SetActionKey(GUI_VIEW_LEFT_CLICK, daidalosengine::INPUT_KEY_MOUSE_BUTTON0);
		inputDevice->SetActionKey(GUI_VIEW_WHEEL_UP, daidalosengine::INPUT_KEY_MOUSE_WHEELUP);
		inputDevice->SetActionKey(GUI_VIEW_WHEEL_DOWN, daidalosengine::INPUT_KEY_MOUSE_WHEELDOWN);
	}

	void CCursor::SetTexture(const std::string & texture) {
		LOG_TRACE();

		if(texture != "") {
			this->m_texture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, IRenderer::GetRenderer()->GetDefaultTextureFilter());
			this->m_preferredsize = this->m_texture.GetSize();
			this->m_changed = true;
		} else {
			this->m_texture = CTexture2D();
		}
	}

	void CCursor::Update(const float time) {
		LOG_TRACE();

		UpdatePosition();

		IRenderer * renderer = IRenderer::GetRenderer();

		this->m_data = GenerateQuad(this->m_boundingRectangle, CRectangleF(0, 0, 1, 1));

		if(this->m_data.size() > 0) {
			// Create a new buffer if the current one is too small
			if((int)this->m_buffer.GetSize() < this->m_data.size() * sizeof(SGUIData))
				this->m_buffer = renderer->CreateVertexBuffer(sizeof(SGUIData), this->m_data.size(), BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);

			// Lock and copy the data
			unsigned char * lock = (unsigned char*)this->m_buffer.Lock(sizeof(SGUIData), 0, 0, LOCK_WRITEONLY);
			memcpy(lock, &this->m_data[0], this->m_data.size() * sizeof(SGUIData));
			this->m_buffer.Unlock();
		}

		// Update the subviews
		IView::Update(time);
	}

	void CCursor::Render() {
		LOG_TRACE();

		if(this->m_data.size() > 0) {
			IRenderer * renderer = IRenderer::GetRenderer();

			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), this->m_parent != nullptr ? this->m_parent->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), CColor(1.0F, 1.0F, 1.0F, 1.0F));

			renderer->SetTexture(0, this->m_texture.GetTexture());
			renderer->SetTexture(1, nullptr);
			renderer->SetVertexBuffer(0, this->m_buffer.GetBuffer(), sizeof(SGUIData));
			renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_data.size() / 3);
		}

		// Render the subviews
		IView::Render();
	}

	void CCursor::UpdatePosition() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();

		// Update the cursor position
		CInputDevice * inputDevice = CInputDevice::GetInstance();
		this->m_position.x += inputDevice->GetMouseMovement().x;
		this->m_position.y += inputDevice->GetMouseMovement().y;

		this->m_position.x = ClampNumber(this->m_position.x, 0, renderer->GetResolution().x);
		this->m_position.y = ClampNumber(this->m_position.y, 0, renderer->GetResolution().y);

		this->m_boundingRectangle = CRectangleI(this->m_position.x, this->m_position.y, this->m_preferredsize.x, this->m_preferredsize.y);
	}
}
