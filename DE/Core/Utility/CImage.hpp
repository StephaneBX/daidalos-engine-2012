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

#ifndef __CIMAGE_HPP
#define __CIMAGE_HPP

#include <vector>
#include "../../Core/Utility/CVector2.hpp"
#include "../../Core/Utility/CColor.hpp"
#include "../../Core/Utility/CRectangle.hpp"
#include "../../Core/Enums.hpp"

namespace daidalosengine {
	/**
	* CImage class
	*/
	class CImage {
	private:
		CVector2I m_size;
		EPixelFormat m_format;
		std::vector<unsigned char> m_data;

	public:
		/**
		* Constructor
		* @param size The image size
		* @param format The image format
		*/
		CImage(const CVector2I & size = CVector2I(1, 1), EPixelFormat format = PIXEL_FORMAT_A8R8G8B8);

		/**
		* Constructor
		* @param size The image size
		* @param format The image format
		* @param data The data
		*/
		CImage(const CVector2I & size, EPixelFormat format, const unsigned char * data);

		/**
		* Get the image width
		* @return The image width
		*/
		const int GetWidth() const;

		/**
		* Get the image height
		* @return The image height
		*/
		const int GetHeight() const;

		/**
		* Get the image data
		* @return The image data
		*/
		const unsigned char * GetData() const;

		/**
		* Get a pixel and return it in pixel
		* @param x The x coordinate
		* @param y The y coordinate
		* @return The color
		*/
		const CColor GetPixel(const int x, const int y) const;

		/**
		* Get the image size
		* @return The image size
		*/
		const CVector2I & GetSize() const;

		/**
		* Get the image format
		* @return The image format
		*/
		EPixelFormat GetFormat() const;

		/**
		* Get a sub image.
		* @param rect
		* @return The image
		*/
		CImage SubImage(const CRectangleI & rect) const;

		/**
		* Get a pixel and return it in pixel
		* @param x The x coordinate
		* @param y The y coordinate
		* @param pixel The color
		*/
		void GetPixel(const int x, const int y, unsigned char * pixel) const;

		/**
		* Set a pixel
		* @param x The x coordinate
		* @param y Th y coordinate
		* @param color The color
		*/
		void SetPixel(const int x, const int y, const CColor & color);

		/**
		* Set a pixel
		* @param x The x coordinate
		* @param y Th y coordinate
		* @param pixel The pixel
		*/
		void SetPixel(const int x, const int y, const unsigned char * pixel);

		/**
		* Fill the image with a color
		* @param color The color
		*/
		void Fill(const CColor & color);

		/**
		* Copy an image
		* @param image The image to copy
		*/
		void Copy(const CImage & image);

		/**
		* Flip the image vertically
		*/
		void Flip();

		/**
		* Flip the image horizontally
		*/
		void Mirror();
	};
}

#endif
