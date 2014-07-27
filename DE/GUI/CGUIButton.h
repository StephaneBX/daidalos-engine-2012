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

#ifndef __CGUIBUTTON_H
#define __CGUIBUTTON_H

#include "../GUI/IGUIButton.h"

namespace DE
{
	/**
	* Gui button enclosing class
	*/
	class CGUIButton
	{
	private:
		IGUIButton * m_button;

	public:
		/**
		* Constructor
		* @param texture The default Texture
		*/
		CGUIButton(const std::string & texture = "");

		/**
		* Destructor
		*/
		~CGUIButton();

		/**
		* Set the visible value
		* @param visible The visible value
		*/
		void SetVisible(const bool visible);

		/**
		* Set the enabled state
		* @param enabled The enabled state
		*/
		void SetEnabled(const bool enabled);

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
		* Set the image inset
		* The inset represents the parts of the button that will not be resized with the button
		* The values are between 0 and 1
		*/
		void SetImageInset(const CInsetF & inset);

		/**
		* Set the default texture
		*/
		void SetDefaultTexture(const std::string & texture);

		/**
		* Set the clicked texture
		*/
		void SetClickedTexture(const std::string & texture);

		/**
		* Set the focused texture
		*/
		void SetFocusedTexture(const std::string & texture);

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
		* Check if the button is enabled
		* @return True if the button is enabled, false otherwise
		*/
		const bool IsEnabled() const;

		/**
		* Check if a point is inside the gui
		* @param point The point to test
		* @return True if the point is inside the gui
		*/
		const bool IsInside(const CVector2I & point) const;

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
		* = operator override
		* @param copy A CGUIText element copy
		* @return The CGUIText
		*/
		const CGUIButton & operator=(const CGUIButton & copy);

		/**
		* Get the GUI element
		* @return the GUI element
		*/
		IGUIButton * GetGUIElement() const;
	};
}

#endif
