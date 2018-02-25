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

#include <algorithm>
#include "../../Core/Core.hpp"
#include "../../Core/Logger/ILogger.hpp"
#include "../../Renderers/DX9/CDX9Enums.hpp"
#include "../../Renderers/DX9/CDX9Exception.hpp"
#include "../../Renderers/DX9/DX9Global.hpp"
#include "../../Renderers/DX9/CDX9Texture2D.hpp"

namespace daidalosengine {
	CDX9Texture2D::CDX9Texture2D(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const bool mipmap, const bool autoMipmap, IDirect3DTexture9 * texture) : ITexture2DBase(size, filter, format, mipmap, autoMipmap) {
		LOG_TRACE();

		this->m_texture = texture;
	}

	CDX9Texture2D::~CDX9Texture2D() {
		LOG_TRACE();

		SAFE_RELEASE(this->m_texture);
	}

	IDirect3DTexture9 * CDX9Texture2D::GetTexture() const {
		LOG_TRACE();

		return this->m_texture;
	}

	void CDX9Texture2D::Update(const CRectangleI & rect) {
		LOG_TRACE();

		if(this->m_format == this->m_image.GetFormat()) {
			D3DLOCKED_RECT lockedRect;
			RECT Lock = { rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetBottom() };

			this->m_texture->LockRect(0, &lockedRect, &Lock, 0);
			UpdateSurface(lockedRect, rect);
			this->m_texture->UnlockRect(0);
		} else {
			IDirect3DDevice9 * device = nullptr;
			this->m_texture->GetDevice(&device);

			IDirect3DSurface9 * src = nullptr;
			if(FAILED(device->CreateOffscreenPlainSurface(rect.GetWidth(), rect.GetHeight(), CDX9Enums::Get(this->m_image.GetFormat()), D3DPOOL_SYSTEMMEM, &src, nullptr))) {
				LOG_ERROR("Unable to create an offscreen plain surface");
				ILogger::Kill();

				throw CDX9Exception("CreateOffscreenPlainSurface", "CDX9Texture::Update");
			}

			D3DLOCKED_RECT LockedRect;
			src->LockRect(&LockedRect, nullptr, 0);
			UpdateSurface(LockedRect, rect);
			src->UnlockRect();

			IDirect3DSurface9 * dest = nullptr;
			this->m_texture->GetSurfaceLevel(0, &dest);

			RECT destRect = { rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetBottom() };
			if(FAILED(D3DXLoadSurfaceFromSurface(dest, nullptr, &destRect, src, nullptr, nullptr, D3DX_DEFAULT, 0))) {
				LOG_ERROR("Unable to load an offscreen plain surface");
				ILogger::Kill();

				throw CDX9Exception("D3DXLoadSurfaceFromSurface", "CDX9Texture::Update");
			}
		}

		if(this->m_mipmaps) {
			if(this->m_autoMipmap)
				this->m_texture->GenerateMipSubLevels();
			else
				D3DXFilterTexture(this->m_texture, nullptr, D3DX_DEFAULT, D3DX_DEFAULT);
		}
	}

	void CDX9Texture2D::UpdateSurface(const D3DLOCKED_RECT & lockedRect, const CRectangleI & rect) {
		LOG_TRACE();

		unsigned char * destPix = reinterpret_cast<unsigned char*>(lockedRect.pBits);
		const unsigned char * srcPix = this->m_image.GetData() + (rect.GetLeft() + rect.GetTop() * this->m_size.x) * GetBytesPerPixel(this->m_image.GetFormat());
		const unsigned int bpp = GetBytesPerPixel(this->m_image.GetFormat());

		for(int i = 0; i < rect.GetHeight(); ++i) {
			std::copy(srcPix, srcPix + rect.GetWidth() * bpp, destPix);
			srcPix += this->m_size.x * bpp;
			destPix += lockedRect.Pitch;
		}
	}
}
