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

#include "../Gui/IGUIBase.h"

namespace DE
{
	void IGUIBase::SetChanged(const bool changed)
	{
		this->m_changed = changed;
	}

	void IGUIBase::SetVisible(const bool visible)
	{
		this->m_visible = visible;
	}

	void IGUIBase::SetPosition(const CVector2I & position)
	{
		this->m_position = position;
	}

	void IGUIBase::SetPreferredSize(const CVector2I & size)
	{
		this->m_preferredsize = size;
	}

	void IGUIBase::SetBounds(const CRectangleI & bounds)
	{
		SetPosition(CVector2I(bounds.GetLeft(), bounds.GetBottom()));
		SetPreferredSize(CVector2I(bounds.GetWidth(), bounds.GetHeight()));
	}
	
	const bool IGUIBase::IsChanged() const
	{
		return this->m_changed;
	}

	const bool IGUIBase::IsVisible() const
	{
		return this->m_visible;
	}

	const bool IGUIBase::IsInside(const CVector2I & point) const
	{
		return this->m_boundingRectangle.Intersect(point) == RECTANGLE_INTERSECTION_INSIDE;
	}

	const CVector2I & IGUIBase::GetPosition() const
	{
		return this->m_position;
	}

	const CVector2I & IGUIBase::GetPreferredSize() const
	{
		return this->m_preferredsize;
	}

	const CRectangleI & IGUIBase::GetBoundingRectangle() const
	{
		return this->m_boundingRectangle;
	}

	IGUIBase::IGUIBase()
	{
		this->m_changed = false;
		this->m_visible = true;
	}

	IGUIBase::IGUIBase(const IGUIBase & copy)
	{
		*this = copy;
	}

	const IGUIBase & IGUIBase::operator=(const IGUIBase & copy)
	{
		this->m_changed = copy.m_changed;
		this->m_visible = copy.m_visible;
		this->m_position = copy.m_position;
		this->m_preferredsize = copy.m_preferredsize;
		this->m_boundingRectangle = copy.m_boundingRectangle;
		return *this;
	}
}
