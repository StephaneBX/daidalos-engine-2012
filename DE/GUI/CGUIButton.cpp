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

#include "../Core/IRenderer.h"
#include "../Core/Memory/CExceptionAssert.h"
#include "../Core/CInputDevice.h"
#include "../GUI/CGUIButton.h"

namespace DE {
	CGUIButton::CGUIButton(const std::string & texture) {
		this->m_enabled = true;
		this->m_clicked = false;
		this->m_imageInset = CInsetF(0, 0, 0, 0);
		this->m_changed = true;

		this->m_title = new CGUILabel();
		AddSubview(this->m_title);
	}

	CGUIButton::~CGUIButton() {
		delete this->m_title;
	}

	void CGUIButton::AddButtonListener(IGUIButtonListener * listener) {
		this->m_buttonListeners.insert(listener);
	}

	void CGUIButton::RemoveButtonListener(IGUIButtonListener * listener) {
		this->m_buttonListeners.erase(listener);
	}

	void CGUIButton::RemoveAllButtonListeners() {
		this->m_buttonListeners.clear();
	}

	void CGUIButton::SetEnabled(const bool enabled) {
		this->m_enabled = enabled;
	}

	void CGUIButton::SetImageInset(const CInsetF & inset) {
		if(inset.GetLeft() >= 0.0f && inset.GetTop() >= 0.0f && inset.GetRight() >= 0.0f && inset.GetBottom() >= 0.0f) {
			this->m_imageInset = inset;
			this->m_changed = true;
		}
	}

	void CGUIButton::SetDefaultTexture(const std::string & texture) {
		if(texture != "") {
			this->m_defaultTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, &this->m_textureFilter);
		} else {
			this->m_defaultTexture = CTexture2D();
		}
	}

	void CGUIButton::SetClickedTexture(const std::string & texture) {
		if(texture != "") {
			this->m_clickedTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, &this->m_textureFilter);
		} else {
			this->m_clickedTexture = CTexture2D();
		}
	}

	void CGUIButton::SetFocusedTexture(const std::string & texture) {
		if(texture != "") {
			this->m_focusedTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, &this->m_textureFilter);
		} else {
			this->m_focusedTexture = CTexture2D();
		}
	}

	void CGUIButton::SetDefaultTitleColor(const CColor & color) {
		this->m_defaultTitleColor = color;
	}

	void CGUIButton::SetClickedTitleColor(const CColor & color) {
		this->m_clickedTitleColor = color;
	}

	void CGUIButton::SetFocusedTitleColor(const CColor & color) {
		this->m_focusedTitleColor = color;
	}

	void CGUIButton::Update() {
		if(this->m_changed) {
			this->m_boundingRectangle = CRectangleI(this->m_position.x, this->m_position.y, this->m_preferredsize.x, this->m_preferredsize.y);
			this->m_title->SetPreferredSize(this->m_preferredsize);

			IRenderer * renderer = IRenderer::GetRenderer();

			GenerateGeometry();

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
		IGUIView::Update();
	}

	void CGUIButton::Render() {
		if(this->m_data.size() > 0) {
			// Handle the disabled gray color
			CColor color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
			if(!this->m_enabled) {
				color = CColor(0.5f, 0.5f, 0.5f, 1.0f);
			}

			IRenderer * renderer = IRenderer::GetRenderer();
			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), this->m_parent != nullptr ? this->m_parent->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), color);

			renderer->SetTexture(0, GetTextureToRender());
			renderer->SetTexture(1, nullptr);
			renderer->SetVertexBuffer(0, this->m_buffer, sizeof(SGUIData));
			renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_data.size() / 3);
		}

		// Set the title color
		SetTitleColor();

		// Render the subviews
		IGUIView::Render();
	}

	const bool CGUIButton::HitTest(const CVector2I & point) {
		this->m_hitTest = IsInside(point);

		// Click canceled by the user
		if(this->m_clicked && !this->m_hitTest) {
			this->m_clicked = false;
		}

		CInputDevice * inputDevice = CInputDevice::GetInstance();

		// Click start test
		if(inputDevice->IsActionActive(GUI_VIEW_LEFT_CLICK) && !this->m_clicked && this->m_hitTest) {
			this->m_clicked = true;
		} else {
			// Click end test
			if(!inputDevice->IsActionActive(GUI_VIEW_LEFT_CLICK) && this->m_clicked && this->m_hitTest) {
				this->m_clicked = false;

				// Call the listeners
				for(auto it = this->m_buttonListeners.begin(); it != this->m_buttonListeners.end(); ++it)
					(*it)->OnButtonClicked(this);
			}
		}

		return this->m_hitTest;
	}

	const bool CGUIButton::IsEnabled() const {
		return this->m_enabled;
	}

	CGUILabel * CGUIButton::GetTitleElement() {
		return this->m_title;
	}

	void CGUIButton::GenerateGeometry() {
		this->m_data.clear();

		// Create the simple button if the inset is 0 or if no texture is given
		if((this->m_imageInset.GetLeft() == 0.0f && this->m_imageInset.GetTop() == 0.0f && this->m_imageInset.GetRight() == 0.0f && this->m_imageInset.GetBottom() == 0.0f) || this->m_defaultTexture.GetTexture() == nullptr) {
			// Simple button with only on quad
			this->m_data = GenerateQuad(GetBoundingRectangle(), CRectangleF(0, 0, 1, 1));
		} else {
			// Generate a 9 quad button

			// Create the top left part
			CRectangleI topLeftPosition = CRectangleI(this->m_position.x, this->m_position.y, (int)(this->m_imageInset.GetLeft() * this->m_defaultTexture.GetSize().x), (int)(this->m_imageInset.GetTop() * this->m_defaultTexture.GetSize().y));
			CRectangleF topLeftCoordinates = CRectangleF(0, 0, this->m_imageInset.GetLeft(), this->m_imageInset.GetTop());
			std::vector<SGUIData> topLeft = GenerateQuad(topLeftPosition, topLeftCoordinates);
			this->m_data.insert(this->m_data.end(), topLeft.begin(), topLeft.end());

			// Create the bottom left part
			CRectangleI bottomLeftPosition = CRectangleI(this->m_position.x, this->m_position.y + (this->m_preferredsize.y - (int)(this->m_imageInset.GetBottom() * this->m_defaultTexture.GetSize().y)), topLeftPosition.GetWidth(), (int)(this->m_imageInset.GetBottom() * this->m_defaultTexture.GetSize().y));
			CRectangleF bottomLeftCoordinates = CRectangleF(0, (1.0f - this->m_imageInset.GetBottom()), this->m_imageInset.GetLeft(), this->m_imageInset.GetBottom());
			std::vector<SGUIData> bottomLeft = GenerateQuad(bottomLeftPosition, bottomLeftCoordinates);
			this->m_data.insert(this->m_data.end(), bottomLeft.begin(), bottomLeft.end());

			// Create the middle left part
			CRectangleI middleLeftPosition = CRectangleI(topLeftPosition.GetLeft(), topLeftPosition.GetBottom(), topLeftPosition.GetWidth(), bottomLeftPosition.GetTop() - topLeftPosition.GetBottom());
			CRectangleF middleLeftCoordinates = CRectangleF(0, this->m_imageInset.GetTop(), this->m_imageInset.GetLeft(), 1 - this->m_imageInset.GetTop() - this->m_imageInset.GetBottom());
			std::vector<SGUIData> middleLeft = GenerateQuad(middleLeftPosition, middleLeftCoordinates);
			this->m_data.insert(this->m_data.end(), middleLeft.begin(), middleLeft.end());

			// Create the top right part
			CRectangleI topRightPosition = CRectangleI(this->m_position.x + (this->m_preferredsize.x - (int)(this->m_imageInset.GetRight() * this->m_defaultTexture.GetSize().x)), this->m_position.y, (int)(this->m_imageInset.GetRight() * this->m_defaultTexture.GetSize().x), (int)(this->m_imageInset.GetTop() * this->m_defaultTexture.GetSize().y));
			CRectangleF topRightCoordinates = CRectangleF((1.0f - this->m_imageInset.GetRight()), 0, this->m_imageInset.GetRight(), this->m_imageInset.GetTop());
			std::vector<SGUIData> topRight = GenerateQuad(topRightPosition, topRightCoordinates);
			this->m_data.insert(this->m_data.end(), topRight.begin(), topRight.end());

			// Create the top part
			CRectangleI topPosition = CRectangleI(topLeftPosition.GetRight(), topLeftPosition.GetTop(), topRightPosition.GetLeft() - topLeftPosition.GetRight(), topLeftPosition.GetHeight());
			CRectangleF topCoordinates = CRectangleF(this->m_imageInset.GetLeft(), 0, 1.0f - this->m_imageInset.GetLeft() - this->m_imageInset.GetRight(), this->m_imageInset.GetTop());
			std::vector<SGUIData> top = GenerateQuad(topPosition, topCoordinates);
			this->m_data.insert(this->m_data.end(), top.begin(), top.end());

			// Create the bottom right part
			CRectangleI bottomRightPosition = CRectangleI(topRightPosition.GetLeft(), bottomLeftPosition.GetTop(), topRightPosition.GetWidth(), bottomLeftPosition.GetHeight());
			CRectangleF bottomRightCoordinates = CRectangleF(1.0f - this->m_imageInset.GetRight(), 1.0f - this->m_imageInset.GetBottom(), this->m_imageInset.GetRight(), this->m_imageInset.GetBottom());
			std::vector<SGUIData> bottomRight = GenerateQuad(bottomRightPosition, bottomRightCoordinates);
			this->m_data.insert(this->m_data.end(), bottomRight.begin(), bottomRight.end());

			// Create the middle right part
			CRectangleI middleRightPosition = CRectangleI(topRightPosition.GetLeft(), topRightPosition.GetBottom(), topRightPosition.GetWidth(), bottomRightPosition.GetTop() - topRightPosition.GetBottom());
			CRectangleF middleRightCoordinates = CRectangleF(1.0f - this->m_imageInset.GetRight(), this->m_imageInset.GetTop(), this->m_imageInset.GetRight(), 1.0f - this->m_imageInset.GetTop() - this->m_imageInset.GetBottom());
			std::vector<SGUIData> middleRight = GenerateQuad(middleRightPosition, middleRightCoordinates);
			this->m_data.insert(this->m_data.end(), middleRight.begin(), middleRight.end());

			// Create the bottom part
			CRectangleI bottomPosition = CRectangleI(bottomLeftPosition.GetRight(), bottomLeftPosition.GetTop(), bottomRightPosition.GetLeft() - bottomLeftPosition.GetRight(), bottomLeftPosition.GetHeight());
			CRectangleF bottomCoordinates = CRectangleF(this->m_imageInset.GetLeft(), 1.0f - this->m_imageInset.GetBottom(), 1.0f - this->m_imageInset.GetLeft() - this->m_imageInset.GetRight(), this->m_imageInset.GetBottom());
			std::vector<SGUIData> bottom = GenerateQuad(bottomPosition, bottomCoordinates);
			this->m_data.insert(this->m_data.end(), bottom.begin(), bottom.end());

			// Create the center part
			CRectangleI centerPosition = CRectangleI(topLeftPosition.GetRight(), topLeftPosition.GetBottom(), topRightPosition.GetLeft() - topLeftPosition.GetRight(), bottomLeftPosition.GetTop() - topLeftPosition.GetBottom());
			std::vector<SGUIData> center = GenerateQuad(centerPosition, CRectangleF(this->m_imageInset.GetLeft(), this->m_imageInset.GetTop(), 1.0f - this->m_imageInset.GetLeft() - this->m_imageInset.GetRight(), 1.0f - this->m_imageInset.GetTop() - this->m_imageInset.GetBottom()));
			this->m_data.insert(this->m_data.end(), center.begin(), center.end());
		}

		this->m_changed = false;
	}

	void CGUIButton::SetTitleColor() {
		if(this->m_clicked) {
			this->m_title->SetColor(this->m_clickedTitleColor);
		} else {
			if(this->m_hitTest) {
				this->m_title->SetColor(this->m_focusedTitleColor);
			} else {
				this->m_title->SetColor(this->m_defaultTitleColor);
			}
		}
	}

	ITexture2DBase * CGUIButton::GetTextureToRender() {
		if(this->m_clicked && this->m_clickedTexture.GetTexture() != nullptr) {
			return this->m_clickedTexture.GetTexture();
		} else {
			if(this->m_hitTest && this->m_focusedTexture.GetTexture() != nullptr) {
				return this->m_focusedTexture.GetTexture();
			} else {
				return this->m_defaultTexture.GetTexture();
			}
		}
	}
}
