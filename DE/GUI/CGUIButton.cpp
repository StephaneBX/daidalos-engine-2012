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

#include "../GUI/CGUIButton.h"

namespace DE
{
	CGUIButton::CGUIButton(const std::string & texture)
	{
		this->m_button = new IGUIButton(texture);
	}

	CGUIButton::~CGUIButton()
	{
		this->m_button->RemoveFromParentView();
		delete this->m_button;
	}

	void CGUIButton::SetVisible(const bool visible)
	{
		this->m_button->SetVisible(visible);
	}

	void CGUIButton::SetEnabled(const bool enabled)
	{
		this->m_button->SetEnabled(enabled);
	}

	void CGUIButton::SetPosition(const CVector2I & position)
	{
		this->m_button->SetPosition(position);
	}

	void CGUIButton::SetPreferredSize(const CVector2I & size)
	{
		this->m_button->SetPreferredSize(size);
	}

	void CGUIButton::SetBounds(const CRectangleI & bounds)
	{
		this->m_button->SetBounds(bounds);
	}

	void CGUIButton::SetImageInset(const CInsetF & inset)
	{
		this->m_button->SetImageInset(inset);
	}

	void CGUIButton::SetDefaultTexture(const std::string & texture)
	{
		this->m_button->SetDefaultTexture(texture);
	}

	void CGUIButton::SetClickedTexture(const std::string & texture)
	{
		this->m_button->SetClickedTexture(texture);
	}

	void CGUIButton::SetFocusedTexture(const std::string & texture)
	{
		this->m_button->SetFocusedTexture(texture);
	}

	const bool CGUIButton::IsChanged() const
	{
		return this->m_button->IsChanged();
	}

	const bool CGUIButton::IsVisible() const
	{
		return this->m_button->IsVisible();
	}

	const bool CGUIButton::IsEnabled() const
	{
		return this->m_button->IsEnabled();
	}

	const bool CGUIButton::IsInside(const CVector2I & point) const
	{
		return this->m_button->IsInside(point);
	}

	const CVector2I & CGUIButton::GetPosition() const
	{
		return this->m_button->GetPosition();
	}

	const CVector2I & CGUIButton::GetPreferredSize() const
	{
		return this->m_button->GetPreferredSize();
	}

	const CRectangleI & CGUIButton::GetBoundingRectangle() const
	{
		return this->m_button->GetBoundingRectangle();
	}

	const CGUIButton & CGUIButton::operator=(const CGUIButton & copy)
	{
		*this->m_button = *copy.GetGUIElement();
		return *this;
	}

	IGUIButton * CGUIButton::GetGUIElement() const
	{
		return this->m_button;
	}
}
