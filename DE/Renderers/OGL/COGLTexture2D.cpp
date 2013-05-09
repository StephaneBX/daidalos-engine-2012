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

#include <Windows.h>
#include <gl/GL.h>
#include "../../Core/Core.h"
#include "../../Renderers/OGL/COGLRenderer.h"
#include "../../Renderers/OGL/COGLEnums.h"
#include "../../Renderers/OGL/COGLTexture2D.h"

namespace DE
{
	COGLTexture2D::COGLTexture2D(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const bool mipmap, const bool autoMipmap, const unsigned int texture) : ITexture2DBase(size, filter, format, mipmap, autoMipmap)
	{
		this->m_texture = texture;
	}

	COGLTexture2D::~COGLTexture2D()
	{
		if(this->m_texture)
			glDeleteTextures(1, &this->m_texture);
	}

	const unsigned int COGLTexture2D::GetTexture() const
	{
		return this->m_texture;
	}

	void COGLTexture2D::Update(const CRectangleI & rect)
	{
		// Get the formats
		SOGLPixelFmt textureFormat = COGLEnums::Get(this->m_format);
		SOGLPixelFmt imageFormat = COGLEnums::Get(this->m_image.GetFormat());

		glBindTexture(GL_TEXTURE_2D, this->m_texture);

		// Check if the format is compressed
		if(IsFormatCompressed(this->m_image.GetFormat()))
		{
			// Get the data size
			const unsigned long dataSize = rect.GetWidth() * rect.GetHeight() * GetBytesPerPixel(this->m_image.GetFormat());

			
			if(rect.GetWidth() == this->m_size.x && rect.GetHeight() == this->m_size.y)
			{
				// Update the whole texture
				COGLRenderer::glCompressedTexSubImage2DARB(GL_TEXTURE_2D, 0, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), imageFormat.format, dataSize, this->m_image.GetData());
			}
			else
			{
				// Update a part of the texture
				CImage tmp = this->m_image.SubImage(rect);
				COGLRenderer::glCompressedTexSubImage2DARB(GL_TEXTURE_2D, 0, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), imageFormat.format, dataSize, tmp.GetData());
			}
		}
		else
		{
			if(rect.GetWidth() == this->m_size.x && rect.GetHeight() == this->m_size.y)
			{
				// Update the whole texture
				glTexSubImage2D(GL_TEXTURE_2D, 0, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), imageFormat.format, imageFormat.type, this->m_image.GetData());
			}
			else
			{
				// Update a part of the texture
				CImage tmp = this->m_image.SubImage(rect);
				glTexSubImage2D(GL_TEXTURE_2D, 0, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), imageFormat.format, imageFormat.type, tmp.GetData());
			}

			if(this->m_autoMipmap)
				COGLRenderer::glGenerateMipmap(GL_TEXTURE_2D);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
