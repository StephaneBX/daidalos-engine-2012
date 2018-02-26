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

#ifndef __CSCROLLABLEVIEW_HPP
#define __CSCROLLABLEVIEW_HPP

#include <set>
#include "../../Scene/2D/IView.hpp"
#include "../../Scene/2D/CSprite.hpp"

namespace daidalosengine {
	/**
	* Content view for the CScrollableView
	*/
	class CScrollableContentView : public IView {
	public:
		/**
		* Constructor
		*/
		CScrollableContentView();

		/**
		* Destructor
		*/
		virtual ~CScrollableContentView();

		/**
		* Initialize the content
		*/
		virtual void Initialize();

		/**
		* Update the text geometry
		* @param time The elapsed time
		*/
		virtual void Update(const float time) override;

		/**
		* Render the text
		*/
		virtual void Render() override;

		/**
		* Perform a hit test, checking if the point is inside of the subview or in this view
		* The test is exclusive, only 1 view can succeed in the hit test
		* @param point The point to test in screen coordinate
		* @return True if the point is inside the view on inside one of its subviews
		*/
		virtual const bool HitTest(const CVector2I & point) override;
	};

	/**
	* Basic class for scrollable views
	*/
	class CScrollableView : public IView {
	private:
		bool m_enabled;
		int m_scrollPitch;
		CScrollableContentView m_contentView;
		CSprite m_scrollBarView;

	public:
		/**
		* Constructor
		*/
		CScrollableView();

		/**
		* Destructor
		*/
		virtual ~CScrollableView();

		/**
		* Initialize the content
		*/
		void Initialize() final;

		/**
		* Update the text geometry
		* @param time The elapsed time
		*/
		virtual void Update(const float time) override;

		/**
		* Add a subview
		* @param view The subview to add
		*/
		virtual void AddSubview(IView * view) override;

		/**
		* Remove a subview
		* @param view The subview to remove
		*/
		virtual void RemoveSubview(IView * view) override;

		/**
		* Remove all the subviews
		*/
		virtual void RemoveAllSubviews();

		/**
		* Set the enabled state
		* @param enabled The enabled state
		*/
		void SetEnabled(const bool enabled);

		/**
		* Set the scroll pitch in pixels
		* @param pitch The scroll pitch
		*/
		void SetScrollPitch(const bool pitch);

		/**
		* Set the scroll bar thickness
		* @param thickness The scroll bar thickness
		*/
		void SetScrollBarThickness(const int thickness);

		/**
		* Set the scroll bar color
		*/
		void SetScrollBarColor(const daidalosengine::CColor & color);

		/**
		* Perform a hit test, checking if the point is inside of the subview or in this view
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
		* Get te scroll pitch in pixels
		* @return The scroll pitch
		*/
		const int GetScrollPitch() const;

	private:
		/**
		* Offset the content to a number of pixels.
		* The Offset will stop when reaching the top or bottom of the content
		* @param movement The number of pixel to offset (can be positive or negative)
		*/
		void OffsetContent(const int movement);

		/**
		* Get the content position relative to the top of the scrollable view
		* @return The number of pixels away from the top of the scrollable view
		*/
		const int GetContentTopPosition() const;

		/**
		* Get the content position relative to the bottom of the scrollable view
		* @return The number of pixels away from the bottom of the scrollable view
		*/
		const int GetContentBottomPosition() const;
	};
}

#endif
