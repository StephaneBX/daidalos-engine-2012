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

#ifndef __CSELECTIONLIST_HPP
#define __CSELECTIONLIST_HPP

#include <vector>
#include <set>
#include "../../Scene/2D/IView.hpp"
#include "../../Scene/2D/CLabel.hpp"
#include "../../Scene/2D/CButton.hpp"
#include "../../Scene/2D/CScrollableView.hpp"
#include "../../Scene/2D/Listeners/ISelectionListListener.hpp"
#include "../../Scene/2D/Listeners/IButtonListener.hpp"

namespace daidalosengine {
	/**
	* Basic class for a selection list
	*/
	class CSelectionList : public IView, public IButtonListener {
	private:
		bool m_enabled;
		std::string m_selectionTexture;
		daidalosengine::CLabel m_title;
		daidalosengine::CButton m_selectedValue;
		daidalosengine::CScrollableView m_scrollableView;
		std::vector<daidalosengine::CButton*> m_selections;
		std::set<ISelectionListListener *> m_selectionListListeners;

	public:
		/**
		* Constructor
		* @param texture The texture name
		*/
		CSelectionList();

		/**
		* Destructor
		*/
		virtual ~CSelectionList();

		/**
		* Initialize the content
		*/
		virtual void Initialize() final;

		/**
		* Add a button listener
		* @param listener The button listener
		*/
		void AddSelectionListListener(ISelectionListListener * listener);

		/**
		* Remove a button listener
		* @param listener The button listener
		*/
		void RemoveSelectionListListener(ISelectionListListener * listener);

		/**
		* Remove all the listeners
		*/
		void RemoveAllSelectionListListeners();

		/**
		* Set the enabled state
		* @param enabled The enabled state
		*/
		void SetEnabled(const bool enabled);

		/**
		* Set the selection list title
		* @param title The title
		*/
		void SetSelectionListTitle(const std::string & title);

		/**
		* Set the selection texture
		* @param texture The texture
		*/
		void SetSelectionTexture(const std::string & texture);

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
		* Add a selection in the list and return the associated index
		* @param text The selection text
		* @return The associated index
		*/
		const int AddSelection(const std::string & text);

		/**
		* Called when the button is clicked
		* @param button The button that trigger the action
		*/
		void OnButtonClicked(CButton * button) override;
	};
}

#endif
