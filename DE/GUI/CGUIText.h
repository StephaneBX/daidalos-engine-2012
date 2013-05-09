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

#ifndef __CGUITEXT_H
#define __CGUITEXT_H

#include "../GUI/IGUITextBase.h"

namespace DE
{
	/**
	* Gui text enclosing class
	*/
	class CGUIText
	{
	private:
		IGUITextBase * m_text;

	public:
		/**
		* Constructor
		* @param text The text
		* @param font The font to use
		*/
		CGUIText(const std::string & text = "", const std::string & font = "arial.fnt");

		/**
		* Destructor
		*/
		~CGUIText();

		/**
		* Set the visible value
		* @param visible The visible value
		*/
		void SetVisible(const bool visible);

		/**
		* Set the position
		* @param position The position
		*/
		void SetPosition(const CVector2I & position);

		/**
		* Set the preferred size
		* @param size The preferred size
		*/
		void SetPreferredSize(const CVector2I & size);

		/**
		* Set the bounds (position + preferred size)
		* @param bounds The bounds
		*/
		void SetBounds(const CRectangleI & bounds);

		/**
		* Set the font
		* @param font The font name
		*/
		void SetFont(const std::string & font);

		/**
		* Set the text
		* @param text The text
		*/
		void SetText(const std::string & text);

		/**
		* Set the horizontal alignment
		* @param alignment The horizontal alignment
		*/
		void SetHorizontalAlignment(ETextHorizontalAlignment alignment);

		/**
		* Set the vertical alignment
		* @param alignment The vertical alignment
		*/
		void SetVerticalAlignment(ETextVerticalAlignment alignment);

		/**
		* Set the text size
		* @param size The size
		*/
		void SetSize(const unsigned int size);

		/**
		* Set the color
		* @param color The color
		*/
		void SetColor(const CColor & color);

		/**
		* Get the horizontal alignment
		* @return The horizontal alignment
		*/
		ETextHorizontalAlignment GetHorizontalAlignment() const;

		/**
		* Get the vertical alignment
		* @return The vertical alignment
		*/
		ETextVerticalAlignment GetVerticalAlignment() const;

		/**
		* Get the changed value
		* @return The changed value
		*/
		const bool IsChanged() const;

		/**
		* Get the visible value
		* @return The visible value
		*/
		const bool IsVisible() const;

		/**
		* Check if a point is inside the gui
		* @param point The point to test
		* @return Trueif the point is inside the gui
		*/
		const bool IsInside(const CVector2I & point) const;

		/**
		* Get the text size
		* @return The text size
		*/
		const unsigned int GetSize() const;

		/**
		* Get the font name
		* @return The font name
		*/
		const std::string & GetFontName() const;

		/**
		* Get the text
		* @return The text
		*/
		const std::string & GetText() const;

		/**
		* Get the position
		* @return The position
		*/
		const CVector2I & GetPosition() const;

		/**
		* Get the preferred size
		* @return The preferred size
		*/
		const CVector2I & GetPreferredSize() const;

		/**
		* Get the bounding rectangle
		* @return The bounding rectangle
		*/
		const CRectangleI & GetBoundingRectangle() const;

		/**
		* Get the color
		* @return The color
		*/
		const CColor & GetColor() const;

		/**
		* = operator override
		* @param copy A CGUIText element copy
		* @return The CGUIText
		*/
		const CGUIText & operator=(const CGUIText & copy);

		/**
		* Get the GUI element
		* @return the GUI element
		*/
		IGUITextBase * GetGUIElement() const;
	};
}

#endif
