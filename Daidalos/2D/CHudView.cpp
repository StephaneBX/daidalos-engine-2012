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
#include "CHudView.hpp"

namespace daidalos {

	CHudView::CHudView(const daidalosengine::CRectangleI & bounds) {

		SetBounds(bounds);
		Initialize();
	}

	/**
	* Initialize the content
	*/
	void CHudView::Initialize() {
		// Create the position label
		this->m_positionLabel.SetBounds(daidalosengine::CRectangleI(5, 0, 400, 30));
		this->m_positionLabel.SetSize(26);
		this->m_positionLabel.SetColor(daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F));
		this->m_positionLabel.SetHorizontalAlignment(daidalosengine::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_positionLabel.SetVerticalAlignment(daidalosengine::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_positionLabel.SetText("Position");
		AddSubview(&this->m_positionLabel);

		// Create the gravity label
		this->m_gravityLabel.SetBounds(daidalosengine::CRectangleI(5, 30, 200, 30));
		this->m_gravityLabel.SetSize(26);
		this->m_gravityLabel.SetColor(daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F));
		this->m_gravityLabel.SetHorizontalAlignment(daidalosengine::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_gravityLabel.SetVerticalAlignment(daidalosengine::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_gravityLabel.SetText("Gravity");
		AddSubview(&this->m_gravityLabel);

		// Create the health sprite
		this->m_healthSprite.SetTexture("hud.png");
		this->m_healthSprite.SetCoordinates(daidalosengine::CRectangleF(0, 0.281F, 0.125F, 0.125F));
		this->m_healthSprite.SetBounds(daidalosengine::CRectangleI(40, GetPreferredSize().y - 110, 32, 32));
		AddSubview(&this->m_healthSprite);

		// Create the health label
		this->m_healthLabel.SetBounds(daidalosengine::CRectangleI(75, GetPreferredSize().y - 110, 100, 55));
		this->m_healthLabel.SetSize(35);
		this->m_healthLabel.SetColor(daidalosengine::CColor(0.0F, 1.0F, 0.0F, 1.0F));
		this->m_healthLabel.SetHorizontalAlignment(daidalosengine::TEXT_HORIZONTAL_ALIGNMENT_RIGHT);
		this->m_healthLabel.SetVerticalAlignment(daidalosengine::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_healthLabel.SetText("0%");
		AddSubview(&this->m_healthLabel);

		// Create the armor sprite
		this->m_armorSprite.SetTexture("hud.png");
		this->m_armorSprite.SetCoordinates(daidalosengine::CRectangleF(0, 0.141F, 0.125F, 0.125F));
		this->m_armorSprite.SetBounds(daidalosengine::CRectangleI(40, GetPreferredSize().y - 55, 32, 32));
		AddSubview(&this->m_armorSprite);

		// Create the armor label
		this->m_armorLabel.SetBounds(daidalosengine::CRectangleI(75, GetPreferredSize().y - 55, 100, 55));
		this->m_armorLabel.SetSize(35);
		this->m_armorLabel.SetColor(daidalosengine::CColor(1.0F, 0.0F, 0.0F, 1.0F));
		this->m_armorLabel.SetHorizontalAlignment(daidalosengine::TEXT_HORIZONTAL_ALIGNMENT_RIGHT);
		this->m_armorLabel.SetVerticalAlignment(daidalosengine::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_armorLabel.SetText("0%");
		AddSubview(&this->m_armorLabel);

		// Create the ammo sprite
		this->m_ammoSprite.SetTexture("hud.png");
		this->m_ammoSprite.SetCoordinates(daidalosengine::CRectangleF(0, 0, 0.125F, 0.125F));
		this->m_ammoSprite.SetBounds(daidalosengine::CRectangleI(GetPreferredSize().x - 40 - 32, GetPreferredSize().y - 55, 32, 32));
		AddSubview(&this->m_ammoSprite);

		// Create the ammo label
		this->m_ammoLabel.SetBounds(daidalosengine::CRectangleI(GetPreferredSize().x - 230, GetPreferredSize().y - 55, 130, 55));
		this->m_ammoLabel.SetSize(35);
		this->m_ammoLabel.SetColor(daidalosengine::CColor(1.0F, 1.0F, 1.0F, 1.0F));
		this->m_ammoLabel.SetHorizontalAlignment(daidalosengine::TEXT_HORIZONTAL_ALIGNMENT_RIGHT);
		this->m_ammoLabel.SetVerticalAlignment(daidalosengine::TEXT_VERTICAL_ALIGNMENT_TOP);
		this->m_ammoLabel.SetText("0 / 0");
		AddSubview(&this->m_ammoLabel);

		// Create the flashlight sprite
		this->m_flashlightSprite.SetTexture("hud.png");
		this->m_flashlightSprite.SetCoordinates(daidalosengine::CRectangleF(0.141F, 0, 0.250F, 0.250F));
		this->m_flashlightSprite.SetBounds(daidalosengine::CRectangleI(GetPreferredSize().x - 40 - 32, 20, 32, 32));
		this->m_flashlightSprite.SetVisible(false);
		AddSubview(&this->m_flashlightSprite);

		// Create the crosshair
		this->m_crosshairView.SetTexture("crosshair.tga");
		this->m_crosshairView.SetBounds(daidalosengine::CRectangleI((GetPreferredSize().x - 32) / 2, (GetPreferredSize().y - 32) / 2, 32, 32));
		AddSubview(&this->m_crosshairView);
	}

	/**
	* Set the debug mode
	* @param debugMode The debug mode
	*/
	void CHudView::SetDebugMode(const bool debugMode) {
		this->m_debugMode = debugMode;
		this->m_positionLabel.SetVisible(this->m_debugMode);
		this->m_gravityLabel.SetVisible(this->m_debugMode);
	}

	/**
	* Set the flashlight state
	* @param flashlight The flashlight state
	*/
	void CHudView::SetFlashlight(const bool flashlight) {
		this->m_flashlightSprite.SetVisible(flashlight);
	}

	/**
	* Set the player's position
	* @param position The position
	*/
	void CHudView::SetPosition(const daidalosengine::CVector3F position) {
		std::stringstream ss;
		ss << "POSITION: " << static_cast<int>(position.x) << " : " << static_cast<int>(position.y) << " : " << static_cast<int>(position.z);
		this->m_positionLabel.SetText(ss.str());
	}

	/**
	* Set the players's gravity
	* @param gravity The gravity
	*/
	void CHudView::SetGravity(const float gravity) {
		std::stringstream ss;
		ss << "GRAVITY: " << static_cast<int>(gravity);
		this->m_gravityLabel.SetText(ss.str());
	}

	/**
	* Set the player's health
	* @param health The health
	*/
	void CHudView::SetHealth(const int health) {
		std::stringstream ss;
		ss << static_cast<int>(health) << "%";
		this->m_healthLabel.SetText(ss.str());
		this->m_healthLabel.SetColor(GetStateColor(health));
	}

	/**
	* Set the player's armor
	* @param armor The armor
	*/
	void CHudView::SetArmor(const int armor) {
		std::stringstream ss;
		ss << static_cast<int>(armor) << "%";
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
		ss << static_cast<int>(magazine) << " / " << static_cast<int>(stock);
		this->m_ammoLabel.SetText(ss.str());
	}

	/**
	* Get the color for a state value
	* @param state The state value
	*/
	daidalosengine::CColor CHudView::GetStateColor(const int state) {

		daidalosengine::CColor color;

		if(state <= 20) {
			color = daidalosengine::CColor(1.0F, 0.0F, 0.0F, 1.0F);

		} else if(state <= 80) {
			color = daidalosengine::CColor(1.0F, 1.0F, 0.5F, 1.0F);

		} else if(state <= 100) {
			color = daidalosengine::CColor(0.0F, 1.0F, 0.0F, 1.0F);

		} else {
			color = daidalosengine::CColor(0.0F, 0.0F, 1.0F, 1.0F);
		}
		return color;
	}
}

