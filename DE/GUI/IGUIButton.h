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

#ifndef __IGUIBUTTON_H
#define __IGUIBUTTON_H

#include "../GUI/IGUIView.h"
#include "../Core/Utility/CInset.h"
#include "../Medias/CTexture2D.h"

namespace DE
{
	/**
	* Basic class for gui button
	*/
	class IGUIButton : public IGUIView
	{
	private:
		bool m_enabled;
		bool m_clicked;
		CInsetF m_imageInset;
		CTexture2D m_defaultTexture;
		CTexture2D m_focusedTexture;
		CTexture2D m_clickedTexture;

	public:
		/**
		* Constructor
		* @param text The texture name
		* @param font The font name
		*/
		IGUIButton(const std::string & texture);

		/**
		* Set the enabled state
		* @param enabled The enabled state
		*/
		void SetEnabled(const bool enabled);

		/**
		* Set the image inset
		* The inset represents the parts of the button that will not be resized with the button
		* The values are between 0 and 1
		*/
		void SetImageInset(const CInsetF & inset);

		/**
		* Set the default texture
		*/
		void SetDefaultTexture(const std::string & texture);

		/**
		* Set the clicked texture
		*/
		void SetClickedTexture(const std::string & texture);

		/**
		* Set the focused texture
		*/
		void SetFocusedTexture(const std::string & texture);

		/**
		* Update the state and the geometry
		*/
		void Update();

		/**
		* Render the button
		*/
		void Render();

		/**
		* Call when a left click occurred
		*/
		void LeftClick();

		/**
		* Call when a right click occurred
		*/
		void RightClick();

		/**
		* Call when a left unclick occurred
		*/
		void LeftClickRelease();

		/**
		* Call when a right unclick occurred
		*/
		void RightClickRelease();

		/**
		* Check if the button is enabled
		* @return True if the button is enabled, false otherwise
		*/
		const bool IsEnabled() const;

	private:
		/**
		* Generate the geometry
		*/
		void GenerateGeometry();

		/**
		* Get the texture to used for rendering
		*/
		ITexture2DBase * GetTextureToRender();
	};
}

#endif
