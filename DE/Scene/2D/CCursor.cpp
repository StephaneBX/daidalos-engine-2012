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
		SetChanged(true);

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
			SetPreferredSize(this->m_texture.GetSize());
			SetChanged(true);
		} else {
			this->m_texture = CTexture2D();
		}
	}

	void CCursor::Update(const float time) {
		LOG_TRACE();

		UpdatePosition();

		IRenderer * renderer = IRenderer::GetRenderer();

		SetData(GenerateQuad(this->GetBoundingRectangle(), CRectangleF(0, 0, 1, 1)));
		UpdateBuffer();

		// Update the subviews
		IView::Update(time);
	}

	void CCursor::Render() {
		LOG_TRACE();

		if (GetDataSize() > 0) {
			IRenderer * renderer = IRenderer::GetRenderer();

			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), GetParent() != nullptr ? GetParent()->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), CColor(1.0F, 1.0F, 1.0F, 1.0F));

			renderer->SetTexture(0, this->m_texture.GetTexture());
			renderer->SetTexture(1, nullptr);
			RenderPrimitives();
		}

		// Render the subviews
		IView::Render();
	}

	void CCursor::UpdatePosition() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();

		// Update the cursor position
		CInputDevice * inputDevice = CInputDevice::GetInstance();
		SetPosition(CVector2I(ClampNumber(GetPosition().x + inputDevice->GetMouseMovement().x, 0, renderer->GetResolution().x), ClampNumber(GetPosition().y + inputDevice->GetMouseMovement().y, 0, renderer->GetResolution().y)));
		SetBounds(CRectangleI(GetPosition(), GetPreferredSize()));
	}
}
