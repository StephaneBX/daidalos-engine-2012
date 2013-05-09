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

#ifndef __COGLTEXTURE2D_H
#define __COGLTEXTURE2D_H

#include <memory>
#include "../../Medias/ITexture2DBase.h"

namespace DE
{
	/**
	* OGL class used for 2D textures
	*/
	class COGLTexture2D : public ITexture2DBase
	{
	private:
		unsigned int m_texture;

	public:
		/**
		* Constructor
		* @param size The texture size
		* @param filter The filter policy
		* @param format The texture format
		* @param mipmap Mipmaps enabled
		* @param autoMipmap Hardware mipmaps enabled
		* @param texture The OGL texture
		*/
		COGLTexture2D(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const bool mipmap, const bool autoMipmap, const unsigned int texture);

		/**
		* Destructor
		*/
		~COGLTexture2D();

		/**
		* Get the OGL texture
		* @return The OGL texture
		*/
		const unsigned int GetTexture() const;

		/**
		* Update the texture
		* @param rect The rectangle representing the surface to update
		*/
		void Update(const CRectangleI & rect);

	private:
		COGLTexture2D();
		COGLTexture2D(const COGLTexture2D & copy);
		const COGLTexture2D & operator=(const COGLTexture2D & copy);
	};
}

#endif
