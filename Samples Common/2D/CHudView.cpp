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

#include <sstream>
#include "CHudView.h"

namespace SAMPLES {

	CHudView::CHudView(const DE::CRectangleI & bounds) : IView() {

		SetBounds(bounds);

		// Create the fps label
		this->m_fpsLabel.SetBounds(DE::CRectangleI((int)(this->m_preferredsize.x - 200), 0, 200, 30));
		this->m_fpsLabel.SetSize(30);
		this->m_fpsLabel.SetColor(DE::CColor(255, 255, 102, 255));
		this->m_fpsLabel.SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_fpsLabel.SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_fpsLabel.SetText("FPS");
		AddSubview(&this->m_fpsLabel);

		// Create the position label
		this->m_positionLabel.SetBounds(DE::CRectangleI(5, 0, 400, 30));
		this->m_positionLabel.SetSize(26);
		this->m_positionLabel.SetColor(DE::CColor(1.0f, 1.0f, 1.0f, 1.0f));
		this->m_positionLabel.SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_positionLabel.SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_positionLabel.SetText("Position");
		AddSubview(&this->m_positionLabel);

		// Create the gravity label
		this->m_gravityLabel.SetBounds(DE::CRectangleI(5, 30, 200, 30));
		this->m_gravityLabel.SetSize(26);
		this->m_gravityLabel.SetColor(DE::CColor(1.0f, 1.0f, 1.0f, 1.0f));
		this->m_gravityLabel.SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_gravityLabel.SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_gravityLabel.SetText("Gravity");
		AddSubview(&this->m_gravityLabel);

		// Create the health sprite
		this->m_healthSprite.SetTexture("health.png");
		this->m_healthSprite.SetBounds(DE::CRectangleI(40, this->m_preferredsize.y - 110, 32, 32));
		AddSubview(&this->m_healthSprite);

		// Create the health label
		this->m_healthLabel.SetBounds(DE::CRectangleI(75, this->m_preferredsize.y - 110, 100, 55));
		this->m_healthLabel.SetSize(35);
		this->m_healthLabel.SetColor(DE::CColor(0.0f, 1.0f, 0.0f, 1.0f));
		this->m_healthLabel.SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_RIGHT);
		this->m_healthLabel.SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_healthLabel.SetText("0%");
		AddSubview(&this->m_healthLabel);

		// Create the armor sprite
		this->m_armorSprite.SetTexture("armor.png");
		this->m_armorSprite.SetBounds(DE::CRectangleI(40, this->m_preferredsize.y - 55, 32, 32));
		AddSubview(&this->m_armorSprite);

		// Create the armor label
		this->m_armorLabel.SetBounds(DE::CRectangleI(75, this->m_preferredsize.y - 55, 100, 55));
		this->m_armorLabel.SetSize(35);
		this->m_armorLabel.SetColor(DE::CColor(1.0f, 0.0f, 0.0f, 1.0f));
		this->m_armorLabel.SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_RIGHT);
		this->m_armorLabel.SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_armorLabel.SetText("0%");
		AddSubview(&this->m_armorLabel);

		// Create the ammo sprite
		this->m_ammoSprite.SetTexture("ammo.png");
		this->m_ammoSprite.SetBounds(DE::CRectangleI(this->m_preferredsize.x - 40 - 32, this->m_preferredsize.y - 55, 32, 32));
		AddSubview(&this->m_ammoSprite);

		// Create the ammo label
		this->m_ammoLabel.SetBounds(DE::CRectangleI(this->m_preferredsize.x - 230, this->m_preferredsize.y - 55, 130, 55));
		this->m_ammoLabel.SetSize(35);
		this->m_ammoLabel.SetColor(DE::CColor(1.0f, 1.0f, 1.0f, 1.0f));
		this->m_ammoLabel.SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_RIGHT);
		this->m_ammoLabel.SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_ammoLabel.SetText("0 / 0");
		AddSubview(&this->m_ammoLabel);

		// Create the crosshair
		this->m_crosshairView.SetTexture("crosshair.tga");
		this->m_crosshairView.SetBounds(DE::CRectangleI((this->m_preferredsize.x - 32) / 2, (this->m_preferredsize.y - 32) / 2, 32, 32));
		AddSubview(&this->m_crosshairView);
	}

	/**
	* Set the fps value
	* @param fps Set the fps value
	*/
	void CHudView::SetFPS(const std::string fps) {
		this->m_fpsLabel.SetText(fps);
	}

	/**
	* Set the player's position
	* @param position The position
	*/
	void CHudView::SetPosition(const DE::CVector3F position) {
		std::stringstream ss;
		ss << "POSITION: " << (int)position.x << " : " << (int)position.y << " : " << (int)position.z;
		this->m_positionLabel.SetText(ss.str());
	}

	/**
	* Set the players's gravity
	* @param gravity The gravity
	*/
	void CHudView::SetGravity(const float gravity) {
		std::stringstream ss;
		ss << "GRAVITY: " << (int)gravity;
		this->m_gravityLabel.SetText(ss.str());
	}

	/**
	* Set the player's health
	* @param health The health
	*/
	void CHudView::SetHealth(const int health) {
		std::stringstream ss;
		ss << (int)health << "%";
		this->m_healthLabel.SetText(ss.str());
		this->m_healthLabel.SetColor(GetStateColor(health));
	}

	/**
	* Set the player's armor
	* @param armor The armor
	*/
	void CHudView::SetArmor(const int armor) {
		std::stringstream ss;
		ss << (int)armor << "%";
		this->m_armorLabel.SetText(ss.str());
		this->m_armorLabel.SetColor(GetStateColor(armor));
	}

	/**
	* Set the player's ammo
	* @param magazine The magazine ammo
	* @param stock The stock ammo
	*/
	void CHudView::SetAmmo(const unsigned int magazine, const unsigned int stock) {
		std::stringstream ss;
		ss << (int)magazine << " / " << (int)stock;
		this->m_ammoLabel.SetText(ss.str());
	}

	/**
	* Get the color for a state value
	* @param state The state value
	*/
	const DE::CColor CHudView::GetStateColor(const int state) {

		DE::CColor color;

		if(state <= 20) {
			color = DE::CColor(1.0f, 0.0f, 0.0f, 1.0f);

		} else if(state <= 80) {
			color = DE::CColor(1.0f, 1.0f, 0.5f, 1.0f);

		} else if(state <= 100) {
			color = DE::CColor(0.0f, 1.0f, 0.0f, 1.0f);

		} else {
			color = DE::CColor(0.0f, 0.0f, 1.0f, 1.0f);
		}
		return color;
	}
}

