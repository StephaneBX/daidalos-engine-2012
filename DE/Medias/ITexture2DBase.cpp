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

#include "../Core/Logger/ILogger.hpp"
#include "../Medias/ITexture2DBase.hpp"

namespace daidalosengine {
	ITexture2DBase::~ITexture2DBase() {
		LOG_TRACE();
	}

	/**
	* Check if the the texture has mipmaps
	* @return True if the texture has mipmaps
	*/
	bool ITexture2DBase::HasMipmaps() const {
		return this->m_mipmaps;
	}

	/**
	* Check if the auto mipmap is enabled
	* @return True if the auto mipmap is enabled
	*/
	bool ITexture2DBase::HasAutoMimap() const {
		return this->m_autoMipmap;
	}

	/**
	* Get the texture size
	* @return The texture size
	*/
	const CVector2I & ITexture2DBase::GetSize() const {
		return this->m_size;
	}

	/**
	* Get the texture format
	* @return The texture format
	*/
	EPixelFormat ITexture2DBase::GetFormat() const {
		return this->m_format;
	}

	/**
	* Get the texture image
	* @return The texture image
	*/
	const CImage & ITexture2DBase::GetImage() const {
		return this->m_image;
	}

	const STexture2DFilterPolicy & ITexture2DBase::GetTextureFilterPolicy() const {
		LOG_TRACE();

		return this->m_filter;
	}

	ITexture2DBase::ITexture2DBase(const CVector2I& size, const STexture2DFilterPolicy & filter, const EPixelFormat format, const bool mipmap, const bool autoMipmap) : IResource("texture") {
		LOG_TRACE();

		this->m_size = size;
		this->m_filter = filter;
		this->m_format = format;
		this->m_mipmaps = mipmap;
		this->m_autoMipmap = autoMipmap;
		this->m_image = CImage(this->m_size, this->m_format);
	}
}
