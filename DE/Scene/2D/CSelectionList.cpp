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
#include "../../Scene/2D/CSelectionList.hpp"

#define BUTTON_HEIGHT 32
#define NUMBER_VISIBLE_CHOICES 3
#define TRANSPARENT_TEXTURE "alpha.png"
#define SELECTION_TEXTURE "blue.png"

namespace DE {
	/**
	* Constructor
	* @param texture The texture name
	*/
	CSelectionList::CSelectionList() {
		LOG_TRACE();

		this->m_enabled = true;
		this->m_selectionTexture = SELECTION_TEXTURE;

		// Set the title label
		this->m_title.SetHorizontalAlignment(TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		AddSubview(&this->m_title);

		// Set the value label
		this->m_selectedValue.SetDefaultTexture(TRANSPARENT_TEXTURE);
		this->m_selectedValue.GetTitleElement()->SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_selectedValue.AddButtonListener(this);
		AddSubview(&this->m_selectedValue);

		// Set the scrollable view
		this->m_scrollableView.SetVisible(false);
		AddSubview(&this->m_scrollableView);
	}

	/**
	* Destructor
	*/
	CSelectionList::~CSelectionList() {
		LOG_TRACE();
	}

	/**
	* Add a button listener
	* @param listener The button listener
	*/
	void CSelectionList::AddSelectionListListener(ISelectionListListener * listener) {
		LOG_TRACE();

		this->m_selectionListListeners.insert(listener);
	}

	/**
	* Remove a button listener
	* @param listener The button listener
	*/
	void CSelectionList::RemoveSelectionListListener(ISelectionListListener * listener) {
		LOG_TRACE();

		this->m_selectionListListeners.erase(listener);
	}

	/**
	* Remove all the listeners
	*/
	void CSelectionList::RemoveAllSelectionListListeners() {
		LOG_TRACE();

		this->m_selectionListListeners.clear();
	}

	/**
	* Set the enabled state
	* @param enabled The enabled state
	*/
	void CSelectionList::SetEnabled(const bool enabled) {
		LOG_TRACE();

		this->m_enabled = enabled;
	}

	/**
	* Set the selection list title
	* @param title The title
	*/
	void CSelectionList::SetSelectionListTitle(const std::string & title) {
		LOG_TRACE();

		this->m_title.SetText(title);
	}

	/**
	* Set the selection texture
	* @param texture The texture
	*/
	void CSelectionList::SetSelectionTexture(const std::string & texture) {
		LOG_TRACE();

		this->m_selectionTexture = texture;
	}

	/**
	* Update the state and the geometry
	* @param time The elapsed time
	*/
	void CSelectionList::Update(const float time) {
		LOG_TRACE();

		// Update the title
		this->m_title.Update(time);

		// Update the value position and size
		this->m_selectedValue.SetBounds(DE::CRectangleI(this->m_title.GetBoundingRectangle().GetRight(), 0, this->m_preferredsize.x - this->m_title.GetBoundingRectangle().GetRight(), BUTTON_HEIGHT));
		this->m_selectedValue.Update(time);

		// Update the button size
		int index = 0;
		for (auto it = this->m_selections.begin(); it != this->m_selections.end(); ++it) {
			(*it)->SetBounds(DE::CRectangleI(0, index * BUTTON_HEIGHT, this->m_selectedValue.GetPreferredSize().x, BUTTON_HEIGHT));
			index++;
		}

		// Update the scrollabe view position and size
		this->m_scrollableView.SetBounds(DE::CRectangleI(this->m_selectedValue.GetPosition().x, this->m_selectedValue.GetPosition().y + BUTTON_HEIGHT, this->m_selectedValue.GetPreferredSize().x, NUMBER_VISIBLE_CHOICES * BUTTON_HEIGHT));
		this->m_scrollableView.Update(time);
	}

	/**
	* Render the button
	*/
	void CSelectionList::Render() {
		LOG_TRACE();

		IView::Render();
	}

	/**
	* Perform a hit test, checking if the point is inside in the the subview or in this view
	* The test is exclusive, only 1 view can succeed in the hit test
	* @param point The point to test in screen coordinate
	* @return True if the point is inside the view on inside one of its subviews
	*/
	const bool CSelectionList::HitTest(const CVector2I & point) {
		LOG_TRACE();

		bool hitTest = this->m_selectedValue.HitTest(point);

		if (!hitTest && this->m_scrollableView.IsVisible()) {
			hitTest = this->m_scrollableView.HitTest(point);
		}

		return hitTest;
	}

	/**
	* Check if the button is enabled
	* @return True if the button is enabled, false otherwise
	*/
	const bool CSelectionList::IsEnabled() const {
		LOG_TRACE();

		return this->m_enabled;
	}

	/**
	* Add a selection in the list and return the associated index
	* @param text The selection text
	* @return The associated index
	*/
	const int CSelectionList::AddSelection(const std::string & text) {
		LOG_TRACE();

		this->m_selections.push_back(new DE::CButton(TRANSPARENT_TEXTURE));
		int index = this->m_selections.size() - 1;

		// Fill the button
		this->m_selections[index]->GetTitleElement()->SetText(text);
		this->m_selections[index]->SetFocusedTexture(this->m_selectionTexture);
		this->m_selections[index]->AddButtonListener(this);
		this->m_scrollableView.AddSubview(this->m_selections[index]);

		// Set the first value as selected
		if (index == 0) {
			this->m_selectedValue.GetTitleElement()->SetText(text);
			this->m_selections[index]->SetDefaultTexture(this->m_selectionTexture);
		}

		return index;
	}

	/**
	* Called when the button is clicked
	* @param button The button that trigger the action
	*/
	void CSelectionList::OnButtonClicked(CButton * button) {
		LOG_TRACE();

		if (button == &this->m_selectedValue) {
			this->m_scrollableView.SetVisible(!this->m_scrollableView.IsVisible());
		} else {
			// A new value has been selected
			this->m_selectedValue.GetTitleElement()->SetText(button->GetTitleElement()->GetText());
			this->m_scrollableView.SetVisible(false);

			// Find the clicked button
			int index = -1;
			for (auto it = this->m_selections.begin(); it != this->m_selections.end(); ++it) {
				if ((*it) == button) {
					// Mark as selected
					(*it)->SetDefaultTexture(this->m_selectionTexture);

					// Call the listeners
					for (auto it = this->m_selectionListListeners.begin(); it != this->m_selectionListListeners.end(); ++it) {
						(*it)->OnValueSelected(index, button->GetTitleElement()->GetText());
					}
				} else {
					// Mark as unselected
					(*it)->SetDefaultTexture(TRANSPARENT_TEXTURE);
				}
				index++;
			}
		}
	}
}