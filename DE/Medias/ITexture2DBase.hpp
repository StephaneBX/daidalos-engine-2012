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

#ifndef __ITEXTURE2DBASE_HPP
#define __ITEXTURE2DBASE_HPP

#include "../Core/Utility/CVector2.hpp"
#include "../Core/Utility/CImage.hpp"
#include "../Core/Utility/CRectangle.hpp"
#include "../Core/IResource.hpp"
#include "../Core/Enums.hpp"

namespace daidalosengine {
	/**
	* Texture filter policy structure
	*/
	struct STexture2DFilterPolicy {
		ETextureFilterType minFilter;
		ETextureFilterType magFilter;
		ETextureFilterType mipFilter;
		ETextureAddress addressU;
		ETextureAddress addressV;
		unsigned int maxAnisotropy;
	};

	/**
	* Basic class used for 2D textures
	*/
	class ITexture2DBase : public IResource {
	protected:
		friend class CTexture2D;

	private:
		bool m_mipmaps;
		bool m_autoMipmap;
		CVector2I m_size;
		EPixelFormat m_format;
		CImage m_image;
		STexture2DFilterPolicy m_filter;

	public:
		/**
		* Destructor
		*/
		virtual ~ITexture2DBase();

		/**
		* Check if the the texture has mipmaps
		* @return True if the texture has mipmaps
		*/
		bool HasMipmaps() const;

		/**
		* Check if the auto mipmap is enabled
		* @return True if the auto mipmap is enabled
		*/
		bool HasAutoMimap() const;

		/**
		* Get the texture size
		* @return The texture size
		*/
		const CVector2I & GetSize() const;

		/**
		* Get the texture format
		* @return The texture format
		*/
		EPixelFormat GetFormat() const;

		/**
		* Get the texture image
		* @return The texture image
		*/
		const CImage & GetImage() const;

		/**
		* Get the filter policy
		* @return The filter policy
		*/
		const STexture2DFilterPolicy & GetTextureFilterPolicy() const;

	protected:
		/**
		* Constructor
		* @param size The texture size
		* @param filter The filter policy
		* @param format The texture format
		* @param mipmap Mipmaps enabled
		* @param autoMipmap Hardware mipmaps enabled
		*/
		ITexture2DBase(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const bool mipmap, const bool autoMipmap);

		/**
		* Update the texture
		* @param rect The rectangle representing the surface to update
		*/
		virtual void Update(const CRectangleI & rect) = 0;

	public:
		ITexture2DBase() = delete;
		ITexture2DBase(const ITexture2DBase & copy) = delete;
		const ITexture2DBase & operator=(const ITexture2DBase & copy) = delete;
	};
}

#endif
