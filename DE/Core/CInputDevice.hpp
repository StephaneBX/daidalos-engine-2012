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

#ifndef __CINPUTDEVICE_HPP
#define __CINPUTDEVICE_HPP

#include <map>
#include <vector>
#include <array>
#include "../Core/CSingletonManager.hpp"
#include "../Core/Utility/CVector2.hpp"
#include "../Core/Listeners/IInputKeyboardListener.hpp"
#include "../Core/Listeners/IInputMouseListener.hpp"
#include "../Core/Listeners/IInputGamepadListener.hpp"

#define INPUT_NO_LINKED_ACTION -1
#define MAX_GAMEPADS 4
#define GAMEPAD_AXIS_THRESHOLD 0.25f

namespace daidalosengine {
	enum EInputKey {
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_BEGINNING,
		INPUT_KEY_KEYBOARD_BACKSPACE,
		INPUT_KEY_KEYBOARD_TAB,
		INPUT_KEY_KEYBOARD_CLEAR,
		INPUT_KEY_KEYBOARD_RETURN,
		INPUT_KEY_KEYBOARD_PAUSE,
		INPUT_KEY_KEYBOARD_ESCAPE,
		INPUT_KEY_KEYBOARD_SPACE,
		INPUT_KEY_KEYBOARD_EXCLAIMATION,
		INPUT_KEY_KEYBOARD_QUOTEDOUBLE,
		INPUT_KEY_KEYBOARD_HASH,
		INPUT_KEY_KEYBOARD_DOLLAR,
		INPUT_KEY_KEYBOARD_AMPERSAND,
		INPUT_KEY_KEYBOARD_QUOTE,
		INPUT_KEY_KEYBOARD_LEFTPARENTHESIS,
		INPUT_KEY_KEYBOARD_RIGHTPARENTHESIS,
		INPUT_KEY_KEYBOARD_ASTERISK,
		INPUT_KEY_KEYBOARD_PLUS,
		INPUT_KEY_KEYBOARD_COMMA,
		INPUT_KEY_KEYBOARD_MINUS,
		INPUT_KEY_KEYBOARD_PERIOD,
		INPUT_KEY_KEYBOARD_SLASH,
		INPUT_KEY_KEYBOARD_0,
		INPUT_KEY_KEYBOARD_1,
		INPUT_KEY_KEYBOARD_2,
		INPUT_KEY_KEYBOARD_3,
		INPUT_KEY_KEYBOARD_4,
		INPUT_KEY_KEYBOARD_5,
		INPUT_KEY_KEYBOARD_6,
		INPUT_KEY_KEYBOARD_7,
		INPUT_KEY_KEYBOARD_8,
		INPUT_KEY_KEYBOARD_9,
		INPUT_KEY_KEYBOARD_COLON,
		INPUT_KEY_KEYBOARD_SEMICOLON,
		INPUT_KEY_KEYBOARD_LESS,
		INPUT_KEY_KEYBOARD_EQUALS,
		INPUT_KEY_KEYBOARD_GREATER,
		INPUT_KEY_KEYBOARD_QUESTION,
		INPUT_KEY_KEYBOARD_AT,
		INPUT_KEY_KEYBOARD_LEFTBRACKET,
		INPUT_KEY_KEYBOARD_BACKSLASH,
		INPUT_KEY_KEYBOARD_RIGHTBRACKET,
		INPUT_KEY_KEYBOARD_CARET,
		INPUT_KEY_KEYBOARD_UNDERSCORE,
		INPUT_KEY_KEYBOARD_BACKQUOTE,
		INPUT_KEY_KEYBOARD_A,
		INPUT_KEY_KEYBOARD_B,
		INPUT_KEY_KEYBOARD_C,
		INPUT_KEY_KEYBOARD_D,
		INPUT_KEY_KEYBOARD_E,
		INPUT_KEY_KEYBOARD_F,
		INPUT_KEY_KEYBOARD_G,
		INPUT_KEY_KEYBOARD_H,
		INPUT_KEY_KEYBOARD_I,
		INPUT_KEY_KEYBOARD_J,
		INPUT_KEY_KEYBOARD_K,
		INPUT_KEY_KEYBOARD_L,
		INPUT_KEY_KEYBOARD_M,
		INPUT_KEY_KEYBOARD_N,
		INPUT_KEY_KEYBOARD_O,
		INPUT_KEY_KEYBOARD_P,
		INPUT_KEY_KEYBOARD_Q,
		INPUT_KEY_KEYBOARD_R,
		INPUT_KEY_KEYBOARD_S,
		INPUT_KEY_KEYBOARD_T,
		INPUT_KEY_KEYBOARD_U,
		INPUT_KEY_KEYBOARD_V,
		INPUT_KEY_KEYBOARD_W,
		INPUT_KEY_KEYBOARD_X,
		INPUT_KEY_KEYBOARD_Y,
		INPUT_KEY_KEYBOARD_Z,
		INPUT_KEY_KEYBOARD_DELETE,
		INPUT_KEY_KEYBOARD_KP0,
		INPUT_KEY_KEYBOARD_KP1,
		INPUT_KEY_KEYBOARD_KP2,
		INPUT_KEY_KEYBOARD_KP3,
		INPUT_KEY_KEYBOARD_KP4,
		INPUT_KEY_KEYBOARD_KP5,
		INPUT_KEY_KEYBOARD_KP6,
		INPUT_KEY_KEYBOARD_KP7,
		INPUT_KEY_KEYBOARD_KP8,
		INPUT_KEY_KEYBOARD_KP9,
		INPUT_KEY_KEYBOARD_KP_PERIOD,
		INPUT_KEY_KEYBOARD_KP_DIVIDE,
		INPUT_KEY_KEYBOARD_KP_MULTIPLY,
		INPUT_KEY_KEYBOARD_KP_MINUS,
		INPUT_KEY_KEYBOARD_KP_PLUS,
		INPUT_KEY_KEYBOARD_KP_ENTER,
		INPUT_KEY_KEYBOARD_KP_EQUALS,
		INPUT_KEY_KEYBOARD_UP,
		INPUT_KEY_KEYBOARD_DOWN,
		INPUT_KEY_KEYBOARD_RIGHT,
		INPUT_KEY_KEYBOARD_LEFT,
		INPUT_KEY_KEYBOARD_INSERT,
		INPUT_KEY_KEYBOARD_HOME,
		INPUT_KEY_KEYBOARD_END,
		INPUT_KEY_KEYBOARD_PAGEUP,
		INPUT_KEY_KEYBOARD_PAGEDOWN,
		INPUT_KEY_KEYBOARD_F1,
		INPUT_KEY_KEYBOARD_F2,
		INPUT_KEY_KEYBOARD_F3,
		INPUT_KEY_KEYBOARD_F4,
		INPUT_KEY_KEYBOARD_F5,
		INPUT_KEY_KEYBOARD_F6,
		INPUT_KEY_KEYBOARD_F7,
		INPUT_KEY_KEYBOARD_F8,
		INPUT_KEY_KEYBOARD_F9,
		INPUT_KEY_KEYBOARD_F10,
		INPUT_KEY_KEYBOARD_F11,
		INPUT_KEY_KEYBOARD_F12,
		INPUT_KEY_KEYBOARD_F13,
		INPUT_KEY_KEYBOARD_F14,
		INPUT_KEY_KEYBOARD_F15,
		INPUT_KEY_KEYBOARD_NUMLOCK,
		INPUT_KEY_KEYBOARD_CAPSLOCK,
		INPUT_KEY_KEYBOARD_SCROLLOCK,
		INPUT_KEY_KEYBOARD_RSHIFT,
		INPUT_KEY_KEYBOARD_LSHIFT,
		INPUT_KEY_KEYBOARD_RCTRL,
		INPUT_KEY_KEYBOARD_LCTRL,
		INPUT_KEY_KEYBOARD_RALT,
		INPUT_KEY_KEYBOARD_LALT,
		INPUT_KEY_KEYBOARD_RMETA,
		INPUT_KEY_KEYBOARD_LMETA,
		INPUT_KEY_KEYBOARD_LSUPER,
		INPUT_KEY_KEYBOARD_RSUPER,
		INPUT_KEY_KEYBOARD_ALTGR,
		INPUT_KEY_KEYBOARD_COMPOSE,
		INPUT_KEY_KEYBOARD_ENDING,
		INPUT_KEY_MOUSE_BEGINNING,
		INPUT_KEY_MOUSE_WHEELUP,
		INPUT_KEY_MOUSE_WHEELDOWN,
		INPUT_KEY_MOUSE_BUTTON0,
		INPUT_KEY_MOUSE_BUTTON1,
		INPUT_KEY_MOUSE_BUTTON2,
		INPUT_KEY_MOUSE_BUTTON3,
		INPUT_KEY_MOUSE_BUTTON4,
		INPUT_KEY_MOUSE_ENDING
	};

	enum EInputKeyGamepad {
		INPUT_KEY_GAMEPAD_BUTTON0,
		INPUT_KEY_GAMEPAD_BUTTON1,
		INPUT_KEY_GAMEPAD_BUTTON2,
		INPUT_KEY_GAMEPAD_BUTTON3,
		INPUT_KEY_GAMEPAD_BUTTON4,
		INPUT_KEY_GAMEPAD_BUTTON5,
		INPUT_KEY_GAMEPAD_BUTTON6,
		INPUT_KEY_GAMEPAD_BUTTON7,
		INPUT_KEY_GAMEPAD_BUTTON8,
		INPUT_KEY_GAMEPAD_BUTTON9,
		INPUT_KEY_GAMEPAD_BUTTON10,
		INPUT_KEY_GAMEPAD_BUTTON11,
		INPUT_KEY_GAMEPAD_BUTTON12,
		INPUT_KEY_GAMEPAD_BUTTON13,
		INPUT_KEY_GAMEPAD_BUTTON14,
		INPUT_KEY_GAMEPAD_BUTTON15,
		INPUT_KEY_GAMEPAD_BUTTON16,
		INPUT_KEY_GAMEPAD_BUTTON17,
		INPUT_KEY_GAMEPAD_BUTTON18,
		INPUT_KEY_GAMEPAD_BUTTON19,
		INPUT_KEY_GAMEPAD_BUTTON20,
		INPUT_KEY_GAMEPAD_BUTTON21,
		INPUT_KEY_GAMEPAD_BUTTON22,
		INPUT_KEY_GAMEPAD_BUTTON23,
		INPUT_KEY_GAMEPAD_BUTTON24,
		INPUT_KEY_GAMEPAD_BUTTON25,
		INPUT_KEY_GAMEPAD_BUTTON26,
		INPUT_KEY_GAMEPAD_BUTTON27,
		INPUT_KEY_GAMEPAD_BUTTON28,
		INPUT_KEY_GAMEPAD_BUTTON29,
		INPUT_KEY_GAMEPAD_BUTTON30,
		INPUT_KEY_GAMEPAD_BUTTON31,
		INPUT_KEY_GAMEPAD_BUTTON_END,
		INPUT_KEY_GAMEPAD_AXIS_LXP,
		INPUT_KEY_GAMEPAD_AXIS_LXN,
		INPUT_KEY_GAMEPAD_AXIS_LYP,
		INPUT_KEY_GAMEPAD_AXIS_LYN,
		INPUT_KEY_GAMEPAD_AXIS_LZP,
		INPUT_KEY_GAMEPAD_AXIS_LZN,
		INPUT_KEY_GAMEPAD_AXIS_RXP,
		INPUT_KEY_GAMEPAD_AXIS_RXN,
		INPUT_KEY_GAMEPAD_AXIS_RYP,
		INPUT_KEY_GAMEPAD_AXIS_RYN,
		INPUT_KEY_GAMEPAD_AXIS_RZP,
		INPUT_KEY_GAMEPAD_AXIS_RZN,
		INPUT_KEY_GAMEPAD_POV_UP,
		INPUT_KEY_GAMEPAD_POV_DOWN,
		INPUT_KEY_GAMEPAD_POV_LEFT,
		INPUT_KEY_GAMEPAD_POV_RIGHT
	};

	enum EInputKeyType {
		INPUT_TYPE_KEYBOARD,
		INPUT_TYPE_MOUSE,
		INPUT_TYPE_GAMEPAD
	};

	enum EInputKeyState {
		INPUT_KEY_STATE_UP,
		INPUT_KEY_STATE_DOWN,
		INPUT_KEY_STATE_FROZEN,
		INPUT_KEY_STATE_DISABLED
	};

	/**
	* Input key structure
	*/
	struct SInputKey {
		EInputKeyType type;
		unsigned int key;
	};

	/**
	* Input action structure
	*/
	struct SInputAction {
		int actionId;
		int linkedActionId;
		float actionFactor;

		std::vector<SInputKey> keys;
		EInputKeyState state;
	};

	/**
	* Input action declaration structure
	*/
	struct SInputActionDeclaration {
		int actionId;
		int linkedActionId;
	};

	/**
	* The inputs device class.
	* Handle the input events and call the registered listeners when necessary.
	*/
	class CInputDevice : public CSingletonManager < CInputDevice > {
		friend class CSingletonManager < CInputDevice > ;

	private:
		std::map<unsigned int, SInputAction> m_actions;
		std::array<std::map<unsigned int, SInputAction>, MAX_GAMEPADS> m_gamepadActions;

		CVector2I m_mousePosition;
		CVector2I m_mouseMovement;

		std::vector<IInputKeyboardListener *> m_keyboardListeners;
		std::vector<IInputMouseListener *> m_mouseListeners;
		std::map<unsigned int, std::vector<IInputGamepadListener *> > m_gamepadListeners;

	public:
		/**
		* Add a list of actions to the input device.
		* @param declaration The action list.
		*/
		template <std::size_t N> void AddActions(const SInputActionDeclaration(&declaration)[N]);

		/**
		* Add an action to the input device
		* @param declaration An action declaration
		*/
		void AddAction(const SInputActionDeclaration & declaration);

		/**
		* Add a keyboard action
		* @param action The action name
		* @param key The keyboard key
		*/
		void SetActionKey(const unsigned int action, EInputKey key);

		/**
		* Add a gamepad action
		* @param action The action name
		* @param gamepadId The gamepad device id
		* @param key The gamepad key
		*/
		void SetGamepadActionKey(const unsigned int action, const unsigned int gamepadId, EInputKeyGamepad key);

		/**
		* Add a keyboard listener to the listeners list
		* @param listener The keyboard listener
		*/
		void AddKeyboardListener(IInputKeyboardListener * listener);

		/**
		* Remove a keyboard listener from the listeners list
		* @param listener The keyboard listener
		*/
		void RemoveKeyboardListener(IInputKeyboardListener * listener);

		/**
		* Add a mouse listener to the listeners list
		* @param listener The mouse listener
		*/
		void AddMouseListener(IInputMouseListener * listener);

		/**
		* Add a gamepad listener to the listeners list
		* @param gamepadId The gamepad id
		* @param listener The gamepad listener
		*/
		void AddGamepadListener(const unsigned int gamepadId, IInputGamepadListener * listener);

		/**
		* Clear all the actions of a key type
		* @param action The action name
		* @param type The key type
		* @param index The device id
		*/
		void ClearActionKeys(const unsigned int action, EInputKeyType type, const int index);

		/**
		* Change the mouse position and movement
		* Trigger the mouse listeners
		* @param position The new mouse position
		* @param movement The movement
		*/
		void MoveMouse(const CVector2I & position, const CVector2I& movement);

		/**
		* Set the mouse position
		* Trigger the mouse listeners
		* @param position The new mouse position
		* @param triggerEvent If the event triggers the listeners
		*/
		void SetMousePosition(const CVector2I & position, const bool triggerEvent = true);

		/**
		* Move the mouse.
		* Trigger the mouse listeners
		* @param movement The movement
		*/
		void SetMouseMotion(const CVector2I & movement);

		/**
		* Set a keyboard key to up
		* @param key The keyboard key
		* @param ascii The ascii value
		*/
		void SetKeyUp(EInputKey key, const unsigned int ascii = 0);

		/**
		* Set a keyboard key to down
		* @param key The keyboard key
		* @param ascii The ascii value
		*/
		void SetKeyDown(EInputKey key, const unsigned int ascii = 0);

		/**
		* Set a gamepad key
		* @param gamepadId The gamepadId
		* @param key The gamepad key
		* @param factor The key factor (between -1 and 1)
		*/
		void SetGamepadKey(const unsigned int gamepadId, EInputKeyGamepad key, const float factor);

		/**
		* Reset the mouse movement
		*/
		void ResetMouseMovement();

		/**
		* Freeze an active action, it will be avaible after the associated key is up
		* @param action The action name
		*/
		void FreezeAction(const unsigned int action);

		/**
		* Freeze an active action, it will be avaible after the associated key is up
		* @param gamepadId The gamepad id
		* @param action The action name
		*/
		void FreezeGamepadAction(const unsigned int gamepadId, const unsigned int action);

		/**
		* Check if an action is active
		* @param action The action name
		* @return True if the action is active
		*/
		bool IsActionActive(const unsigned int action) const;

		/**
		* Check if a gamepad action is active
		* @param gamepadId The gamepad id
		* @param action The action name
		* @return True if the action is active
		*/
		bool IsGamepadActionActive(const unsigned int gamepadId, const unsigned int action) const;

		/**
		* Get the factor of a gamepad action
		* @param gamepadId The gamepad id
		* @param action The action name
		* @return The factor
		*/
		float GetGamepadActionFactor(const unsigned int gamepadId, const unsigned int action) const;

		/**
		* Get the mouse position
		* @return The mouse position
		*/
		const CVector2I & GetMousePosition() const;

		/**
		* Get the mouse movement
		* @return The mouse movement
		*/
		const CVector2I & GetMouseMovement() const;

	private:
		/**
		* Set a key up
		* Make the changes in the actions by starting and stopping the associated action
		* @param action The action map
		* @param type The key type
		* @param key The key
		*/
		void SetKeyUp(std::map<unsigned int, SInputAction> & action, EInputKeyType type, const unsigned int key);

		/**
		* Set a key down
		* Make the changes in the actions by starting and stopping the associated action
		* @param action The action map
		* @param type The key type
		* @param key The key
		* @param the factor
		*/
		void SetKeyDown(std::map<unsigned int, SInputAction> & action, EInputKeyType type, const unsigned int key, const float factor = 1);

	private:
		/**
		* Constructor
		*/
		CInputDevice();

	public:
		CInputDevice(const CInputDevice & copy) = delete;
		const CInputDevice & operator=(const CInputDevice & copy) = delete;
	};


	template <std::size_t N> void CInputDevice::AddActions(const SInputActionDeclaration(&declaration)[N]) {
		for(unsigned int i = 0; i < N; ++i)
			AddAction(declaration[i]);
	}
}

#endif
