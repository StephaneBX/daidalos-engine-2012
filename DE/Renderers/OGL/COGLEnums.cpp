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

#include "../../Renderers/OGL/COGLEnums.h"

namespace DE
{
	SOGLPixelFmt COGLEnums::s_formatType[] =
	{
		{GL_LUMINANCE,       GL_LUMINANCE8,                    GL_UNSIGNED_BYTE},
		{GL_LUMINANCE_ALPHA, GL_LUMINANCE8_ALPHA8,             GL_UNSIGNED_BYTE},
		{GL_BGRA,            GL_RGB5_A1,                       GL_UNSIGNED_SHORT_1_5_5_5_REV},
		{GL_BGRA,            GL_RGBA4,                         GL_UNSIGNED_SHORT_4_4_4_4_REV},
		{GL_BGR,             GL_RGB8,                          GL_UNSIGNED_BYTE},
		{GL_BGRA,            GL_RGBA8,                         GL_UNSIGNED_BYTE},
		{GL_RED,			 GL_R16F,						   GL_FLOAT},
		{GL_RED,			 GL_R32F,						   GL_FLOAT},
		{GL_BGR,             GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 0},
		{GL_BGRA,            GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 0},
		{GL_BGRA,            GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0},
		{GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16,			   GL_FLOAT},
		{GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32,			   GL_FLOAT},
		{GL_DEPTH_STENCIL,	 GL_DEPTH24_STENCIL8,			   GL_UNSIGNED_INT_24_8}
	};

	GLenum COGLEnums::s_textureFilterType[] =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR
	};

	GLenum COGLEnums::s_textureAddressType[] =
	{
		GL_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_CLAMP,
		GL_CLAMP_TO_BORDER,
		GL_MIRRORED_REPEAT
	};

	GLenum COGLEnums::s_compare[] =
	{
		GL_NEVER,
		GL_LESS,
		GL_EQUAL,
		GL_LEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_GEQUAL,
		GL_ALWAYS
	};

	GLenum COGLEnums::s_renderStateBlendFunction[] =
	{
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA_SATURATE,
		GL_ZERO,
		GL_ZERO,
		GL_ZERO,
		GL_ZERO,
		GL_ZERO,
		GL_ZERO
	};

	GLenum COGLEnums::s_renderStateBlendOperation[] =
	{
		GL_FUNC_ADD,
		GL_FUNC_SUBTRACT,
		GL_FUNC_REVERSE_SUBTRACT,
		GL_MIN,
		GL_MAX,
	};

	const unsigned long COGLEnums::BufferFlags(const unsigned long flags)
	{
		unsigned long bufferFlags = GL_STATIC_DRAW;

		if((flags & BUFFER_DYNAMIC) != 0)
			bufferFlags = GL_DYNAMIC_DRAW;

		return bufferFlags;
	}

	const unsigned long COGLEnums::LockFlags(const unsigned long flags)
	{
		unsigned long lockFlags = GL_READ_WRITE;

		if ((flags & LOCK_READONLY) != 0)
			lockFlags = GL_READ_ONLY;

		if ((flags & LOCK_WRITEONLY) != 0)
			lockFlags = GL_WRITE_ONLY;

		return lockFlags;
	}

	const unsigned long COGLEnums::SurfaceFlags(const unsigned long flags)
	{
		unsigned long surfaceFlags = 0;

		if ((flags & SURFACE_TYPE_TARGET) != 0)
			surfaceFlags |= GL_COLOR_BUFFER_BIT;

		if ((flags & SURFACE_TYPE_DEPTH) != 0)
			surfaceFlags |= GL_DEPTH_BUFFER_BIT;

		if ((flags & SURFACE_TYPE_STENCIL) != 0)
			surfaceFlags |= GL_STENCIL_BUFFER_BIT;

		return surfaceFlags;
	}

	SOGLPixelFmt COGLEnums::Get(EPixelFormat type)
	{
		return s_formatType[type];
	}

	GLenum COGLEnums::Get(ETextureFilterType type)
	{
		return s_textureFilterType[type];
	}
	
	GLenum COGLEnums::Get(ETextureAddress type)
	{
		return s_textureAddressType[type];
	}

	GLenum COGLEnums::Get(ECompare compare)
	{
		return s_compare[compare];
	}

	GLenum COGLEnums::Get(ERenderStateBlendFunction type)
	{
		return s_renderStateBlendFunction[type];
	}

	GLenum COGLEnums::Get(ERenderStateBlendOperation type)
	{
		return s_renderStateBlendOperation[type];
	}
}