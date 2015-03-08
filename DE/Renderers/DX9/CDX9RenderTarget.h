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

#ifndef __CDX9RENDERTARGET_H
#define __CDX9RENDERTARGET_H

#include <d3d9.h>
#include <d3dx9.h>
#include "../../Renderers/DX9/CDX9Texture2D.h"
#include "../../Medias/IRenderTargetBase.h"

namespace DE {
	/**
	* DX9 class used for render targets
	*/
	class CDX9RenderTarget : public IRenderTargetBase {
	private:
		IDirect3DSurface9 * m_renderTarget;

	public:
		/**
		* Constructor
		* @param type The type
		*/
		CDX9RenderTarget(ERenderTargetType type);

		/**
		* Constructor
		* @param renderTarget The render target
		* @param type The type
		*/
		CDX9RenderTarget(IDirect3DSurface9 * renderTarget, ERenderTargetType type);

		/**
		* Destructor
		*/
		~CDX9RenderTarget();

		/**
		* Link the render target to a texture
		*/
		void LinkWithTexture(CDX9Texture2D * texture);

		/**
		* Get the render target
		* @return The render target
		*/
		IDirect3DSurface9 * GetRenderTarget() const;

	private:
		CDX9RenderTarget();
		CDX9RenderTarget(const CDX9RenderTarget & copy);
		const CDX9RenderTarget & operator=(const CDX9RenderTarget & copy);
	};
}

#endif
