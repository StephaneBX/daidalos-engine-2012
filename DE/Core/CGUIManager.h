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

#ifndef __CGUIMANAGER_H
#define __CGUIMANAGER_H

#include <set>
#include "../Medias/CShaderProgram.h"
#include "../Core/Utility/CMatrix.h"
#include "../Core/IDeclaration.h"
#include "../Core/IRenderer.h"
#include "../Core/CSingletonManager.h"
#include "../Scene/2D/CSprite.h"
#include "../Scene/2D/CCursor.h"

namespace DE {
	/**
	* Handle the gui elements
	*/
	class CGUIManager : public CSingletonManager <CGUIManager> {
		friend class CSingletonManager <CGUIManager> ;

	private:
		CMatrix m_proj;
		IRenderer * m_renderer;
		DeclarationPtr m_declaration;
		CShaderProgram m_shaderProgram;
		std::unique_ptr<CSprite> m_mainView;
		CCursor m_cursor;

	public:
		/**
		* Destructor
		*/
		virtual ~CGUIManager();

		/**
		* Initialize the manager
		*/
		void Initialize();

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
		* Set the cursor visibility
		* @param visible If the cursor is visible
		*/
		void SetCursorVisible(const bool visible);

		/**
		* Set the cursor texture
		* @param texture The cursor texture
		*/
		void SetCursorTexture(const std::string & texture);

		/**
		* Render the gui
		*/
		void Render();

	private:
		CGUIManager();
	};
}

#endif
