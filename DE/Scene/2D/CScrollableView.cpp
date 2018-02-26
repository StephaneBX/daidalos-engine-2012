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

#include "../../Core/IRenderer.hpp"
#include "../../Core/CInputDevice.hpp"
#include "../../Core/Logger/ILogger.hpp"
#include "../../Scene/2D/CScrollableView.hpp"

#define DEFAULT_SCROLL_PITCH 20
#define DEFAULT_SCROLLBAR_THICKNESS 10
#define DEFAULT_SCROLLBAR_COLOR CColor(0.5F, 0.5F, 0.5F, 0.5F)

namespace daidalosengine {
	/**
	* Constructor
	*/
	CScrollableView::CScrollableView() {
		LOG_TRACE();

		Initialize();
	}

	/**
	* Destructor
	*/
	CScrollableView::~CScrollableView() {
		LOG_TRACE();

	}

	/**
	* Initialize the content
	*/
	void CScrollableView::Initialize() {
		this->m_enabled = true;
		this->m_scrollPitch = DEFAULT_SCROLL_PITCH;

		// Initialize the scroll bar
		this->m_scrollBarView.SetBounds(daidalosengine::CRectangleI(0, 0, DEFAULT_SCROLLBAR_THICKNESS, DEFAULT_SCROLLBAR_THICKNESS));
		this->m_scrollBarView.SetBackgroundColor(DEFAULT_SCROLLBAR_COLOR);

		// Add the content and the scroll bar
		this->m_subviews.push_back(&this->m_contentView);
		this->m_contentView.SetParent(this);
		this->m_subviews.push_back(&this->m_scrollBarView);
		this->m_scrollBarView.SetParent(this);
	}

	/**
	* Update the text geometry
	* @param time The elapsed time
	*/
	void CScrollableView::Update(const float time) {
		LOG_TRACE();

		// Update the content view
		this->m_contentView.Update(time);

		// Update the scroll view size to contain the content view and the scroll bar
		SetPreferredSize(daidalosengine::CVector2I(this->m_contentView.GetPreferredSize().x + this->m_scrollBarView.GetPreferredSize().x, GetPreferredSize().y));

		// Calculate the scroll bar new position and size
		float sizeFactor = this->m_preferredsize.y / (float)this->m_contentView.GetPreferredSize().y;
		this->m_scrollBarView.SetBounds(daidalosengine::CRectangleI(GetPreferredSize().x - this->m_scrollBarView.GetPreferredSize().x, -(int)(this->m_contentView.GetPosition().y * sizeFactor), this->m_scrollBarView.GetPreferredSize().x, (int)(GetPreferredSize().y * sizeFactor)));

		// Update the scroll bar
		this->m_scrollBarView.Update(time);

		// Update the scroll bar visibility
		this->m_scrollBarView.SetVisible(GetContentTopPosition() != 0 || GetContentBottomPosition() != 0);
	}

	/**
	* Add a subview
	* @param view The subview to add
	*/
	void CScrollableView::AddSubview(IView * view) {
		LOG_TRACE();

		this->m_contentView.AddSubview(view);
	}

	/**
	* Remove a subview
	* @param view The subview to remove
	*/
	void CScrollableView::RemoveSubview(IView * view) {
		LOG_TRACE();

		this->m_contentView.RemoveSubview(view);
	}

	/**
	* Remove all the subviews
	*/
	void CScrollableView::RemoveAllSubviews() {
		LOG_TRACE();

		this->m_contentView.RemoveAllSubviews();
	}

	/**
	* Set the enabled state
	* @param enabled The enabled state
	*/
	void CScrollableView::SetEnabled(const bool enabled) {
		LOG_TRACE();

		this->m_enabled = enabled;
	}

	/**
	* Set the scroll pitch in pixels
	* @param pitch The scroll pitch
	*/
	void CScrollableView::SetScrollPitch(const bool pitch) {
		LOG_TRACE();

		this->m_scrollPitch = pitch;
	}

	/**
	* Set the scroll bar thickness
	* @param thickness The scroll bar thickness
	*/
	void CScrollableView::SetScrollBarThickness(const int thickness) {
		LOG_TRACE();

		this->m_scrollBarView.SetPreferredSize(daidalosengine::CVector2I(thickness, this->m_scrollBarView.GetPreferredSize().y));
	}

	/**
	* Set the scroll bar color
	*/
	void CScrollableView::SetScrollBarColor(const daidalosengine::CColor & color) {
		LOG_TRACE();

		this->m_scrollBarView.SetBackgroundColor(color);
	}

	/**
	* Perform a hit test, checking if the point is inside of the subview or in this view
	* The test is exclusive, only 1 view can succeed in the hit test
	* @param point The point to test in screen coordinate
	* @return True if the point is inside the view on inside one of its subviews
	*/
	const bool CScrollableView::HitTest(const CVector2I & point) {
		LOG_TRACE();

		this->m_hitTest = false;

		if (this->m_enabled) {
			this->m_hitTest = IsInside(point);
			CInputDevice * inputDevice = CInputDevice::GetInstance();

			if (this->m_hitTest && inputDevice->IsActionActive(GUI_VIEW_WHEEL_UP)) {
				// Move up
				const int offset = std::min(this->m_scrollPitch, GetContentTopPosition());
				OffsetContent(offset);

			} else if (this->m_hitTest && inputDevice->IsActionActive(GUI_VIEW_WHEEL_DOWN)) {
				// Move down
				const int offset = std::min(this->m_scrollPitch, GetContentBottomPosition());
				OffsetContent(-offset);

			} else {
				// Check the content
				return this->m_contentView.HitTest(point);
			}
		}

		return this->m_hitTest;
	}

	/**
	* Check if the button is enabled
	* @return True if the button is enabled, false otherwise
	*/
	const bool CScrollableView::IsEnabled() const {
		LOG_TRACE();

		return this->m_enabled;
	}

	/**
	* Get te scroll pitch in pixels
	* @return The scroll pitch
	*/
	const int CScrollableView::GetScrollPitch() const {
		LOG_TRACE();

		return this->m_scrollPitch;
	}

	/**
	* Offset the content to a number of pixels.
	* The Offset will stop when reaching the top or bottom of the content
	* @param movement The number of pixel to offset (can be positive or negative)
	*/
	void CScrollableView::OffsetContent(const int movement) {
		LOG_TRACE();

		this->m_contentView.SetPosition(CVector2I(this->m_contentView.GetPosition().x, this->m_contentView.GetPosition().y + movement));
	}

	/**
	* Get the content position relative to the top of the scrollable view
	* @return The number of pixels away from the top of the scrollable view
	*/
	const int CScrollableView::GetContentTopPosition() const {
		LOG_TRACE();

		int topPosition = std::min(GetPreferredSize().y, this->m_contentView.GetPosition().y);

		// The top position should be positive to allow scrolling, otherwise the content view is too small
		if (topPosition > 0) {
			topPosition = 0;
		}

		// Distance from the top of the subviews to the scroll top
		return -topPosition;
	}

	/**
	* Get the content position relative to the bottom of the scrollable view
	* @return The number of pixels away from the bottom of the scrollable view
	*/
	const int CScrollableView::GetContentBottomPosition() const {
		LOG_TRACE();

		int bottomPosition = this->m_contentView.GetPosition().y + this->m_contentView.GetPreferredSize().y;

		// The bottom position should be lower than the scroll bottom to allow scrolling, otherwise the subviews are too high
		if (bottomPosition < GetPreferredSize().y) {
			bottomPosition = GetPreferredSize().y;
		}

		// Distance from the bottom of the subviews to the scroll bottom
		return bottomPosition - GetPreferredSize().y;
	}


	/**
	* Constructor
	*/
	CScrollableContentView::CScrollableContentView() {
		LOG_TRACE();
	}

	/**
	* Destructor
	*/
	CScrollableContentView::~CScrollableContentView() {
		LOG_TRACE();
	}

	/**
	* Initialize the content
	*/
	void CScrollableContentView::Initialize() {
		// Nothing to do
	}

	/**
	* Update the text geometry
	* @param time The elapsed time
	*/
	void CScrollableContentView::Update(const float time) {
		LOG_TRACE();

		// First update
		IView::Update(time);

		// Set the size to max its content
		if (this->m_subviews.size() > 0) {
			CVector2I maxSize;
			for (auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
				maxSize.x = std::max(maxSize.x, (*it)->GetPosition().x + (*it)->GetPreferredSize().x);
				maxSize.y = std::max(maxSize.y, (*it)->GetPosition().y + (*it)->GetPreferredSize().y);
			}
			this->m_preferredsize = maxSize;
			this->m_boundingRectangle = CRectangleI(this->m_position.x, this->m_position.y, this->m_preferredsize.x, this->m_preferredsize.y);
		}

		// Second update
		IView::Update(time);
	}

	/**
	* Render the view
	*/
	void CScrollableContentView::Render() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();

		for (auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			if ((*it)->IsVisible()) {
				renderer->SetScissor(this->m_parent->GetAbsoluteBoundingRectangle());
				(*it)->Render();
			}
		}
	}

	/**
	* Perform a hit test, checking if the point is inside of the subview or in this view
	* The test is exclusive, only 1 view can succeed in the hit test
	* @param point The point to test in screen coordinate
	* @return True if the point is inside the view on inside one of its subviews
	*/
	const bool CScrollableContentView::HitTest(const CVector2I & point) {
		LOG_TRACE();

		this->m_hitTest = false;

		for (auto rit = this->m_subviews.rbegin(); rit != this->m_subviews.rend(); rit++) {
			if (this->m_parent->IsInside((*rit)->GetAbsoluteBoundingRectangle()) && (*rit)->HitTest(point)) {
				this->m_hitTest = true;
			}
		}
		return this->m_hitTest;
	}
}
