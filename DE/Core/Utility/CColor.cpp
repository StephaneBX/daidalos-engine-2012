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

#include "../../Core/Logger/ILogger.hpp"
#include "../../Core/Utility/CColor.hpp"

namespace daidalosengine {
	/**
	* Constructor
	*/
	CColor::CColor() {
		LOG_TRACE();
		SetInt(0, 0, 0, 0);
	}

	/**
	* Constructor
	* @param r Red
	* @param g Green
	* @param b Blue
	* @param a Alpha
	*/
	CColor::CColor(const int r, const int g, const int b, const int a) {
		LOG_TRACE();
		SetInt(r, g, b, a);
	}

	/**
	* Constructor
	* @param r Red
	* @param g Green
	* @param b Blue
	* @param a Alpha
	*/
	CColor::CColor(const float r, const float g, const float b, const float a) {
		LOG_TRACE();
		SetInt(static_cast<int>(r * 255.0F), static_cast<int>(g * 255.0F), static_cast<int>(b * 255.0F), static_cast<int>(a * 255.0F));
	}

	/**
	* Constructor
	* @param argb The color (ARGB)
	*/
	CColor::CColor(const unsigned long argb) {
		LOG_TRACE();
		this->m_color = argb;
	}

	/**
	* Get the alpha channel
	* @return The alpha channel
	*/
	const unsigned char CColor::GetAlpha() const {
		LOG_TRACE();
		return static_cast<unsigned char>((this->m_color & 0xFF000000) >> 24);
	}

	/**
	* Get the red channel
	* @return The red channel
	*/
	const unsigned char CColor::GetRed() const {
		LOG_TRACE();
		return static_cast<unsigned char>((this->m_color & 0x00FF0000) >> 16);
	}

	/**
	* Get the green channel
	* @return The green channel
	*/
	const unsigned char CColor::GetGreen() const {
		LOG_TRACE();
		return static_cast<unsigned char>((this->m_color & 0x0000FF00) >> 8);
	}

	/**
	* Get the blue channel
	* @return The blue channel
	*/
	const unsigned char CColor::GetBlue() const {
		LOG_TRACE();
		return static_cast<unsigned char>(this->m_color & 0x0000FF);
	}

	/**
	* Get an ARGB version of this color
	* @return The ARGB color
	*/
	const unsigned long CColor::ToARGB() const {
		LOG_TRACE();
		return this->m_color;
	}

	/**
	* Get an RGBA version of this color
	* @return The RGBA color
	*/
	const unsigned long CColor::ToRGBA() const {
		LOG_TRACE();
		return static_cast<unsigned long>((GetRed() << 24) | (GetGreen() << 16) | (GetBlue() << 8) | GetAlpha());
	}

	/**
	* Get an ABGR version of this color
	* @return The ABGR color
	*/
	const unsigned long CColor::ToABGR() const {
		LOG_TRACE();
		return static_cast<unsigned long>((GetAlpha() << 24) | (GetBlue() << 16) | (GetGreen() << 8) | GetRed());
	}

	/**
	* Get a string from a CVector3
	* @param separator The separator
	*/
	const std::string CColor::ToString(const std::string separator) const {
		LOG_TRACE();

		// Truncate
		CVector4F vector = ToRGBAVector();
		vector.x = TruncateDecimal(vector.x);
		vector.y = TruncateDecimal(vector.y);
		vector.z = TruncateDecimal(vector.z);
		vector.w = TruncateDecimal(vector.w);

		return vector.ToString();
	}

	/**
	* Get a RGB vector of this color
	* @return a RGB color vector
	*/
	const CVector3F CColor::ToRGBVector() const {
		LOG_TRACE();
		return CVector3F(GetRed() / 255.0F, GetGreen() / 255.0F, GetBlue() / 255.0F);
	}

	/**
	* Get a RGBA vector of this color
	* @return a RGBA color vector
	*/
	const CVector4F CColor::ToRGBAVector() const {
		LOG_TRACE();
		return CVector4F(GetRed() / 255.0F, GetGreen() / 255.0F, GetBlue() / 255.0F, GetAlpha() / 255.0F);
	}

	/**
	* + operator override
	* @param color A color
	* @return The color
	*/
	const CColor CColor::operator+(const CColor & color) const {
		LOG_TRACE();
		return CColor((int)GetRed() + (int)color.GetRed(), static_cast<int>(GetGreen() + color.GetGreen()), static_cast<int>(GetBlue() + color.GetBlue()), static_cast<int>(GetAlpha() + color.GetAlpha()));
	}

	/**
	* - operator override
	* @param color A color
	* @return The color
	*/
	const CColor CColor::operator-(const CColor & color) const {
		LOG_TRACE();
		return CColor(static_cast<int>(GetRed() - color.GetRed()), static_cast<int>(GetGreen() - color.GetGreen()), static_cast<int>(GetBlue() - color.GetBlue()), static_cast<int>(GetAlpha() - color.GetAlpha()));
	}

	/**
	* * operator override
	* @param color A color
	* @return The color
	*/
	const CColor CColor::operator*(const CColor & color) const {
		LOG_TRACE();
		return CColor(static_cast<int>(GetRed() * color.GetRed()), static_cast<int>(GetGreen() * color.GetGreen()), static_cast<int>(GetBlue() * color.GetBlue()), static_cast<int>(GetAlpha() * color.GetAlpha()));
	}

	/**
	* * operator override
	* @param number A number
	* @return The color
	*/
	const CColor CColor::operator*(const float number) const {
		LOG_TRACE();
		return CColor(static_cast<int>(GetRed() * number), static_cast<int>(GetGreen() * number), static_cast<int>(GetBlue() * number), static_cast<int>(GetAlpha() * number));
	}

	/**
	* / operator override
	* @param number A number
	* @return The color
	*/
	const CColor CColor::operator/(const float number) const {
		LOG_TRACE();
		return CColor(static_cast<int>(GetRed() / number), static_cast<int>(GetGreen() / number), static_cast<int>(GetBlue() / number), static_cast<int>(GetAlpha() / number));
	}

	/**
	* += operator override
	* @param color A color
	* @return The color
	*/
	const CColor & CColor::operator+=(const CColor & color) {
		LOG_TRACE();

		*this = *this + color;
		return *this;
	}

	/**
	* -= operator override
	* @param color A color
	* @return The color
	*/
	const CColor & CColor::operator-=(const CColor & color) {
		LOG_TRACE();

		*this = *this - color;
		return *this;
	}

	/**
	* *= operator override
	* @param color A color
	* @return The color
	*/
	const CColor & CColor::operator*=(const CColor & color) {
		LOG_TRACE();

		*this = *this * color;
		return *this;
	}

	/**
	* *= operator override
	* @param number A number
	* @return The color
	*/
	const CColor & CColor::operator*=(const float number) {
		LOG_TRACE();

		*this = *this * number;
		return *this;
	}

	/**
	* /= operator override
	* @param  number A number
	* @return The color
	*/
	const CColor & CColor::operator/=(const float number) {
		LOG_TRACE();

		*this = *this / number;
		return *this;
	}

	/**
	* == operator override
	* @param color A color
	* @return True if equal, false otherwise
	*/
	const bool CColor::operator==(const CColor & color) const {
		LOG_TRACE();
		return this->m_color == color.m_color;
	}

	/**
	* != operator override
	* @param color A color
	* @return True if equal, false otherwise
	*/
	const bool CColor::operator!=(const CColor & color) const {
		LOG_TRACE();
		return !(*this == color);
	}

	/**
	* Set the red channel with an integer
	* @param red The red channel value
	*/
	void CColor::SetRed(const int red) {
		LOG_TRACE();

		int r = ClampNumber(red, 0, 255);
		this->m_color |= r << 16;
	}

	/**
	* Set the green channel with an integer
	* @param green The green channel value
	*/
	void CColor::SetGreen(const int green) {
		LOG_TRACE();

		int g = ClampNumber(green, 0, 255);
		this->m_color |= g << 8;
	}

	/**
	* Set the blue channel with an integer
	* @param blue The blue channel value
	*/
	void CColor::SetBlue(const int blue) {
		LOG_TRACE();

		int b = ClampNumber(blue, 0, 255);
		this->m_color |= b;
	}

	/**
	* Set the alpha channel with an integer
	* @param alpha The alpha channel value
	*/
	void CColor::SetAlpha(const int alpha) {
		LOG_TRACE();

		int a = ClampNumber(alpha, 0, 255);
		this->m_color |= a << 24;
	}

	/**
	* Set the red channel with a float
	* @param red The red channel value
	*/
	void CColor::SetRed(const float red) {
		LOG_TRACE();

		int r = static_cast<int>(ClampNumber(red, 0.0F, 1.0F) * 255);
		this->m_color |= r << 16;
	}

	/**
	* Set the green channel with a float
	* @param green The green channel value
	*/
	void CColor::SetGreen(const float green) {
		LOG_TRACE();

		int g = static_cast<int>(ClampNumber(green, 0.0F, 1.0F) * 255);
		this->m_color |= g << 8;
	}

	/**
	* Set the blue channel with a float
	* @param blue The blue channel value
	*/
	void CColor::SetBlue(const float blue) {
		LOG_TRACE();

		int b = static_cast<int>(ClampNumber(blue, 0.0F, 1.0F) * 255);
		this->m_color |= b;
	}

	/**
	* Set the alpha channel with a float
	* @param alpha The alpha channel value
	*/
	void CColor::SetAlpha(const float alpha) {
		LOG_TRACE();

		int a = static_cast<int>(ClampNumber(alpha, 0.0F, 1.0F) * 255);
		this->m_color |= a << 24;
	}

	/**
	* Set the color with 4 floats
	* @param r Red
	* @param g Green
	* @param b Blue
	* @param a Alpha
	*/
	void CColor::SetFloat(const float r, const float g, const float b, const float a) {
		LOG_TRACE();

		this->m_color = 0;
		SetRed(r);
		SetGreen(g);
		SetBlue(b);
		SetAlpha(a);
	}

	/**
	* Set the color with 4 unsigned char
	* @param r Red
	* @param g Green
	* @param b Blue
	* @param a Alpha
	*/
	void CColor::SetUnsignedChar(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
		LOG_TRACE();
		this->m_color = (a << 24) | (r << 16) | (g << 8) | b;
	}

	/**
	* Set the color with 4 integers
	* @param r Red
	* @param g Green
	* @param b Blue
	* @param a Alpha
	*/
	void CColor::SetInt(const int r, const int g, const int b, const int a) {
		LOG_TRACE();

		unsigned char red = ClampNumber(r, 0, 255);
		unsigned char green = ClampNumber(g, 0, 255);
		unsigned char blue = ClampNumber(b, 0, 255);
		unsigned char alpha = ClampNumber(a, 0, 255);

		SetUnsignedChar(red, green, blue, alpha);
	}

	/**
	* >> operator override
	* @param stream The stream
	* @param color The color
	*/
	std::istream & operator>>(std::istream & stream, CColor & color) {
		LOG_TRACE();

		int r;
		int g;
		int b;
		int a;
		stream >> r >> g >> b >> a;
		color.SetInt(r, g, b, a);

		return stream;
	}

	/**
	* << operator override
	* @param stream The stream
	* @param color The color
	*/
	std::ostream & operator<<(std::ostream & stream, const CColor & color) {
		LOG_TRACE();

		return stream << static_cast<int>(color.GetRed()) << " "
			<< static_cast<int>(color.GetGreen()) << " "
			<< static_cast<int>(color.GetBlue()) << " "
			<< static_cast<int>(color.GetAlpha());
	}

	/**
	* Get a CColor from a string
	* @param text The text
	* @param defaultValue The default value
	* @param separator The separator
	* @return The vector
	*/
	const CColor CColor::GetFrom(const std::string text, const CColor & defaultValue, const std::string separator) {
		CColor result;
		CTokenizer token;

		token.AddLine(text);

		if(text.length() > 0 && token.Tokenize(separator, 0) && token.GetTokens().size() >= 3) {
			float r;
			float g;
			float b;
			float a;
			a = 1.0F;
			std::istringstream ss;

			ss.clear();
			ss.str(token.GetTokens()[0]);
			ss >> r;

			ss.clear();
			ss.str(token.GetTokens()[1]);
			ss >> g;

			ss.clear();
			ss.str(token.GetTokens()[2]);
			ss >> b;

			if(token.GetTokens().size() >= 4) {
				ss.clear();
				ss.str(token.GetTokens()[3]);
				ss >> a;
			}

			result = CColor(r, g, b, a);
		} else {
			result = defaultValue;
		}

		return result;
	}
}
