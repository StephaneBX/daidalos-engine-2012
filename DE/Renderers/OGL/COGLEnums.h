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

#ifndef __COGLENUMS_H
#define __COGLENUMS_H

#include <Windows.h>
#include <gl/GL.h>
#include "../../Renderers/OGL/Extensions/glext.h"
#include "../../Core/Enums.h"

namespace DE
{
	/**
	* OGL Pixel format structure.
	*/
	struct SOGLPixelFmt 
	{
		GLenum format;
		GLenum internalFormat;
		GLenum type;
	};

	/**
	* OGL class used for enums
	*/
	class COGLEnums
	{
	private:
		static SOGLPixelFmt s_formatType[];
		static GLenum s_textureFilterType[];
		static GLenum s_textureAddressType[];
		static GLenum s_compare[];
		static GLenum s_renderStateBlendFunction[];
		static GLenum s_renderStateBlendOperation[];

	public:
		/**
		* Get the OGL buffer flags corresponding to the CE flag
		* @param flags The CE flags
		* @return The OGL buffer flag
		*/
		static const unsigned long BufferFlags(const unsigned long flags);

		/**
		* Get the OGL lock flags corresponding to the CE flag
		* @param flags The CE flags
		* @return The OGL lock flag
		*/
		static const unsigned long LockFlags(const unsigned long flags);

		/**
		* Get the OGL surface flags corresponding to the DE flag
		* @param flags The DE flags
		* @return The OGL lock flag
		*/
		static const unsigned long SurfaceFlags(const unsigned long flags);

		/**
		* Get the OGL format type
		* @param type The format type
		* @return The OGL format type
		*/
		static SOGLPixelFmt Get(EPixelFormat type);

		/**
		* Get the OGL texture filter type
		* @param type The texture type
		* @return The OGL texture type
		*/
		static GLenum Get(ETextureFilterType type);

		/**
		* Get the OGL texture address type
		* @param type The texture address type
		* @return The OGL texture address type 
		*/
		static GLenum Get(ETextureAddress type);

		/**
		* Get the OGL compare type
		* @param compare The compare
		* @return The OGL compare
		*/
		static GLenum Get(ECompare compare);

		/**
		* Get the OGL render state blend function
		* @param type The render state blend function
		* @return The OGL render state blend function
		*/
		static GLenum Get(ERenderStateBlendFunction type);

		/**
		* Get the OGL render state blend operation
		* @param type The render state blend operation
		* @return The OGL render state blend operation
		*/
		static GLenum Get(ERenderStateBlendOperation type);
	};
}

#endif
