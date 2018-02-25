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

#ifndef __IINPUTMOUSELISTENER_HPP
#define __IINPUTMOUSELISTENER_HPP

#include <string>
#include "../../Core/Utility/CVector2.hpp"

namespace daidalosengine {
	/**
	* Mouse inputs listener interface.
	*/
	class IInputMouseListener {
	public:
		/**
		* Called when the mouse has moved
		* @param position The new position
		* @param movement The movement
		*/
		virtual void OnInputMouseMoved(const CVector2I & position, const CVector2I & movement) = 0;

		/**
		* Called when a mouse action is started
		* @param actionId The action id
		*/
		virtual void OnInputActionStarted(const unsigned int actionId) = 0;

		/**
		* Called when a mouse action is stopped
		* @param actionId The action id
		*/
		virtual void OnInputActionStopped(const unsigned int actionId) = 0;
	};
}

#endif
