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
#include "../../Scene/2D/CCheckbox.hpp"

namespace daidalosengine {
	/**
	* Constructor
	*/
	CCheckbox::CCheckbox() {
		LOG_TRACE();

		Initialize();
	}

	/**
	* Destructor
	*/
	CCheckbox::~CCheckbox() {
		LOG_TRACE();

	}

	/**
	* Initialize the content
	*/
	void CCheckbox::Initialize() {
		this->m_enabled = true;
		this->m_checked = false;
		this->m_clicked = false;
		SetChanged(true);

		this->m_label.SetVerticalAlignment(TEXT_VERTICAL_ALIGNMENT_CENTERED);
		this->m_label.SetHorizontalAlignment(TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_labelInset = daidalosengine::CInsetI(5, 5, 5, 5);
		AddSubview(&this->m_label);
	}

	/**
	* Add a checkbox listener
	* @param listener The checkbox listener
	*/
	void CCheckbox::AddCheckboxListener(ICheckboxListener * listener) {
		LOG_TRACE();

		this->m_checkboxListeners.insert(listener);
	}

	/**
	* Remove a checkbox listener
	* @param listener The checkbox listener
	*/
	void CCheckbox::RemoveCheckboxListener(ICheckboxListener * listener) {
		LOG_TRACE();

		this->m_checkboxListeners.erase(listener);
	}

	/**
	* Remove all the listeners
	*/
	void CCheckbox::RemoveAllCheckboxListeners() {
		LOG_TRACE();

		this->m_checkboxListeners.clear();
	}

	void CCheckbox::SetEnabled(const bool enabled) {
		LOG_TRACE();

		this->m_enabled = enabled;
	}

	void CCheckbox::SetChecked(const bool checked) {
		LOG_TRACE();

		this->m_checked = checked;
	}

	void CCheckbox::SetLabelInset(const CInsetI & inset) {
		LOG_TRACE();

		this->m_labelInset = inset;
	}

	void CCheckbox::SetUncheckedTexture(const std::string & texture) {
		LOG_TRACE();

		if(!texture.empty()) {
			this->m_uncheckedTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, GetTextureFilter());
		} else {
			this->m_uncheckedTexture = CTexture2D();
		}
	}

	void CCheckbox::SetCheckedTexture(const std::string & texture) {
		LOG_TRACE();

		if(!texture.empty()) {
			this->m_checkedTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, GetTextureFilter());
		} else {
			this->m_checkedTexture = CTexture2D();
		}
	}

	void CCheckbox::SetClickedTexture(const std::string & texture) {
		LOG_TRACE();

		if(!texture.empty()) {
			this->m_clickedTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, GetTextureFilter());
		} else {
			this->m_clickedTexture = CTexture2D();
		}
	}

	void CCheckbox::Update(const float time) {
		LOG_TRACE();

		if (IsChanged()) {
			SetBounds(CRectangleI(GetPosition(), GetPreferredSize()));
			GenerateGeometry();
			UpdateBuffer();
		}

		// Update the subviews
		IView::Update(time);
	}

	void CCheckbox::Render() {
		LOG_TRACE();

		if (GetDataSize() > 0) {
			auto* renderer = IRenderer::GetRenderer();

			// Handle the disabled gray color
			auto color = CColor(1.0F, 1.0F, 1.0F, 1.0F);
			if (!this->m_enabled) {
				color = CColor(0.5F, 0.5F, 0.5F, 1.0F);
			}

			auto shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), GetParent() != nullptr ? GetParent()->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), color);

			renderer->SetTexture(0, GetTextureToRender());
			renderer->SetTexture(1, nullptr);
			RenderPrimitives();
		}

		// Render the subviews
		RenderSubviews();
	}

	bool CCheckbox::HitTest(const CVector2I & point) {
		LOG_TRACE();

		SetHit(IsInside(point));

		// Click canceled by the user
		if(this->m_clicked && !IsHit()) {
			this->m_clicked = false;
		}

		const auto inputDevice = CInputDevice::GetInstance();

		// Click start test
		if(inputDevice->IsActionActive(GUI_VIEW_LEFT_CLICK) && !this->m_clicked && IsHit()) {
			this->m_clicked = true;
		} else {
			// Click end test
			if(!inputDevice->IsActionActive(GUI_VIEW_LEFT_CLICK) && this->m_clicked && IsHit()) {
				this->m_clicked = false;
				this->m_checked = !this->m_checked;

				// Call the listeners
				for (auto checkboxListener : this->m_checkboxListeners) {
					if (this->m_checked) {
						checkboxListener->OnCheckboxChecked(this);
					} else {
						checkboxListener->OnCheckboxUnchecked(this);
					}
				}
			}
		}

		return IsHit();
	}

	bool CCheckbox::IsEnabled() const {
		LOG_TRACE();

		return this->m_enabled;
	}

	bool CCheckbox::IsChecked() const {
		LOG_TRACE();

		return this->m_checked;
	}

	CLabel * CCheckbox::GetLabelElement() {
		LOG_TRACE();

		return &this->m_label;
	}

	void CCheckbox::GenerateGeometry() {
		LOG_TRACE();

		auto positionRectangle = GetPositionRectangle();
		SetData(GenerateQuad(positionRectangle, CRectangleF(0, 0, 1, 1)));

		// Update the label position
		this->m_label.SetBounds(CRectangleI(positionRectangle.GetWidth() + this->m_labelInset.GetLeft(), this->m_labelInset.GetTop(), GetPreferredSize().x - positionRectangle.GetWidth() - this->m_labelInset.GetLeft() - this->m_labelInset.GetRight(), GetPreferredSize().y - this->m_labelInset.GetTop() - this->m_labelInset.GetBottom()));

		SetChanged(false);
	}

	CRectangleI CCheckbox::GetPositionRectangle() const {
		LOG_TRACE();

		CRectangleI positionRectangle;

		if(this->m_clicked && this->m_clickedTexture.GetTexture() != nullptr) {
			positionRectangle = CRectangleI(GetPosition().x, GetPosition().y + (GetPreferredSize().y - this->m_clickedTexture.GetSize().y) / 2, this->m_clickedTexture.GetSize().x, this->m_clickedTexture.GetSize().y);
		} else {
			if(this->m_checked && this->m_checkedTexture.GetTexture() != nullptr) {
				positionRectangle = CRectangleI(GetPosition().x, GetPosition().y + (GetPreferredSize().y - this->m_checkedTexture.GetSize().y) / 2, this->m_checkedTexture.GetSize().x, this->m_checkedTexture.GetSize().y);
			} else {
				positionRectangle = CRectangleI(GetPosition().x, GetPosition().y + (GetPreferredSize().y - this->m_uncheckedTexture.GetSize().y) / 2, this->m_uncheckedTexture.GetSize().x, this->m_uncheckedTexture.GetSize().y);
			}
		}
		return positionRectangle;
	}

	ITexture2DBase * CCheckbox::GetTextureToRender() const {
		LOG_TRACE();

		if(this->m_clicked && this->m_clickedTexture.GetTexture() != nullptr) {
			return this->m_clickedTexture.GetTexture();
		}

		if(this->m_checked && this->m_checkedTexture.GetTexture() != nullptr) {
			return this->m_checkedTexture.GetTexture();
		}

		if(this->m_uncheckedTexture.GetTexture() != nullptr) {
			return this->m_uncheckedTexture.GetTexture();
		}

		return nullptr;
	}
}
