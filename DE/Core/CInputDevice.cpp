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

#include <algorithm>
#include "../Core/Memory/CExceptionAssert.hpp"
#include "../Core/Logger/ILogger.hpp"
#include "../Core/Core.hpp"
#include "../Core/CInputDevice.hpp"

namespace daidalosengine {
	CInputDevice::CInputDevice() {
		LOG_TRACE();
	}

	void CInputDevice::AddAction(const SInputActionDeclaration & declaration) {
		LOG_TRACE();

		SInputAction action;
		action.state = INPUT_KEY_STATE_DISABLED;
		action.actionFactor = 1;

		action.actionId = declaration.actionId;
		action.linkedActionId = declaration.linkedActionId;

		// Main actions
		this->m_actions[declaration.actionId] = action;

		// Gamepad actions
		for(auto &it : this->m_gamepadActions) {
			it[declaration.actionId] = action;
		}
	}

	void CInputDevice::SetActionKey(const unsigned int action, EInputKey key) {
		LOG_TRACE();
		Assert(this->m_actions.find(action) != this->m_actions.end());

		SInputKey tmp;
		if(key > INPUT_KEY_KEYBOARD_BEGINNING && key < INPUT_KEY_KEYBOARD_ENDING)
			tmp.type = INPUT_TYPE_KEYBOARD;
		else
			tmp.type = INPUT_TYPE_MOUSE;
		tmp.key = key;

		this->m_actions[action].keys.push_back(tmp);
		this->m_actions[action].state = INPUT_KEY_STATE_UP;
	}

	void CInputDevice::SetGamepadActionKey(const unsigned int action, const unsigned int gamepadId, EInputKeyGamepad key) {
		LOG_TRACE();

		Assert(this->m_gamepadActions[gamepadId].find(action) != this->m_gamepadActions[gamepadId].end());

		SInputKey tmp;
		tmp.type = INPUT_TYPE_GAMEPAD;
		tmp.key = key;

		this->m_gamepadActions[gamepadId][action].keys.push_back(tmp);
		this->m_gamepadActions[gamepadId][action].state = INPUT_KEY_STATE_UP;
	}

	void CInputDevice::AddKeyboardListener(IInputKeyboardListener * listener) {
		LOG_TRACE();
		this->m_keyboardListeners.push_back(listener);
	}

	void CInputDevice::RemoveKeyboardListener(IInputKeyboardListener * listener) {
		LOG_TRACE();

		auto it = std::find(this->m_keyboardListeners.begin(), this->m_keyboardListeners.end(), listener);
		if(it != this->m_keyboardListeners.end()) {
			this->m_keyboardListeners.erase(it);
		}
	}

	void CInputDevice::AddMouseListener(IInputMouseListener * listener) {
		LOG_TRACE();
		this->m_mouseListeners.push_back(listener);
	}

	void CInputDevice::AddGamepadListener(const unsigned int gamepadId, IInputGamepadListener * listener) {
		LOG_TRACE();
		this->m_gamepadListeners[gamepadId].push_back(listener);
	}

	void CInputDevice::ClearActionKeys(const unsigned int action, EInputKeyType type, const int index) {
		LOG_TRACE();
		Assert(this->m_actions.find(action) != this->m_actions.end());

		for(auto it = this->m_actions[action].keys.begin(); it != this->m_actions[action].keys.end(); ++it) {
			this->m_actions[action].keys.erase(it);
		}
	}

	void CInputDevice::MoveMouse(const CVector2I & position, const CVector2I movement) {
		LOG_TRACE();

		this->m_mousePosition = position;
		this->m_mouseMovement += movement;

		// Call the listeners
		for(auto it = this->m_mouseListeners.begin(); it != this->m_mouseListeners.end(); ++it)
			(*it)->OnInputMouseMoved(this->m_mousePosition, movement);
	}

	void CInputDevice::SetMousePosition(const CVector2I & position, const bool triggerEvent) {
		LOG_TRACE();

		if(triggerEvent) {
			this->m_mouseMovement += position - this->m_mousePosition;
			this->m_mousePosition = position;

			// Call the listeners
			for(auto it = this->m_mouseListeners.begin(); it != this->m_mouseListeners.end(); ++it)
				(*it)->OnInputMouseMoved(this->m_mousePosition, position - this->m_mousePosition);
		} else
			this->m_mousePosition = position;
	}

	void CInputDevice::SetMouseMotion(const CVector2I & movement) {
		LOG_TRACE();

		this->m_mouseMovement = movement;
		this->m_mousePosition += movement;

		// Call the listeners
		for(auto it = this->m_mouseListeners.begin(); it != this->m_mouseListeners.end(); ++it)
			(*it)->OnInputMouseMoved(this->m_mousePosition, this->m_mouseMovement);
	}

	void CInputDevice::SetKeyUp(EInputKey key, const unsigned int ascii) {
		LOG_TRACE();

		if(key > INPUT_KEY_KEYBOARD_BEGINNING && key < INPUT_KEY_KEYBOARD_ENDING) {
			SetKeyUp(this->m_actions, INPUT_TYPE_KEYBOARD, key);

			// Call all the listeners
			for(auto it2 = this->m_keyboardListeners.begin(); it2 != this->m_keyboardListeners.end(); ++it2) {
				(*it2)->OnKeyPressed(ascii);
			}

		} else {
			SetKeyUp(this->m_actions, INPUT_TYPE_MOUSE, key);
		}
	}

	void CInputDevice::SetKeyDown(EInputKey key, const unsigned int ascii) {
		LOG_TRACE();

		if(key > INPUT_KEY_KEYBOARD_BEGINNING && key < INPUT_KEY_KEYBOARD_ENDING) {
			SetKeyDown(this->m_actions, INPUT_TYPE_KEYBOARD, key);
		} else {
			SetKeyDown(this->m_actions, INPUT_TYPE_MOUSE, key);
		}
	}

	void CInputDevice::SetGamepadKey(const unsigned int gamepadId, EInputKeyGamepad key, const float factor) {
		LOG_TRACE();
		Assert(gamepadId >= 0 && gamepadId < this->m_gamepadActions.size());

		if(factor == 0) {
			SetKeyUp(this->m_gamepadActions[gamepadId], INPUT_TYPE_GAMEPAD, key);
		} else {
			SetKeyDown(this->m_gamepadActions[gamepadId], INPUT_TYPE_GAMEPAD, key, factor);
		}
	}

	void CInputDevice::ResetMouseMovement() {
		LOG_TRACE();
		this->m_mouseMovement = CVector2I(0, 0);
	}

	void CInputDevice::FreezeAction(const unsigned int action) {
		if(this->m_actions.find(action) != this->m_actions.end() && this->m_actions[action].state == INPUT_KEY_STATE_DOWN) {
			this->m_actions[action].state = INPUT_KEY_STATE_FROZEN;
		}
	}

	void CInputDevice::FreezeGamepadAction(const unsigned int gamepadId, const unsigned int action) {
		if(this->m_gamepadActions[gamepadId].find(action) != this->m_gamepadActions[gamepadId].end() && this->m_gamepadActions[gamepadId][action].state == INPUT_KEY_STATE_DOWN) {
			this->m_gamepadActions[gamepadId][action].state = INPUT_KEY_STATE_FROZEN;
		}
	}

	bool CInputDevice::IsActionActive(const unsigned int action) const {
		LOG_TRACE();
		Assert(this->m_actions.find(action) != this->m_actions.end());

		if(this->m_actions.find(action) != this->m_actions.end()) {
			return this->m_actions.at(action).state == INPUT_KEY_STATE_DOWN;
		} 
		return false;
	}

	bool CInputDevice::IsGamepadActionActive(const unsigned int gamepadId, const unsigned int action) const {
		LOG_TRACE();
		Assert(this->m_gamepadActions[gamepadId].find(action) != this->m_gamepadActions[gamepadId].end());

		return this->m_gamepadActions[gamepadId].at(action).state == INPUT_KEY_STATE_DOWN;
	}

	float CInputDevice::GetGamepadActionFactor(const unsigned int gamepadId, const unsigned int action) const {
		LOG_TRACE();
		Assert(this->m_gamepadActions[gamepadId].find(action) != this->m_gamepadActions[gamepadId].end());

		return this->m_gamepadActions[gamepadId].at(action).actionFactor;
	}

	const CVector2I & CInputDevice::GetMousePosition() const {
		LOG_TRACE();
		return this->m_mousePosition;
	}

	const CVector2I & CInputDevice::GetMouseMovement() const {
		LOG_TRACE();
		return this->m_mouseMovement;
	}

	void CInputDevice::SetKeyUp(std::map<unsigned int, SInputAction> & action, EInputKeyType type, const unsigned int key) {
		LOG_TRACE();

		// Loop through the actions
		for(auto it = action.begin(); it != action.end(); ++it) {
			bool found = false;

			// Search if the action uses this key
			for(unsigned int i = 0; i < it->second.keys.size() && !found; ++i) {
				if(it->second.keys[i].type == type && it->second.keys[i].key == key)
					found = true;
			}

			// If the action uses this key, make the changes
			if(found) {
				// If the action state is DOWN
				if(it->second.state == INPUT_KEY_STATE_DOWN) {
					// Set it to the state UP
					it->second.state = INPUT_KEY_STATE_UP;
					it->second.actionFactor = 0;

					// Call all the listeners
					for(auto it2 = this->m_keyboardListeners.begin(); it2 != this->m_keyboardListeners.end(); ++it2)
						(*it2)->OnInputActionStopped(it->second.actionId);

					// Check if it has a linked action to the state FROZEN
					if(it->second.linkedActionId != INPUT_NO_LINKED_ACTION && action[it->second.linkedActionId].state == INPUT_KEY_STATE_FROZEN) {
						// Change the state to DOWN
						action[it->second.linkedActionId].state = INPUT_KEY_STATE_DOWN;

						// Call all the listeners
						for(auto it2 = this->m_keyboardListeners.begin(); it2 != this->m_keyboardListeners.end(); ++it2)
							(*it2)->OnInputActionStarted(action[it->second.linkedActionId].actionId);
					}
				} else {
					// If the action state is FROZEN
					if(it->second.state == INPUT_KEY_STATE_FROZEN) {
						// Set it to the state UP
						it->second.state = INPUT_KEY_STATE_UP;

						// Call all the listeners
						for(auto it2 = this->m_keyboardListeners.begin(); it2 != this->m_keyboardListeners.end(); ++it2)
							(*it2)->OnInputActionStopped(it->second.actionId);
					}
				}
			}
		}
	}

	void CInputDevice::SetKeyDown(std::map<unsigned int, SInputAction> & action, EInputKeyType type, const unsigned int key, const float factor) {
		LOG_TRACE();

		// Loop through the actions
		for(auto it = action.begin(); it != action.end(); ++it) {
			bool found = false;

			// Search if the action uses this key
			for(unsigned int i = 0; i < it->second.keys.size() && !found; ++i) {
				if(it->second.keys[i].type == type && it->second.keys[i].key == key)
					found = true;
			}

			// If the action uses this key, make the changes
			if(found) {
				// If the action state is UP
				if(it->second.state == INPUT_KEY_STATE_UP) {
					// Set it to the state DOWN
					it->second.state = INPUT_KEY_STATE_DOWN;


					// Check if it has a linked action to the state DOWN
					if(it->second.linkedActionId != INPUT_NO_LINKED_ACTION && action[it->second.linkedActionId].state == INPUT_KEY_STATE_DOWN) {
						// Change the state to FROZEN
						action[it->second.linkedActionId].state = INPUT_KEY_STATE_FROZEN;

						// Call all the listeners
						for(auto it2 = this->m_keyboardListeners.begin(); it2 != this->m_keyboardListeners.end(); ++it2)
							(*it2)->OnInputActionStopped(action[it->second.linkedActionId].actionId);
					}

					// Call all the listeners
					for(auto it2 = this->m_keyboardListeners.begin(); it2 != this->m_keyboardListeners.end(); ++it2) {
						(*it2)->OnInputActionStarted(it->second.actionId);
					}
				}

				// Update the factor
				it->second.actionFactor = factor;
			}
		}
	}
}
