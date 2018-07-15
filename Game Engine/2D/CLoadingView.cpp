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

#include "CLoadingView.hpp"

#define GEAR_ROTATION_SPEED 90.0F

namespace daidalosgameengine {
	/**
	* Constructor
	* @param bounds The view bounds
	*/
	CLoadingView::CLoadingView(const daidalosengine::CRectangleI & bounds) {

		SetBounds(bounds);
		Initialize();
	}

	/**
	* Initialize the content
	*/
	void CLoadingView::Initialize() {
		this->m_backgroundSprite.SetBounds(GetBoundingRectangle());
		this->m_backgroundSprite.SetTexture("loading.png");
		AddSubview(&this->m_backgroundSprite);

		this->m_textLabel.SetBounds(daidalosengine::CRectangleI(0, (GetBoundingRectangle().GetHeight() - 100) / 2, GetBoundingRectangle().GetWidth(), 100));
		this->m_textLabel.SetSize(80);
		this->m_textLabel.SetColor(daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F));
		this->m_textLabel.SetHorizontalAlignment(daidalosengine::TEXT_HORIZONTAL_ALIGNMENT_CENTERED);
		this->m_textLabel.SetVerticalAlignment(daidalosengine::TEXT_VERTICAL_ALIGNMENT_CENTERED);
		this->m_textLabel.SetText("LOADING...");
		AddSubview(&this->m_textLabel);

		this->m_gearSprite.SetBounds(daidalosengine::CRectangleI(GetBoundingRectangle().GetWidth() - 256, GetBoundingRectangle().GetHeight() - 256, 256, 256));
		this->m_gearSprite.SetTexture("gear.png");
		AddSubview(&this->m_gearSprite);
	}

	/**
	* Update the state and the geometry
	* @param time The elapsed time
	*/
	void CLoadingView::Update(const float time) {

		// Update the gear rotation
		this->m_gearSprite.SetRotation(this->m_gearSprite.GetRotation() + (GEAR_ROTATION_SPEED * time) / 1000.0F);

		IView::Update(time);
	}
}

