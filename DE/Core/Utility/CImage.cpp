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
#include "../../Core/Core.h"
#include "../../Core/Utility/CVector2.h"
#include "../../Core/Utility/CImage.h"

namespace DE {
	/**
	* Constructor
	* @param size The image size
	* @param format The image format
	*/
	CImage::CImage(const CVector2I & size, EPixelFormat format) {
		this->m_size = size;
		this->m_format = format;
		this->m_data = std::vector<unsigned char>(this->m_size.x * this->m_size.y * GetBytesPerPixel(format), 0x00);
	}

	/**
	* Constructor
	* @param size The image size
	* @param format The image format
	* @param data The data
	*/
	CImage::CImage(const CVector2I & size, EPixelFormat format, const unsigned char * data) {
		this->m_size = size;
		this->m_format = format;
		this->m_data = std::vector<unsigned char>(data, data + this->m_size.x * this->m_size.y * GetBytesPerPixel(format));
	}

	/**
	* Get the image width
	* @return The image width
	*/
	const int CImage::GetWidth() const {
		return this->m_size.x;
	}

	/**
	* Get the image height
	* @return The image height
	*/
	const int CImage::GetHeight() const {
		return this->m_size.y;
	}

	/**
	* Get the image data
	* @return The image data
	*/
	const unsigned char * CImage::GetData() const {
		return &this->m_data[0];
	}

	/**
	* Get a pixel and return it in pixel
	* @param x The x coordinate
	* @param y The y coordinate
	* @return The color
	*/
	const CColor CImage::GetPixel(const int x, const int y) const {
		unsigned char tmp[4];
		const unsigned char * pix = &this->m_data[(y * this->m_size.x + x) * GetBytesPerPixel(this->m_format)];
		ConvertPixel(this->m_format, PIXEL_FORMAT_A8R8G8B8, pix, &tmp[0]);

		return CColor(tmp[2], tmp[1], tmp[0], tmp[3]);
	}

	/**
	* Get the image size
	* @return The image size
	*/
	const CVector2I & CImage::GetSize() const {
		return this->m_size;
	}

	/**
	* Get the image format
	* @return The image format
	*/
	EPixelFormat CImage::GetFormat() const {
		return this->m_format;
	}

	/**
	* Get a sub image.
	* @param rect
	* @return The image
	*/
	CImage CImage::SubImage(const CRectangleI & rect) const {
		// The sub image
		CImage sub(CVector2I(rect.GetWidth(), rect.GetHeight()), this->m_format);

		// Data calculation
		const unsigned char * src = &this->m_data[rect.GetLeft() + rect.GetTop() * GetBytesPerPixel(this->m_format)];
		unsigned char * dest = &sub.m_data[0];
		const unsigned int srcPitch = this->m_size.x * GetBytesPerPixel(this->m_format);
		const unsigned int destPitch = sub.m_size.x * GetBytesPerPixel(this->m_format);

		// Copy the data
		for(int i = rect.GetTop(); i < rect.GetBottom(); ++i) {
			std::copy(src, src + srcPitch, dest);
			src += srcPitch;
			dest += destPitch;
		}

		return sub;
	}

	/**
	* Get a pixel and return it in pixel
	* @param x The x coordinate
	* @param y The y coordinate
	* @param pixel The color
	*/
	void CImage::GetPixel(const int x, const int y, unsigned char * pixel) const {
		memcpy(pixel, GetData() + y * this->m_size.x + x, GetBytesPerPixel(this->m_format));
	}

	/**
	* Set a pixel
	* @param x The x coordinate
	* @param y Th y coordinate
	* @param color The color
	*/
	void CImage::SetPixel(const int x, const int y, const CColor & color) {
		unsigned char tmp[4] = { color.GetBlue(), color.GetGreen(), color.GetRed(), color.GetAlpha() };
		unsigned char * pix = &this->m_data[(y * this->m_size.x + x) * GetBytesPerPixel(this->m_format)];

		ConvertPixel(PIXEL_FORMAT_A8R8G8B8, this->m_format, &tmp[0], pix);
	}

	/**
	* Set a pixel
	* @param x The x coordinate
	* @param y Th y coordinate
	* @param pixel The pixel
	*/
	void CImage::SetPixel(const int x, const int y, const unsigned char * pixel) {
		memcpy(&this->m_data[0] + y * this->m_size.x + x, pixel, GetBytesPerPixel(this->m_format));
	}

	/**
	* Fill the image with a color
	* @param color The color
	*/
	void CImage::Fill(const CColor & color) {
		SetPixel(0, 0, color);

		const unsigned int bpp = GetBytesPerPixel(this->m_format);
		auto begin = this->m_data.begin();

		for(auto it = begin + bpp; it != this->m_data.end(); it += bpp)
			std::copy(begin, begin + bpp, it);
	}

	/**
	* Copy an image
	* @param image The image to copy
	*/
	void CImage::Copy(const CImage & image) {
		// Compare the sizes
		if(this->m_size.x == image.m_size.x && this->m_size.y == image.m_size.y) {
			// Compare the format
			if(this->m_format == image.m_format) {
				this->m_data = image.m_data;
			} else {
				const unsigned char * src = &image.m_data[0];
				unsigned char * dest = &this->m_data[0];
				const unsigned int srcBpp = GetBytesPerPixel(image.m_format);
				const unsigned int destBpp = GetBytesPerPixel(this->m_format);

				for(int i = 0; i < this->m_size.x * this->m_size.y; ++i) {
					ConvertPixel(image.m_format, this->m_format, src, dest);
					src += srcBpp;
					dest += destBpp;
				}
			}
		} else {
			const CVector2F pitch(image.m_size.x / static_cast<float>(this->m_size.x), image.m_size.y / static_cast<float>(this->m_size.y));

			// Compare the format
			if(this->m_format == image.m_format) {
				unsigned char pix[4];
				for(int i = 0; i < this->m_size.y; ++i) {
					for(int j = 0; j < this->m_size.x; ++j) {
						image.GetPixel(static_cast<int>(j * pitch.x), static_cast<int>(i * pitch.y), &pix[0]);
						SetPixel(j, i, &pix[0]);
					}
				}
			} else {
				unsigned char srcPix[4];
				unsigned char destPix[4];
				for(int i = 0; i < this->m_size.y; ++i) {
					for(int j = 0; j < this->m_size.x; ++j) {
						image.GetPixel(static_cast<int>(j * pitch.x), static_cast<int>(i * pitch.y), &srcPix[0]);
						ConvertPixel(image.m_format, this->m_format, &srcPix[0], &destPix[0]);
						SetPixel(j, i, &destPix[0]);
					}
				}
			}
		}
	}

	/**
	* Flip the image vertically
	*/
	void CImage::Flip() {
		int bpp = GetBytesPerPixel(this->m_format);

		for(int i = 0; i < this->m_size.y / 2; ++i) {
			std::swap_ranges(&this->m_data[i * this->m_size.x * bpp],
				&this->m_data[(i + 1) * this->m_size.x * bpp - 1],
				&this->m_data[(this->m_size.y - i - 1) * this->m_size.x * bpp]);
		}
	}

	/**
	* Flip the image horizontally
	*/
	void CImage::Mirror() {
		int bpp = GetBytesPerPixel(this->m_format);

		for(int i = 0; i < this->m_size.x / 2; ++i) {
			for(int j = 0; j < this->m_size.y; ++j) {
				std::swap_ranges(&this->m_data[(i + j * this->m_size.x) * bpp],
					&this->m_data[(i + j * this->m_size.x + 1) * bpp],
					&this->m_data[(this->m_size.x - i - 1 + j * this->m_size.x) * bpp]);
			}
		}
	}
}
