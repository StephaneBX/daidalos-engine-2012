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

#ifndef __CDX9ENUMS_H
#define __CDX9ENUMS_H

#include <d3d9.h>
#include "../../Core/Enums.h"

namespace DE {
	/**
	* DX9 class used for enums
	*/
	class CDX9Enums {
	private:
		static D3DPRIMITIVETYPE s_primitiveType[];
		static D3DFORMAT s_formatType[];
		static D3DTEXTUREFILTERTYPE s_textureFilterType[];
		static D3DTEXTUREADDRESS s_textureAddressType[];
		static D3DRENDERSTATETYPE s_renderStateType[];
		static D3DCMPFUNC s_compare[];
		static D3DBLEND s_renderStateBlendFunction[];
		static D3DBLENDOP s_renderStateBlendOperation[];
		static D3DCULL s_renderStateCullType[];
		static D3DSTENCILOP s_stencilOperation[];

	public:
		/**
		* Get the DX9 buffer flags corresponding to the DE flag
		* @param flags The DE flags
		* @return The DX9 buffer flag
		*/
		static const unsigned long BufferFlags(const unsigned long flags);

		/**
		* Get the DX9 lock flags corresponding to the DE flag
		* @param flags The DE flags
		* @return The DX9 lock flag
		*/
		static const unsigned long LockFlags(const unsigned long flags);

		/**
		* Get the DX9 surface flags corresponding to the DE flag
		* @param flags The DE flags
		* @return The DX9 lock flag
		*/
		static const unsigned long SurfaceFlags(const unsigned long flags);

		/**
		* Get the DX9 primitive type
		* @param type The CE primitive
		* @return The DX9 primitive type
		*/
		static D3DPRIMITIVETYPE Get(EPrimitiveType type);

		/**
		* Get the DX9 format type
		* @param type The format type
		* @return The DX9 format type
		*/
		static D3DFORMAT Get(EPixelFormat type);

		/**
		* Get the DX9 texture filter type
		* @param type The texture type
		* @return The DX9 texture type
		*/
		static D3DTEXTUREFILTERTYPE Get(ETextureFilterType type);

		/**
		* Get the DX9 texture address type
		* @param type The texture address type
		* @return The DX9 texture address type
		*/
		static D3DTEXTUREADDRESS Get(ETextureAddress type);

		/**
		* Get the DX9 render state type
		* @param type The render state type
		* @return The DX9 render state type
		*/
		static D3DRENDERSTATETYPE Get(ERenderStateType type);

		/**
		* Get the DX9 compare type
		* @param compare The compare
		* @return The DX9 compare
		*/
		static D3DCMPFUNC Get(ECompare compare);

		/**
		* Get the DX9 render state blend function
		* @param type The render state blend function
		* @return The DX9 render state blend function
		*/
		static D3DBLEND Get(ERenderStateBlendFunction type);

		/**
		* Get the DX9 render state blend operation
		* @param type The render state blend operation
		* @return The DX9 render state blend operation
		*/
		static D3DBLENDOP Get(ERenderStateBlendOperation type);

		/**
		* Get the DX9 render state cull type
		* @param type The render state cull type
		*/
		static D3DCULL Get(ERenderStateCullType type);

		/**
		* Get the DX9 stencil operation
		* @param operation The render stencil operation
		* @return The DX9 stencil operation
		*/
		static D3DSTENCILOP Get(EStencilOperation operation);
	};
}

#endif
