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

#ifndef __CCHECKBOX_HPP
#define __CCHECKBOX_HPP

#include <set>
#include "../../Scene/2D/IView.hpp"
#include "../../Scene/2D/CLabel.hpp"
#include "../../Scene/2D/Listeners/ICheckboxListener.hpp"
#include "../../Core/Utility/CInset.hpp"
#include "../../Medias/CTexture2D.hpp"

namespace DE {
	/**
	* Basic class for gui checkbox
	*/
	class CCheckbox : public IView {
	private:
		bool m_enabled;
		bool m_checked;
		bool m_clicked;
		CInsetI m_labelInset;
		CTexture2D m_uncheckedTexture;
		CTexture2D m_checkedTexture;
		CTexture2D m_clickedTexture;
		std::set<ICheckboxListener *> m_checkboxListeners;
		CLabel * m_label;

	public:
		/**
		* Constructor
		*/
		CCheckbox();

		/**
		* Destructor
		*/
		virtual ~CCheckbox();

		/**
		* Add a checkbox listener
		* @param listener The checkbox listener
		*/
		void AddCheckboxListener(ICheckboxListener * listener);

		/**
		* Remove a checkbox listener
		* @param listener The checkbox listener
		*/
		void RemoveCheckboxListener(ICheckboxListener * listener);

		/**
		* Remove all the listeners
		*/
		void RemoveAllCheckboxListeners();

		/**
		* Set the enabled state
		* @param enabled The enabled state
		*/
		void SetEnabled(const bool enabled);

		/**
		* Set the checked state
		* @param checked The checked state
		*/
		void SetChecked(const bool checked);

		/**
		* Set the label inset
		* @param inset The label inset
		*/
		void SetLabelInset(const CInsetI & inset);

		/**
		* Set the unchecked texture
		* @param texture The texture
		*/
		void SetUncheckedTexture(const std::string & texture);

		/**
		* Set the checked texture
		* @param texture The texture
		*/
		void SetCheckedTexture(const std::string & texture);

		/**
		* Set the clicked texture
		* @param texture The texture
		*/
		void SetClickedTexture(const std::string & texture);

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
		* Check if the checkbox is enabled
		* @return True if the button is enabled, false otherwise
		*/
		const bool IsEnabled() const;

		/**
		* Check if the checkbox is checked
		* @return True if the checkbox is checked, false otherwise
		*/
		const bool IsChecked() const;

		/**
		* Get the title
		* @return The title
		*/
		CLabel * GetLabelElement();

	private:
		/**
		* Generate the geometry
		*/
		void GenerateGeometry();

		/**
		* Get the position rectangle
		*/
		CRectangleI GetPositionRectangle();

		/**
		* Get the texture to used for rendering
		*/
		ITexture2DBase * GetTextureToRender();
	};
}

#endif
