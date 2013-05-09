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

#include "../Core/CGUIManager.h"
#include "../GUI/CGUIText.h"

namespace DE
{
	CGUIText::CGUIText(const std::string & text, const std::string & font)
	{
		this->m_text = new IGUITextBase(text, font);
		CGUIManager::GetInstance()->RegisterElement(this->m_text);
	}

	CGUIText::~CGUIText()
	{
		CGUIManager::GetInstance()->UnregisterElement(this->m_text);
		delete this->m_text;
	}

	void CGUIText::SetVisible(const bool visible)
	{
		this->m_text->SetVisible(visible);
	}

	void CGUIText::SetPosition(const CVector2I & position)
	{
		this->m_text->SetPosition(position);
	}

	void CGUIText::SetPreferredSize(const CVector2I & size)
	{
		this->m_text->SetPreferredSize(size);
	}

	void CGUIText::SetBounds(const CRectangleI & bounds)
	{
		this->m_text->SetBounds(bounds);
	}

	void CGUIText::SetFont(const std::string & font)
	{
		this->m_text->SetFont(font);
	}

	void CGUIText::SetText(const std::string & text)
	{
		this->m_text->SetText(text);
	}

	void CGUIText::SetHorizontalAlignment(ETextHorizontalAlignment alignment)
	{
		this->m_text->SetHorizontalAlignment(alignment);
	}

	void CGUIText::SetVerticalAlignment(ETextVerticalAlignment alignment)
	{
		this->m_text->SetVerticalAlignment(alignment);
	}

	void CGUIText::SetSize(const unsigned int size)
	{
		this->m_text->SetSize(size);
	}

	void CGUIText::SetColor(const CColor & color)
	{
		this->m_text->SetColor(color);
	}

	ETextHorizontalAlignment CGUIText::GetHorizontalAlignment() const
	{
		return this->m_text->GetHorizontalAlignment();
	}

	ETextVerticalAlignment CGUIText::GetVerticalAlignment() const
	{
		return this->m_text->GetVerticalAlignment();
	}

	const bool CGUIText::IsChanged() const
	{
		return this->m_text->IsChanged();
	}

	const bool CGUIText::IsVisible() const
	{
		return this->m_text->IsVisible();
	}

	const bool CGUIText::IsInside(const CVector2I & point) const
	{
		return this->m_text->IsInside(point);
	}

	const unsigned int CGUIText::GetSize() const
	{
		return this->m_text->GetSize();
	}

	const std::string & CGUIText::GetFontName() const
	{
		return this->m_text->GetFontName();
	}

	const std::string & CGUIText::GetText() const
	{
		return this->m_text->GetText();
	}

	const CVector2I & CGUIText::GetPosition() const
	{
		return this->m_text->GetPosition();
	}

	const CVector2I & CGUIText::GetPreferredSize() const
	{
		return this->m_text->GetPreferredSize();
	}

	const CRectangleI & CGUIText::GetBoundingRectangle() const
	{
		return this->m_text->GetBoundingRectangle();
	}

	const CColor & CGUIText::GetColor() const
	{
		return this->m_text->GetColor();
	}

	const CGUIText & CGUIText::operator=(const CGUIText & copy)
	{
		*this->m_text = *copy.GetGUIElement();
		return *this;
	}

	IGUITextBase * CGUIText::GetGUIElement() const
	{
		return this->m_text;
	}
}
