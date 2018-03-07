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
#include "../Core/IRenderer.hpp"
#include "../Core/Utility/CImage.hpp"
#include "../Core/Utility/CVector2.hpp"
#include "../Core/Utility/CFile.hpp"
#include "../Core/Core.hpp"
#include "../Core/CResourceManager.hpp"
#include "../Core/CMediaManager.hpp"
#include "../Medias/CTexture2D.hpp"

namespace daidalosengine {
	CTexture2D::CTexture2D() {
		LOG_TRACE();

		this->m_texture = nullptr;
	}

	CTexture2D::CTexture2D(const CTexture2D & texture) {
		LOG_TRACE();

		*this = texture;
	}

	CTexture2D::~CTexture2D() {
		LOG_TRACE();

		if(this->m_texture != nullptr)
			CResourceManager::GetInstance()->ReleaseResource(this->m_texture->GetName());
	}

	const CTexture2D & CTexture2D::operator=(const CTexture2D & texture) {
		LOG_TRACE();

		if (this != &texture) {
			if (this->m_texture != nullptr) {
				CResourceManager::GetInstance()->ReleaseResource(this->m_texture->GetName());
			}

			this->m_texture = texture.GetTexture();

			if (this->m_texture != nullptr) {
				this->m_texture->AddReference();
			}
		}

		return *this;
	}

	const CVector2I & CTexture2D::GetSize() const {
		LOG_TRACE();

		return this->m_texture->m_size;
	}

	const CImage & CTexture2D::GetImage() const {
		LOG_TRACE();

		return this->m_texture->m_image;
	}

	const STexture2DFilterPolicy & CTexture2D::GetTextureFilterPolicy() const {
		LOG_TRACE();

		return this->m_texture->m_filter;
	}

	EPixelFormat CTexture2D::GetFormat() const {
		LOG_TRACE();

		return this->m_texture->m_format;
	}

	ITexture2DBase * CTexture2D::GetTexture() const {
		LOG_TRACE();

		return this->m_texture;
	}

	bool CTexture2D::operator==(const CTexture2D & texture) const {
		LOG_TRACE();

		return this->m_texture == texture.m_texture;
	}

	bool CTexture2D::operator!=(const CTexture2D & texture) const {
		LOG_TRACE();

		return !(*this == texture);
	}

	void CTexture2D::CreateEmpty(const std::string name, const CVector2I & size, EPixelFormat format, const STexture2DFilterPolicy * filter, const unsigned long flags) {
		LOG_TRACE();

		STexture2DFilterPolicy usedFilter;
		if(filter == nullptr) {
			usedFilter.minFilter = TEXTURE_FILTER_TYPE_LINEAR;
			usedFilter.magFilter = TEXTURE_FILTER_TYPE_LINEAR;
			usedFilter.magFilter = TEXTURE_FILTER_TYPE_LINEAR;
			usedFilter.addressU = TEXTURE_ADDRESS_CLAMP;
			usedFilter.addressV = TEXTURE_ADDRESS_CLAMP;
			usedFilter.maxAnisotropy = 0;
		} else
			usedFilter = *filter;

		// Release previously used resource
		if(this->m_texture != nullptr) {
			CResourceManager::GetInstance()->ReleaseResource(this->m_texture->GetName());
		}

		CImage * img = new CImage(size, format);
		Load(*img, usedFilter, format, flags, name);
		delete img;
	}

	void CTexture2D::CreateFromFile(const std::string & filename, EPixelFormat format, const STexture2DFilterPolicy * filter, const unsigned long flags) {
		LOG_TRACE();

		// If no filter policy is given, use the default one
		STexture2DFilterPolicy usedFilter;
		if(filter == nullptr) {
			usedFilter.minFilter = TEXTURE_FILTER_TYPE_LINEAR;
			usedFilter.magFilter = TEXTURE_FILTER_TYPE_LINEAR;
			usedFilter.magFilter = TEXTURE_FILTER_TYPE_LINEAR;
			usedFilter.addressU = TEXTURE_ADDRESS_CLAMP;
			usedFilter.addressV = TEXTURE_ADDRESS_CLAMP;
			usedFilter.maxAnisotropy = 0;
		} else {
			usedFilter = *filter;
		}

		// Release previously used resource
		if(this->m_texture != nullptr) {
			CResourceManager::GetInstance()->ReleaseResource(this->m_texture->GetName());
		}

		// Check the existence of the resource and load it if necessary
		ITexture2DBase * tmp = CResourceManager::GetInstance()->GetResource<ITexture2DBase>(filename);

		if(tmp == nullptr) {
			CImage * img = CMediaManager::GetInstance()->LoadMediaFromFile<CImage>(filename);
			Load(*img, usedFilter, format, flags, filename);
			delete img;
		} else {
			this->m_texture = tmp;
		}
	}

	void CTexture2D::CreateFromImage(const CImage & image, EPixelFormat format, const STexture2DFilterPolicy * filter, const unsigned long flags, const std::string & name) {
		LOG_TRACE();

		// If no filter policy is given, use the default one
		STexture2DFilterPolicy usedFilter;
		if(filter == nullptr) {
			usedFilter.minFilter = TEXTURE_FILTER_TYPE_LINEAR;
			usedFilter.magFilter = TEXTURE_FILTER_TYPE_LINEAR;
			usedFilter.magFilter = TEXTURE_FILTER_TYPE_LINEAR;
			usedFilter.addressU = TEXTURE_ADDRESS_CLAMP;
			usedFilter.addressV = TEXTURE_ADDRESS_CLAMP;
			usedFilter.maxAnisotropy = 0;
		} else {
			usedFilter = *filter;
		}

		// Release previously used resource
		if(this->m_texture != nullptr) {
			CResourceManager::GetInstance()->ReleaseResource(this->m_texture->GetName());
		}

		ITexture2DBase * tmp = CResourceManager::GetInstance()->GetResource<ITexture2DBase>(name);

		if(tmp == nullptr) {
			Load(image, usedFilter, format, flags, name);
		} else {
			this->m_texture = tmp;
		}
	}

	void CTexture2D::SaveToFile(const std::string & filename) const {
		LOG_TRACE();

		CMediaManager::GetInstance()->SaveMediaToFile<ITexture2DBase>(this->m_texture, this->m_texture->GetName());
	}

	void CTexture2D::Update(const CRectangleI & rect) {
		LOG_TRACE();

		if(rect.GetSize().x == -1) {
			this->m_texture->Update(CRectangleI(CVector2I(0, 0), this->m_texture->m_size));
		} else {
			this->m_texture->Update(rect);
		}
	}

	void CTexture2D::Load(const CImage & image, const STexture2DFilterPolicy & filter, EPixelFormat format, const unsigned long flags, const std::string name) {
		LOG_TRACE();

		// Check the compression compatibility
		if(IsFormatCompressed(format) && !IRenderer::GetRenderer()->HasCapability(CAPABILITY_HARDWARE_DXT_COMPRESSION)) {
			LOG_WARNING("Using a compressed format with a non compatible hardware. Switching to PIXEL_FORMAT_A8R8G8B8 format");
			format = PIXEL_FORMAT_A8R8G8B8;
		}

		// Check non power of two compatibility
		CVector2I size = CVector2I(GetNearestPowerOfTwo(image.GetSize().x), GetNearestPowerOfTwo(image.GetSize().y));
		if(size != image.GetSize() && !IRenderer::GetRenderer()->HasCapability(CAPABILITY_HARDWARE_NON_POW2_TEXTURES)) {
			LOG_WARNING("Using a non power of two texture with a non compatible hardware. Resizing to the nearest power of two");
		} else {
			size = image.GetSize();
		}

		this->m_texture = IRenderer::GetRenderer()->CreateTexture2D(size, filter, format, flags);

		if(name != "") {
			CResourceManager::GetInstance()->AddResource(name, this->m_texture);
		}

		if(IsFormatCompressed(format)) {
			this->m_texture->m_image = CImage(GetSize(), PIXEL_FORMAT_A8R8G8B8);
		}

		// Copy pixels
		this->m_texture->SetName(name);
		this->m_texture->m_image.Copy(image);

		// Do not update render target texture type
		if((flags & TEXTURE_FLAG_FRAMEBUFFER_TYPE) == 0) {
			Update();
		}
		
	}
}
