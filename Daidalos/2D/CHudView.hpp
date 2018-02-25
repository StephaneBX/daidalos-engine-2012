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

#ifndef __DAIDALOS_CHUDVIEW_HPP
#define __DAIDALOS_CHUDVIEW_HPP

#include <Scene/2D/IView.hpp>
#include <Scene/2D/CLabel.hpp>
#include <Scene/2D/CSprite.hpp>

namespace daidalos {
	/**
	* Head-up display implementation
	*/
	class CHudView : public daidalosengine::IView {
	private:
		bool m_debugMode;
		daidalosengine::CLabel m_positionLabel;
		daidalosengine::CLabel m_gravityLabel;
		daidalosengine::CSprite m_armorSprite;
		daidalosengine::CLabel m_armorLabel;
		daidalosengine::CSprite m_healthSprite;
		daidalosengine::CLabel m_healthLabel;
		daidalosengine::CSprite m_ammoSprite;
		daidalosengine::CLabel m_ammoLabel;
		daidalosengine::CSprite m_flashlightSprite;
		daidalosengine::CSprite m_crosshairView;

	public:
		/**
		* Constructor
		* @param texture The view bounds
		*/
		explicit CHudView(const daidalosengine::CRectangleI & bounds);

		/**
		* Set the debug mode
		* @param debugMode The debug mode
		*/
		void SetDebugMode(const bool debugMode);

		/**
		* Set the flashlight state
		* @param flashlight The flashlight state
		*/
		void SetFlashlight(const bool flashlight);

		/**
		* Set the player's position
		* @param position The position
		*/
		void SetPosition(const daidalosengine::CVector3F position);

		/**
		* Set the players's gravity
		* @param gravity The gravity
		*/
		void SetGravity(const float gravity);

		/**
		* Set the player's health
		* @param health The health
		*/
		void SetHealth(const int health);

		/**
		* Set the player's armor
		* @param armor The armor
		*/
		void SetArmor(const int armor);

		/**
		* Set the player's ammo
		* @param magazine The magazine ammo
		* @param stock The stock ammo
		*/
		void SetAmmo(const unsigned int magazine, const unsigned int stock);

	private:
		/**
		* Get the color for a state value
		* @param state The state value
		*/
		const daidalosengine::CColor GetStateColor(const int state);
	};
}

#endif
