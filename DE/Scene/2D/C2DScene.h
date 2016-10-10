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

#ifndef __C2DSCENE_H
#define __C2DSCENE_H

#include <set>
#include "../../Core/Utility/CMatrix.h"
#include "../../Scene/2D/CSprite.h"
#include "../../Scene/2D/CCursor.h"

namespace DE {
	/**
	* Handle a 2d scene
	*/
	class C2DScene {

	private:
		CMatrix m_proj;
		std::unique_ptr<CSprite> m_mainView;
		CCursor * m_cursor;

	public:
		/**
		* Constructor
		*/
		C2DScene();

		/**
		* Destructor
		*/
		~C2DScene();

		/**
		* Add the element in the main view
		* @param element The gui element
		*/
		void AddToMainView(IView * element);

		/**
		* Remove the element from the main view
		* @param element The gui element
		*/
		void RemoveFromMainView(IView * element);

		/**
		* Set the active cursor
		*/
		void SetCursor(CCursor * cursor);

		/**
		* Update the scene
		* @param time The elapsed time
		*/
		void Update(const float time);

		/**
		* Render the scene
		*/
		void Render();

		/**
		* Get the projection matrix
		*/
		const CMatrix & GetProjMatrix() const;
	};
}

#endif
