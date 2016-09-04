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

#ifndef __CCOLOR_H
#define __CCOLOR_H

#include "../../Core/Utility/CVector3.h"
#include "../../Core/Utility/CVector4.h"
#include <iostream>

namespace DE {
	/**
	* CColor class
	* Handle colors
	*/
	class CColor {
	private:
		unsigned long m_color; ///< ARGB color

	public:
		/**
		* Constructor
		*/
		CColor();

		/**
		* Constructor
		* @param r Red
		* @param g Green
		* @param b Blue
		* @param a Alpha
		*/
		CColor(const int r, const int g, const int b, const int a = 255);

		/**
		* Constructor
		* @param r Red
		* @param g Green
		* @param b Blue
		* @param a Alpha
		*/
		CColor(const float r, const float g, const float b, const float a = 1.0f);

		/**
		* Constructor
		* @param argb The color (ARGB)
		*/
		CColor(const unsigned long argb);

		/**
		* Get the red channel
		* @return The red channel
		*/
		const unsigned char GetRed() const;

		/**
		* Get the green channel
		* @return The green channel
		*/
		const unsigned char GetGreen() const;

		/**
		* Get the blue channel
		* @return The blue channel
		*/
		const unsigned char GetBlue() const;

		/**
		* Get the alpha channel
		* @return The alpha channel
		*/
		const unsigned char GetAlpha() const;

		/**
		* Get an ARGB version of this color
		* @return The ARGB color
		*/
		const unsigned long ToARGB() const;

		/**
		* Get an RGBA version of this color
		* @return The RGBA color
		*/
		const unsigned long ToRGBA() const;

		/**
		* Get an ABGR version of this color
		* @return The ABGR color
		*/
		const unsigned long ToABGR() const;

		/**
		* Get a string from a CVector3
		* @param separator The separator
		*/
		const std::string ToString(const std::string separator = ":") const;

		/**
		* Get a RGB vector of this color
		* @return a RGB color vector
		*/
		const CVector3F ToRGBVector() const;

		/**
		* Get a RGBA vector of this color
		* @return a RGBA color vector
		*/
		const CVector4F ToRGBAVector() const;

		/**
		* + operator override
		* @param color A color
		* @return The color
		*/
		const CColor operator+(const CColor & color) const;

		/**
		* - operator override
		* @param color A color
		* @return The color
		*/
		const CColor operator-(const CColor & color) const;

		/**
		* * operator override
		* @param color A color
		* @return The color
		*/
		const CColor operator*(const CColor & color) const;

		/**
		* * operator override
		* @param number A number
		* @return The color
		*/
		const CColor operator*(const float number) const;

		/**
		* / operator override
		* @param number A number
		* @return The color
		*/
		const CColor operator/(const float number) const;

		/**
		* += operator override
		* @param color A color
		* @return The color
		*/
		const CColor & operator+=(const CColor & color);

		/**
		* -= operator override
		* @param color A color
		* @return The color
		*/
		const CColor & operator-=(const CColor & color);

		/**
		* *= operator override
		* @param color A color
		* @return The color
		*/
		const CColor & operator*=(const CColor & color);

		/**
		* *= operator override
		* @param number A number
		* @return The color
		*/
		const CColor & operator*=(const float number);

		/**
		* /= operator override
		* @param  number A number
		* @return The color
		*/
		const CColor & operator/=(const float number);

		/**
		* == operator override
		* @param color A color
		* @return True if equal, false otherwise
		*/
		const bool operator==(const CColor & color) const;

		/**
		* != operator override
		* @param color A color
		* @return True if equal, false otherwise
		*/
		const bool operator!=(const CColor & color) const;

		/**
		* >> operator override
		* @param stream The stream
		* @param color The color
		*/
		friend std::istream & operator>>(std::istream & stream, CColor & color);

		/**
		* << operator override
		* @param stream The stream
		* @param color The color
		*/
		friend std::ostream & operator<<(std::ostream & stream, const CColor & color);

		/**
		* Set the red channel with an integer
		* @param red The red channel value
		*/
		void SetRed(const int red);

		/**
		* Set the green channel with an integer
		* @param green The green channel value
		*/
		void SetGreen(const int green);

		/**
		* Set the blue channel with an integer
		* @param blue The blue channel value
		*/
		void SetBlue(const int blue);

		/**
		* Set the alpha channel with an integer
		* @param alpha The alpha channel value
		*/
		void SetAlpha(const int alpha);

		/**
		* Set the red channel with a float
		* @param red The red channel value
		*/
		void SetRed(const float red);

		/**
		* Set the green channel with a float
		* @param green The green channel value
		*/
		void SetGreen(const float green);

		/**
		* Set the blue channel with a float
		* @param blue The blue channel value
		*/
		void SetBlue(const float blue);

		/**
		* Set the alpha channel with a float
		* @param alpha The alpha channel value
		*/
		void SetAlpha(const float alpha);

		/**
		* Set the color with 4 floats
		* @param r Red
		* @param g Green
		* @param b Blue
		* @param a Alpha
		*/
		void SetFloat(const float r, const float g, const float b, const float a);

		/**
		* Set the color with 4 unsigned char
		* @param r Red
		* @param g Green
		* @param b Blue
		* @param a Alpha
		*/
		void SetUnsignedChar(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);

		/**
		* Set the color with 4 integers
		* @param r Red
		* @param g Green
		* @param b Blue
		* @param a Alpha
		*/
		void SetInt(const int r, const int g, const int b, const int a);
	};
}

#endif
