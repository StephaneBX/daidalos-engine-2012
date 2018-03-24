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
#include "../../Scene/2D/CButton.hpp"

namespace daidalosengine {
	/**
	* Constructor
	* @param texture The texture name
	*/
	CButton::CButton(const std::string & texture) {
		LOG_TRACE();

		this->m_enabled = true;
		this->m_clicked = false;
		this->m_imageInset = CInsetF(0, 0, 0, 0);
		SetChanged(true);

		this->m_title.SetVerticalAlignment(daidalosengine::TEXT_VERTICAL_ALIGNMENT_CENTERED);
		this->m_defaultTitleColor = daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F);
		this->m_clickedTitleColor = daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F);
		this->m_focusedTitleColor = daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F);
		SetDefaultTexture(texture);
		Initialize();
	}

	/**
	* Destructor
	*/
	CButton::~CButton() {
		LOG_TRACE();
	}

	/**
	* Initialize the content
	*/
	void CButton::Initialize() {
		AddSubview(&this->m_title);
	}

	/**
	* Add a button listener
	* @param listener The button listener
	*/
	void CButton::AddButtonListener(IButtonListener * listener) {
		LOG_TRACE();

		this->m_buttonListeners.insert(listener);
	}

	/**
	* Remove a button listener
	* @param listener The button listener
	*/
	void CButton::RemoveButtonListener(IButtonListener * listener) {
		LOG_TRACE();

		this->m_buttonListeners.erase(listener);
	}

	/**
	* Remove all the listeners
	*/
	void CButton::RemoveAllButtonListeners() {
		LOG_TRACE();

		this->m_buttonListeners.clear();
	}

	/**
	* Set the enabled state
	* @param enabled The enabled state
	*/
	void CButton::SetEnabled(const bool enabled) {
		LOG_TRACE();

		this->m_enabled = enabled;
	}

	/**
	* Set the image inset
	* The inset represents the parts of the button that will not be resized with the button
	* The values are between 0 and 1
	*/
	void CButton::SetImageInset(const CInsetF & inset) {
		LOG_TRACE();

		if(inset.GetLeft() >= 0.0F && inset.GetTop() >= 0.0F && inset.GetRight() >= 0.0F && inset.GetBottom() >= 0.0F) {
			this->m_imageInset = inset;
			SetChanged(true);
		}
	}

	/**
	* Set the default texture
	* @param texture The texture
	*/
	void CButton::SetDefaultTexture(const std::string & texture) {
		LOG_TRACE();

		if(texture != "") {
			this->m_defaultTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, GetTextureFilter());
		} else {
			this->m_defaultTexture = CTexture2D();
		}
	}

	/**
	* Set the clicked texture
	* @param texture The texture
	*/
	void CButton::SetClickedTexture(const std::string & texture) {
		LOG_TRACE();

		if(texture != "") {
			this->m_clickedTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, GetTextureFilter());
		} else {
			this->m_clickedTexture = CTexture2D();
		}
	}

	/**
	* Set the focused texture
	* @param texture The texture
	*/
	void CButton::SetFocusedTexture(const std::string & texture) {
		LOG_TRACE();

		if(texture != "") {
			this->m_focusedTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, GetTextureFilter());
		} else {
			this->m_focusedTexture = CTexture2D();
		}
	}

	/**
	* Set the default title color
	* @param color The color
	*/
	void CButton::SetDefaultTitleColor(const CColor & color) {
		LOG_TRACE();

		this->m_defaultTitleColor = color;
	}

	/**
	* Set the clicked title color
	* @param color The color
	*/
	void CButton::SetClickedTitleColor(const CColor & color) {
		LOG_TRACE();

		this->m_clickedTitleColor = color;
	}

	/**
	* Set the focused title color
	* @param color The color
	*/
	void CButton::SetFocusedTitleColor(const CColor & color) {
		LOG_TRACE();

		this->m_focusedTitleColor = color;
	}

	/**
	* Update the state and the geometry
	* @param time The elapsed time
	*/
	void CButton::Update(const float time) {
		LOG_TRACE();

		if (IsChanged()) {
			SetBounds(CRectangleI(GetPosition(), GetPreferredSize()));
			this->m_title.SetBounds(daidalosengine::CRectangleI(0, 0, GetPreferredSize().x, GetPreferredSize().y));

			GenerateGeometry();
			UpdateBuffer();
		}

		// Update the subviews
		IView::Update(time);
	}

	/**
	* Render the button
	*/
	void CButton::Render() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();

		if (GetDataSize() > 0) {

			// Handle the disabled gray color
			CColor color = CColor(1.0F, 1.0F, 1.0F, 1.0F);
			if (!this->m_enabled) {
				color = CColor(0.5F, 0.5F, 0.5F, 1.0F);
			}

			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), GetParent() != nullptr ? GetParent()->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), color);

			renderer->SetTexture(0, GetTextureToRender());
			renderer->SetTexture(1, nullptr);
			RenderPrimitives();
		}

		// Set the title color
		SetTitleColor();

		// Render the subviews
		RenderSubviews();
	}

	/**
	* Perform a hit test, checking if the point is inside in the the subview or in this view
	* The test is exclusive, only 1 view can succeed in the hit test
	* @param point The point to test in screen coordinate
	* @return True if the point is inside the view on inside one of its subviews
	*/
	const bool CButton::HitTest(const CVector2I & point) {
		LOG_TRACE();

		SetHit(IsInside(point));

		// Click canceled by the user
		if(this->m_clicked && !IsHit()) {
			this->m_clicked = false;
		}

		CInputDevice * inputDevice = CInputDevice::GetInstance();

		// Click start test
		if(inputDevice->IsActionActive(GUI_VIEW_LEFT_CLICK) && !this->m_clicked && IsHit()) {
			this->m_clicked = true;
		} else {
			// Click end test
			if(!inputDevice->IsActionActive(GUI_VIEW_LEFT_CLICK) && this->m_clicked && IsHit()) {
				this->m_clicked = false;

				// Call the listeners
				for(auto it = this->m_buttonListeners.begin(); it != this->m_buttonListeners.end(); ++it)
					(*it)->OnButtonClicked(this);
			}
		}

		return IsHit();
	}

	/**
	* Check if the button is enabled
	* @return True if the button is enabled, false otherwise
	*/
	const bool CButton::IsEnabled() const {
		LOG_TRACE();

		return this->m_enabled;
	}

	/**
	* Get the title
	* @return The title
	*/
	CLabel * CButton::GetTitleElement() {
		LOG_TRACE();

		return &this->m_title;
	}

	/**
	* Generate the geometry
	*/
	void CButton::GenerateGeometry() {
		LOG_TRACE();

		// Create the simple button if the inset is 0 or if no texture is given
		if ((this->m_imageInset.GetLeft() == 0.0F && this->m_imageInset.GetTop() == 0.0F && this->m_imageInset.GetRight() == 0.0F && this->m_imageInset.GetBottom() == 0.0F) || this->m_defaultTexture.GetTexture() == nullptr) {
			// Simple button with only on quad
			SetData(GenerateQuad(GetBoundingRectangle(), CRectangleF(0, 0, 1, 1)));
		} else {
			// Generate a 9 quad button
			std::vector<SGUIData> data;

			// Create the top left part
			CRectangleI topLeftPosition = CRectangleI(GetPosition().x, GetPosition().y, (int)(this->m_imageInset.GetLeft() * this->m_defaultTexture.GetSize().x), (int)(this->m_imageInset.GetTop() * this->m_defaultTexture.GetSize().y));
			CRectangleF topLeftCoordinates = CRectangleF(0, 0, this->m_imageInset.GetLeft(), this->m_imageInset.GetTop());
			std::vector<SGUIData> topLeft = GenerateQuad(topLeftPosition, topLeftCoordinates);
			data.insert(data.end(), topLeft.begin(), topLeft.end());

			// Create the bottom left part
			CRectangleI bottomLeftPosition = CRectangleI(GetPosition().x, GetPosition().y + (GetPreferredSize().y - (int)(this->m_imageInset.GetBottom() * this->m_defaultTexture.GetSize().y)), topLeftPosition.GetWidth(), (int)(this->m_imageInset.GetBottom() * this->m_defaultTexture.GetSize().y));
			CRectangleF bottomLeftCoordinates = CRectangleF(0, (1.0F - this->m_imageInset.GetBottom()), this->m_imageInset.GetLeft(), this->m_imageInset.GetBottom());
			std::vector<SGUIData> bottomLeft = GenerateQuad(bottomLeftPosition, bottomLeftCoordinates);
			data.insert(data.end(), bottomLeft.begin(), bottomLeft.end());

			// Create the middle left part
			CRectangleI middleLeftPosition = CRectangleI(topLeftPosition.GetLeft(), topLeftPosition.GetBottom(), topLeftPosition.GetWidth(), bottomLeftPosition.GetTop() - topLeftPosition.GetBottom());
			CRectangleF middleLeftCoordinates = CRectangleF(0, this->m_imageInset.GetTop(), this->m_imageInset.GetLeft(), 1 - this->m_imageInset.GetTop() - this->m_imageInset.GetBottom());
			std::vector<SGUIData> middleLeft = GenerateQuad(middleLeftPosition, middleLeftCoordinates);
			data.insert(data.end(), middleLeft.begin(), middleLeft.end());

			// Create the top right part
			CRectangleI topRightPosition = CRectangleI(GetPosition().x + (GetPreferredSize().x - (int)(this->m_imageInset.GetRight() * this->m_defaultTexture.GetSize().x)), GetPosition().y, (int)(this->m_imageInset.GetRight() * this->m_defaultTexture.GetSize().x), (int)(this->m_imageInset.GetTop() * this->m_defaultTexture.GetSize().y));
			CRectangleF topRightCoordinates = CRectangleF((1.0F - this->m_imageInset.GetRight()), 0, this->m_imageInset.GetRight(), this->m_imageInset.GetTop());
			std::vector<SGUIData> topRight = GenerateQuad(topRightPosition, topRightCoordinates);
			data.insert(data.end(), topRight.begin(), topRight.end());

			// Create the top part
			CRectangleI topPosition = CRectangleI(topLeftPosition.GetRight(), topLeftPosition.GetTop(), topRightPosition.GetLeft() - topLeftPosition.GetRight(), topLeftPosition.GetHeight());
			CRectangleF topCoordinates = CRectangleF(this->m_imageInset.GetLeft(), 0, 1.0F - this->m_imageInset.GetLeft() - this->m_imageInset.GetRight(), this->m_imageInset.GetTop());
			std::vector<SGUIData> top = GenerateQuad(topPosition, topCoordinates);
			data.insert(data.end(), top.begin(), top.end());

			// Create the bottom right part
			CRectangleI bottomRightPosition = CRectangleI(topRightPosition.GetLeft(), bottomLeftPosition.GetTop(), topRightPosition.GetWidth(), bottomLeftPosition.GetHeight());
			CRectangleF bottomRightCoordinates = CRectangleF(1.0F - this->m_imageInset.GetRight(), 1.0F - this->m_imageInset.GetBottom(), this->m_imageInset.GetRight(), this->m_imageInset.GetBottom());
			std::vector<SGUIData> bottomRight = GenerateQuad(bottomRightPosition, bottomRightCoordinates);
			data.insert(data.end(), bottomRight.begin(), bottomRight.end());

			// Create the middle right part
			CRectangleI middleRightPosition = CRectangleI(topRightPosition.GetLeft(), topRightPosition.GetBottom(), topRightPosition.GetWidth(), bottomRightPosition.GetTop() - topRightPosition.GetBottom());
			CRectangleF middleRightCoordinates = CRectangleF(1.0F - this->m_imageInset.GetRight(), this->m_imageInset.GetTop(), this->m_imageInset.GetRight(), 1.0F - this->m_imageInset.GetTop() - this->m_imageInset.GetBottom());
			std::vector<SGUIData> middleRight = GenerateQuad(middleRightPosition, middleRightCoordinates);
			data.insert(data.end(), middleRight.begin(), middleRight.end());

			// Create the bottom part
			CRectangleI bottomPosition = CRectangleI(bottomLeftPosition.GetRight(), bottomLeftPosition.GetTop(), bottomRightPosition.GetLeft() - bottomLeftPosition.GetRight(), bottomLeftPosition.GetHeight());
			CRectangleF bottomCoordinates = CRectangleF(this->m_imageInset.GetLeft(), 1.0F - this->m_imageInset.GetBottom(), 1.0F - this->m_imageInset.GetLeft() - this->m_imageInset.GetRight(), this->m_imageInset.GetBottom());
			std::vector<SGUIData> bottom = GenerateQuad(bottomPosition, bottomCoordinates);
			data.insert(data.end(), bottom.begin(), bottom.end());

			// Create the center part
			CRectangleI centerPosition = CRectangleI(topLeftPosition.GetRight(), topLeftPosition.GetBottom(), topRightPosition.GetLeft() - topLeftPosition.GetRight(), bottomLeftPosition.GetTop() - topLeftPosition.GetBottom());
			std::vector<SGUIData> center = GenerateQuad(centerPosition, CRectangleF(this->m_imageInset.GetLeft(), this->m_imageInset.GetTop(), 1.0F - this->m_imageInset.GetLeft() - this->m_imageInset.GetRight(), 1.0F - this->m_imageInset.GetTop() - this->m_imageInset.GetBottom()));
			data.insert(data.end(), center.begin(), center.end());

			SetData(data);
		}

		SetChanged(false);
	}

	/**
	* Set the title color
	*/
	void CButton::SetTitleColor() {
		LOG_TRACE();

		if(this->m_clicked) {
			this->m_title.SetColor(this->m_clickedTitleColor);
		} else {
			if(IsHit()) {
				this->m_title.SetColor(this->m_focusedTitleColor);
			} else {
				this->m_title.SetColor(this->m_defaultTitleColor);
			}
		}
	}

	/**
	* Get the texture to used for rendering
	*/
	ITexture2DBase * CButton::GetTextureToRender() {
		LOG_TRACE();

		if(this->m_clicked && this->m_clickedTexture.GetTexture() != nullptr) {
			return this->m_clickedTexture.GetTexture();
		} else {
			if(IsHit() && this->m_focusedTexture.GetTexture() != nullptr) {
				return this->m_focusedTexture.GetTexture();
			} else {
				return this->m_defaultTexture.GetTexture();
			}
		}
	}
}
