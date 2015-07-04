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

#ifndef __CORE_H
#define	__CORE_H

#include <string>
#include <time.h>
#include "../Core/Enums.h"

namespace DE {
	#define SAFE_DELETE(x){ if(x != nullptr){ delete x; x = nullptr; }}
	#define SAFE_DELETE_ARRAY(x){ if(x != nullptr){ delete[] x; x = nullptr; }}

	#ifdef WIN32
	#define SAFE_RELEASE(x){ if(x != nullptr){ x->Release(); x = nullptr; } }
	#endif

	#ifdef DE_DLL_EXPORTS
	#define DE_DLL_API __declspec(dllexport) 
	#else
	#define DE_DLL_API 
	#endif

	#define PI 3.14159265f
	#define PI_OVER_180 0.0174532925f
	#define EPSILON 0.001f
	#define APPLICATION_OPTIONS "Resources/Engine/engine.ini"
	#define APPLICATION_MATERIALS_CONFIGURATION "Resources/Engine/materials.ini"
	#define APPLICATION_NODES_CONFIGURATION "Resources/Engine/nodes.ini"

	inline const std::string GetCurrentTimeAsString() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%Y-%m-%d:%X", &tstruct);

		return buf;
	}

	/**
	* Transform a radian angle into a degree angle
	* @param angle The angle
	*/
	inline const float RadianToDegree(const float angle) {
		return angle * 180.0f / PI;
	}

	/**
	* Transform a degree angle into a radian angle
	* @param angle The angle
	*/
	inline const float DegreeToRadian(const float angle) {
		return angle * PI_OVER_180;
	}

	/**
	* Truncate the float to two decimals
	* @param value The float value
	*/
	inline const float TruncateDecimal(const float value) {
		return (std::ceilf(value * 100.0f) / 100.0f);
	}

	/**
	* Clamp a value
	* @param value The value to clamp
	* @param minValue The minimal value
	* @param maxValue The maximal value
	* @return The clamped value
	*/
	template<typename T> inline const T ClampNumber(const T value, const T minValue, const T maxValue) {
		return value >= minValue ? (value <= maxValue ? value : maxValue) : minValue;
	}

	/**
	* Trim a string (remove all ' ' before and after a text)
	* @param text The text
	* @return The modified text
	*/
	inline const std::string TrimString(const std::string & text) {
		std::string result = text;

		// Trim the beginning of the text
		std::string::size_type pos = text.find_last_not_of(' ');
		if(pos != std::string::npos) {
			result.erase(pos + 1);
		}

		// Trim the ending of the text
		pos = text.find_first_not_of(' ');
		if(pos != std::string::npos) {
			result.erase(0, pos);
		}

		return result;
	}

	/**
	* Get the size (in Bytes) of the given pixel format
	* @param format The pixel format
	* @return The size in Bytes
	*/
	inline const unsigned int GetBytesPerPixel(EPixelFormat format) {
		switch(format) {
		case PIXEL_FORMAT_L8: return 1;
		case PIXEL_FORMAT_A8L8:return 2;
		case PIXEL_FORMAT_A1R5G5B5: return 2;
		case PIXEL_FORMAT_A4R4G4B4: return 2;
		case PIXEL_FORMAT_R8G8B8: return 3;
		case PIXEL_FORMAT_A8R8G8B8: return 4;
		case PIXEL_FORMAT_R16F: return 2;
		case PIXEL_FORMAT_R32F: return 4;
		case PIXEL_FORMAT_DXT1: return 1;
		case PIXEL_FORMAT_DXT3: return 2;
		case PIXEL_FORMAT_DXT5: return 2;
		case PIXEL_FORMAT_DEPTH24S8: return 4;
		default: return 0;
		}
	}

	/**
	* Check if a pixel format is compressed
	* @param format The pixel format
	* @return True is the format is compressed, false otherwise
	*/
	inline const bool IsFormatCompressed(EPixelFormat format) {
		switch(format) {
		case PIXEL_FORMAT_DXT1:
		case PIXEL_FORMAT_DXT3:
		case PIXEL_FORMAT_DXT5:
			return true;

		default: return false;
		}
	}

	/**
	* Get the nearest power of two
	* @param x Value to test
	* @return The nearest power of two
	*/
	inline const int GetNearestPowerOfTwo(const int x) {
		int count = 0;
		int tmp = x;

		while(tmp > 1) {
			tmp >>= 1;
			++count;
		}

		int result = 1 << count;
		if(result < x) {
			result = 1 << (count + 1);
		}

		return result;
	}

	/**
	* Convertex a PIXEL_FORMAT_L8 pixel to another format
	* @param destinationFormat The destination format
	* @param source The source data
	* @param destination The destination data
	*/
	inline void ConvertPixel_L8(EPixelFormat destFormat, const unsigned char * src, unsigned char * dest) {
		switch(destFormat) {
		case PIXEL_FORMAT_L8:
			*dest = *src;
			break;

		case PIXEL_FORMAT_A8L8:
			dest[0] = *src;
			dest[1] = 0xFF;
			break;

		case PIXEL_FORMAT_A1R5G5B5:
			*reinterpret_cast<unsigned short*>(dest) = 0x8000 |
				((*src >> 3) << 10) |
				((*src >> 3) << 5) |
				((*src >> 3) << 0);
			break;

		case PIXEL_FORMAT_A4R4G4B4:
			dest[0] = (*src & 0xF0) | (*src >> 4);
			dest[1] = 0xF0 | (*src >> 4);
			break;

		case PIXEL_FORMAT_R8G8B8:
			dest[0] = *src;
			dest[1] = *src;
			dest[2] = *src;
			break;

		case PIXEL_FORMAT_A8R8G8B8:
			dest[0] = *src;
			dest[1] = *src;
			dest[2] = *src;
			dest[3] = 0xFF;
			break;

		case PIXEL_FORMAT_R16F:
			dest[0] = *src;
			dest[1] = 0x00;
			break;

		case PIXEL_FORMAT_R32F:
		case PIXEL_FORMAT_DEPTH24S8:
			dest[0] = *src;
			dest[1] = 0x00;
			dest[2] = 0x00;
			dest[3] = 0x00;
			break;
		}
	}

	/**
	* Convertex a PIXEL_FORMAT_A8L8 pixel to another format
	* @param destinationFormat The destination format
	* @param source The source data
	* @param destination The destination data
	*/
	inline void ConvertPixel_A8L8(EPixelFormat destFormat, const unsigned char * src, unsigned char * dest) {
		switch(destFormat) {
		case PIXEL_FORMAT_L8:
			*dest = src[0];
			break;

		case PIXEL_FORMAT_A8L8:
			dest[0] = src[0];
			dest[1] = src[1];
			break;

		case PIXEL_FORMAT_A1R5G5B5:
			*reinterpret_cast<unsigned short*>(dest) = ((src[1] >> 7) << 15) |
				((src[0] >> 3) << 10) |
				((src[0] >> 3) << 5) |
				((src[0] >> 3) << 0);
			break;

		case PIXEL_FORMAT_A4R4G4B4:
			dest[0] = (src[0] & 0xF0) | (src[0] >> 4);
			dest[1] = (src[1] & 0xF0) | (src[0] >> 4);
			break;

		case PIXEL_FORMAT_R8G8B8:
			dest[0] = src[0];
			dest[1] = src[0];
			dest[2] = src[0];
			break;

		case PIXEL_FORMAT_A8R8G8B8:
			dest[0] = src[0];
			dest[1] = src[0];
			dest[2] = src[0];
			dest[3] = src[1];
			break;

		case PIXEL_FORMAT_R16F:
			dest[0] = src[0];
			dest[1] = 0x00;
			break;

		case PIXEL_FORMAT_R32F:
		case PIXEL_FORMAT_DEPTH24S8:
			dest[0] = src[0];
			dest[1] = 0x00;
			dest[2] = 0x00;
			dest[3] = 0x00;
			break;
		}
	}

	/**
	* Convertex a PIXEL_FORMAT_A1RG5B5 pixel to another format
	* @param destinationFormat The destination format
	* @param source The source data
	* @param destination The destination data
	*/
	inline void ConvertPixel_A1R5G5B5(EPixelFormat destFormat, const unsigned char * src, unsigned char * dest) {
		unsigned short pix;
		switch(destFormat) {
		case PIXEL_FORMAT_L8:
			pix = *reinterpret_cast<const unsigned short*>(src);
			*dest = static_cast<unsigned char>(((pix & 0x7C00) >> 7) * 0.30 +
				((pix & 0x03E0) >> 2) * 0.59 +
				((pix & 0x001F) << 3) * 0.11);
			break;

		case PIXEL_FORMAT_A8L8:
			pix = *reinterpret_cast<const unsigned short*>(src);
			dest[0] = static_cast<unsigned char>(((pix & 0x7C00) >> 7) * 0.30 +
				((pix & 0x03E0) >> 2) * 0.59 +
				((pix & 0x001F) << 3) * 0.11);
			dest[1] = src[1] & 0x8000 ? 0xFF : 0x00;
			break;

		case PIXEL_FORMAT_A1R5G5B5:
			dest[0] = src[0];
			dest[1] = src[1];
			break;

		case PIXEL_FORMAT_A4R4G4B4:
			pix = *reinterpret_cast<const unsigned short*>(src);
			dest[1] = (pix & 0x8000 ? 0xF0 : 0x00) | ((pix & 0x7C00) >> 11);
			dest[0] = ((pix & 0x03C0) >> 2) | ((pix & 0x001F) >> 1);
			break;

		case PIXEL_FORMAT_R8G8B8:
			pix = *reinterpret_cast<const unsigned short*>(src);
			dest[2] = (pix & 0x7C00) >> 7;
			dest[1] = (pix & 0x03E0) >> 2;
			dest[0] = (pix & 0x001F) << 3;
			break;

		case PIXEL_FORMAT_A8R8G8B8:
			pix = *reinterpret_cast<const unsigned short*>(src);
			dest[3] = (pix & 0x8000) >> 8;
			dest[2] = (pix & 0x7C00) >> 7;
			dest[1] = (pix & 0x03E0) >> 2;
			dest[0] = (pix & 0x001F) << 3;
			break;

		case PIXEL_FORMAT_R16F:
			pix = *reinterpret_cast<const unsigned short*>(src);
			dest[0] = (pix & 0x7C00) >> 7;
			dest[1] = 0x00;
			break;

		case PIXEL_FORMAT_R32F:
		case PIXEL_FORMAT_DEPTH24S8:
			pix = *reinterpret_cast<const unsigned short*>(src);
			dest[0] = (pix & 0x7C00) >> 7;
			dest[1] = 0x00;
			dest[2] = 0x00;
			dest[3] = 0x00;
			break;
		}
	}

	/**
	* Convertex a PIXEL_FORMAT_A4R4G4B4 pixel to another format
	* @param destinationFormat The destination format
	* @param source The source data
	* @param destination The destination data
	*/
	inline void ConvertPixel_A4R4G4B4(EPixelFormat destFormat, const unsigned char * src, unsigned char * dest) {
		switch(destFormat) {
		case PIXEL_FORMAT_L8:
			*dest = static_cast<unsigned char>(((src[1] & 0x0F) << 4) * 0.30 +
				((src[0] & 0xF0) >> 0) * 0.59 +
				((src[0] & 0x0F) << 4) * 0.11);
			break;

		case PIXEL_FORMAT_A8L8:
			dest[0] = static_cast<unsigned char>(((src[1] & 0x0F) << 4) * 0.30 +
				((src[0] & 0xF0) >> 0) * 0.59 +
				((src[0] & 0x0F) << 4) * 0.11);
			dest[1] = src[1] & 0xF0;
			break;

		case PIXEL_FORMAT_A1R5G5B5:
			*reinterpret_cast<unsigned short*>(dest) = ((src[1] & 0x80) << 8) |
				((src[1] & 0x0F) << 11) |
				((src[0] & 0xF0) << 2) |
				((src[0] & 0x0F) << 1);
			break;

		case PIXEL_FORMAT_A4R4G4B4:
			dest[0] = src[0];
			dest[1] = src[1];
			break;

		case PIXEL_FORMAT_R8G8B8:
			dest[0] = (src[0] & 0x0F) << 4;
			dest[1] = (src[0] & 0xF0);
			dest[2] = (src[1] & 0x0F) << 4;
			break;

		case PIXEL_FORMAT_A8R8G8B8:
			dest[0] = (src[0] & 0x0F) << 4;
			dest[1] = (src[0] & 0xF0);
			dest[2] = (src[1] & 0x0F) << 4;
			dest[3] = (src[1] & 0xF0);
			break;

		case PIXEL_FORMAT_R16F:
			dest[0] = (src[1] & 0x0F) << 4;
			dest[1] = 0x00;
			break;

		case PIXEL_FORMAT_R32F:
		case PIXEL_FORMAT_DEPTH24S8:
			dest[0] = (src[1] & 0x0F) << 4;
			dest[1] = 0x00;
			dest[2] = 0x00;
			dest[3] = 0x00;
			break;
		}
	}

	/**
	* Convertex a PIXEL_FORMAT_R8G8B8 pixel to another format
	* @param destinationFormat The destination format
	* @param source The source data
	* @param destination The destination data
	*/
	inline void ConvertPixel_R8G8B8(EPixelFormat destFormat, const unsigned char * src, unsigned char * dest) {
		switch(destFormat) {
		case PIXEL_FORMAT_L8:
			*dest = static_cast<unsigned char>(src[2] * 0.30 + src[1] * 0.59 + src[0] * 0.11);
			break;

		case PIXEL_FORMAT_A8L8:
			dest[0] = static_cast<unsigned char>(src[2] * 0.30 + src[1] * 0.59 + src[0] * 0.11);
			dest[1] = 0xFF;
			break;

		case PIXEL_FORMAT_A1R5G5B5:
			*reinterpret_cast<unsigned short*>(dest) = 0x8000 |
				((src[2] >> 3) << 10) |
				((src[1] >> 3) << 5) |
				((src[0] >> 3) << 0);
			break;

		case PIXEL_FORMAT_A4R4G4B4:
			dest[0] = (src[1] & 0xF0) | (src[0] >> 4);
			dest[1] = 0xF0 | (src[2] >> 4);
			break;

		case PIXEL_FORMAT_R8G8B8:
			dest[0] = src[0];
			dest[1] = src[1];
			dest[2] = src[2];
			break;

		case PIXEL_FORMAT_A8R8G8B8:
			dest[0] = src[0];
			dest[1] = src[1];
			dest[2] = src[2];
			dest[3] = 0xFF;
			break;

		case PIXEL_FORMAT_R16F:
			dest[0] = src[2];
			dest[1] = 0x00;
			break;

		case PIXEL_FORMAT_R32F:
		case PIXEL_FORMAT_DEPTH24S8:
			dest[0] = src[2];
			dest[1] = 0x00;
			dest[2] = 0x00;
			dest[3] = 0x00;
			break;
		}
	}

	/**
	* Convertex a PIXEL_FORMAT_A8R8G8B8 pixel to another format
	* @param destinationFormat The destination format
	* @param source The source data
	* @param destination The destination data
	*/
	inline void ConvertPixel_A8R8G8B8(EPixelFormat destFormat, const unsigned char * src, unsigned char * dest) {
		switch(destFormat) {
		case PIXEL_FORMAT_L8:
			*dest = static_cast<unsigned char>(src[2] * 0.30 + src[1] * 0.59 + src[0] * 0.11);
			break;

		case PIXEL_FORMAT_A8L8:
			dest[0] = static_cast<unsigned char>(src[2] * 0.30 + src[1] * 0.59 + src[0] * 0.11);
			dest[1] = src[3];
			break;

		case PIXEL_FORMAT_A1R5G5B5:
			*reinterpret_cast<unsigned short*>(dest) = ((src[3] >> 7) << 15) |
				((src[2] >> 3) << 10) |
				((src[1] >> 3) << 5) |
				((src[0] >> 3) << 0);
			break;

		case PIXEL_FORMAT_A4R4G4B4:
			dest[0] = (src[1] & 0xF0) | (src[0] >> 4);
			dest[1] = (src[3] & 0xF0) | (src[2] >> 4);
			break;

		case PIXEL_FORMAT_R8G8B8:
			dest[0] = src[0];
			dest[1] = src[1];
			dest[2] = src[2];
			break;

		case PIXEL_FORMAT_A8R8G8B8:
			dest[0] = src[0];
			dest[1] = src[1];
			dest[2] = src[2];
			dest[3] = src[3];
			break;

		case PIXEL_FORMAT_R16F:
			dest[0] = src[2];
			dest[1] = 0x00;
			break;

		case PIXEL_FORMAT_R32F:
		case PIXEL_FORMAT_DEPTH24S8:
			dest[0] = src[2];
			dest[1] = 0x00;
			dest[2] = 0x00;
			dest[3] = 0x00;
			break;
		}
	}

	/**
	* Convert a pixel from a format to another
	* @param sourceFormat The source format
	* @param destinationFormat The destination format
	* @param source The source data
	* @param destination The destination data
	*/
	inline void ConvertPixel(EPixelFormat srcFormat, EPixelFormat destFormat, const unsigned char * src, unsigned char * dest) {
		switch(srcFormat) {
		case PIXEL_FORMAT_L8:
			ConvertPixel_L8(destFormat, src, dest);
			break;

		case PIXEL_FORMAT_A8L8:
			ConvertPixel_A8L8(destFormat, src, dest);
			break;

		case PIXEL_FORMAT_A1R5G5B5:
			ConvertPixel_A1R5G5B5(destFormat, src, dest);
			break;

		case PIXEL_FORMAT_A4R4G4B4:
			ConvertPixel_A4R4G4B4(destFormat, src, dest);
			break;

		case PIXEL_FORMAT_R8G8B8:
			ConvertPixel_R8G8B8(destFormat, src, dest);
			break;

		case PIXEL_FORMAT_A8R8G8B8:
			ConvertPixel_A8R8G8B8(destFormat, src, dest);
			break;
		}
	}
}

#endif
