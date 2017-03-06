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

#ifndef __CBUTTON_H
#define __CBUTTON_H

#include <set>
#include "../../Scene/2D/IView.h"
#include "../../Scene/2D/CLabel.h"
#include "../../Scene/2D/Listeners/IButtonListener.h"
#include "../../Core/Utility/CInset.h"
#include "../../Medias/CTexture2D.h"

namespace DE {
	/**
	* Basic class for buttons
	*/
	class CButton : public IView {
	private:
		bool m_enabled;
		bool m_clicked;
		CInsetF m_imageInset;
		CTexture2D m_defaultTexture;
		CTexture2D m_focusedTexture;
		CTexture2D m_clickedTexture;
		std::set<IButtonListener *> m_buttonListeners;
		CLabel *  m_title;
		CColor m_defaultTitleColor;
		CColor m_clickedTitleColor;
		CColor m_focusedTitleColor;

	public:
		/**
		* Constructor
		* @param texture The texture name
		*/
		CButton(const std::string & texture = "");

		/**
		* Destructor
		*/
		virtual ~CButton();

		/**
		* Add a button listener
		* @param listener The button listener
		*/
		void AddButtonListener(IButtonListener * listener);

		/**
		* Remove a button listener
		* @param listener The button listener
		*/
		void RemoveButtonListener(IButtonListener * listener);

		/**
		* Remove all the listeners
		*/
		void RemoveAllButtonListeners();

		/**
		* Set the enabled state
		* @param enabled The enabled state
		*/
		void SetEnabled(const bool enabled);

		/**
		* Set the image inset
		* The inset represents the parts of the button that will not be resized with the button
		* The values are between 0 and 1
		*/
		void SetImageInset(const CInsetF & inset);

		/**
		* Set the default texture
		* @param texture The texture
		*/
		void SetDefaultTexture(const std::string & texture);

		/**
		* Set the clicked texture
		* @param texture The texture
		*/
		void SetClickedTexture(const std::string & texture);

		/**
		* Set the focused texture
		* @param texture The texture
		*/
		void SetFocusedTexture(const std::string & texture);

		/**
		* Set the default title color
		* @param color The color
		*/
		void SetDefaultTitleColor(const CColor & color);
		/**
		* Set the clicked title color
		* @param color The color
		*/
		void SetClickedTitleColor(const CColor & color);

		/**
		* Set the focused title color
		* @param color The color
		*/
		void SetFocusedTitleColor(const CColor & color);

		/**
		* Update the state and the geometry
		* @param time The elapsed time
		*/
		virtual void Update(const float time) override;

		/**
		* Render the button
		*/
		virtual void Render() override;

		/**
		* Perform a hit test, checking if the point is inside in the the subview or in this view
		* The test is exclusive, only 1 view can succeed in the hit test
		* @param point The point to test in screen coordinate
		* @return True if the point is inside the view on inside one of its subviews
		*/
		virtual const bool HitTest(const CVector2I & point) override;

		/**
		* Check if the button is enabled
		* @return True if the button is enabled, false otherwise
		*/
		const bool IsEnabled() const;

		/**
		* Get the title
		* @return The title
		*/
		CLabel * GetTitleElement();

	private:
		/**
		* Generate the geometry
		*/
		void GenerateGeometry();

		/**
		* Set the title color
		*/
		void SetTitleColor();

		/**
		* Get the texture to used for rendering
		*/
		ITexture2DBase * GetTextureToRender();
	};
}

#endif
