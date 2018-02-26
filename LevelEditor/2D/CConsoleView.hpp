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

#ifndef __CCONSOLEVIEW_HPP
#define __CCONSOLEVIEW_HPP

#include <Scene/2D/CSprite.hpp>
#include <Scene/2D/CLabel.hpp>
#include <Core/Listeners/IInputKeyboardListener.hpp>
#include <Core/CInputDevice.hpp>
#include "IConsoleListener.hpp"

namespace daidaloseditor {

	/**
	* Console implementation
	*/
	class CConsoleView : public daidalosengine::CSprite, public daidalosengine::IInputKeyboardListener {
	private:
		int m_numberOfLines;
		std::vector<IConsoleListener*> m_listeners;
		std::vector<daidalosengine::CLabel> m_historyLabel;
		daidalosengine::CLabel m_inputLabel;

	public:
		/**
		* Constructor
		* @param texture The view bounds
		*/
		explicit CConsoleView(const int numberOfLines);

		/**
		* Initialize the content
		*/
		void Initialize() final;

		/**
		* Set the visible value
		* @param visible The visible value
		*/
		void SetVisible(const bool visible) final;

		/**
		* Add a listener
		* @param listener The listener to add
		*/
		void AddListener(IConsoleListener * listener);

		/**
		* Remove a listener
		* @param listener The listener to remove
		*/
		void RemoveListener(IConsoleListener * listener);

		/**
		* Called when a keyboard action is started
		* @param actionId The action id
		*/
		void OnInputActionStarted(const unsigned int actionId);

		/**
		* Called when a keyboard action is stopped
		* @param actionId The action id
		*/
		void OnInputActionStopped(const unsigned int actionId);

		/**
		* Called when a key has been pressed
		* @param key The key
		*/
		void OnKeyPressed(const unsigned int key);

	private:
		/**
		* Check the input label for a valid command
		*/
		void ValidInputLabel();

		/**
		* Move the current input in the history labels and shift them
		*/
		void ShiftLabels();
	};
}

#endif
