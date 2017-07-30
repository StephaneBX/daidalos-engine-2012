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

#ifndef __CDX9TEXTURE2D_HPP
#define __CDX9TEXTURE2D_HPP

#include <d3d9.h>
#include <d3dx9.h>
#include <memory>
#include "../../Medias/ITexture2DBase.hpp"

namespace DE {
	/**
	* DX9 class used for 2D textures
	*/
	class CDX9Texture2D : public ITexture2DBase {
	private:
		IDirect3DTexture9 * m_texture;

	public:
		/**
		* Constructor
		* @param size The texture size
		* @param filter The filter policy
		* @param format The texture format
		* @param mipmap Mipmaps enabled
		* @param autoMipmap Hardware mipmaps enabled
		* @param texture The DX9 texture
		*/
		CDX9Texture2D(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const bool mipmap, const bool autoMipmap, IDirect3DTexture9 * texture);

		/**
		* Destructor
		*/
		virtual ~CDX9Texture2D();

		/**
		* Get the DX9 texture
		* @return The DX9 texture
		*/
		IDirect3DTexture9 * GetTexture() const;

		/**
		* Update the texture
		* @param rect The rectangle representing the surface to update
		*/
		virtual void Update(const CRectangleI & rect) override;

	private:
		/**
		* Update the surface
		* @param lockedRect The locked rect pointing to the bits
		* @param rect The rectangle representing the surface to update
		*/
		void UpdateSurface(const D3DLOCKED_RECT & lockedRect, const CRectangleI & rect);

	private:
		CDX9Texture2D();
		CDX9Texture2D(const CDX9Texture2D & copy);
		const CDX9Texture2D & operator=(const CDX9Texture2D & copy);
	};
}

#endif
