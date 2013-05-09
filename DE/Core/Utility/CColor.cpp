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

#include "../../Core/Utility/CColor.h"

namespace DE
{
	CColor::CColor()
	{
		SetInt(0,0,0,0);
	}

	CColor::CColor(const int r, const int g, const int b, const int a)
	{
		SetInt(r, g, b, a);
	}

	CColor::CColor(const float r, const float g, const float b, const float a)
	{
		SetInt(static_cast<int>(r * 255.0f), static_cast<int>(g * 255.0f), static_cast<int>(b * 255.0f), static_cast<int>(a * 255.0f));
	}

	CColor::CColor(const unsigned long argb)
	{
		this->m_color = argb;
	}

	const unsigned char CColor::GetAlpha() const
	{
		return static_cast<unsigned char>((this->m_color & 0xFF000000) >> 24);
	}

	const unsigned char CColor::GetRed() const
	{
		return static_cast<unsigned char>((this->m_color & 0x00FF0000) >> 16);
	}

	const unsigned char CColor::GetGreen() const
	{
		return static_cast<unsigned char>((this->m_color & 0x0000FF00) >> 8);
	}

	const unsigned char CColor::GetBlue() const
	{
		return static_cast<unsigned char>(this->m_color & 0x0000FF);
	}

	const unsigned long CColor::ToARGB() const
	{
		return this->m_color;
	}

	const unsigned long CColor::ToRGBA() const
	{
		return static_cast<unsigned long>((GetRed() << 24) | (GetGreen() << 16) | (GetBlue() << 8) | GetAlpha());
	}

	const unsigned long CColor::ToABGR() const
	{
		return static_cast<unsigned long>((GetAlpha() << 24) | (GetBlue() << 16) | (GetGreen() << 8) | GetRed());
	}

	const CVector3F CColor::ToRGBVector() const
	{
		return CVector3F(GetRed() / 255.0f, GetGreen() / 255.0f, GetBlue() / 255.0f);
	}

	const CVector4F CColor::ToRGBAVector() const
	{
		return CVector4F(GetRed() / 255.0f, GetGreen() / 255.0f, GetBlue() / 255.0f, GetAlpha() / 255.0f);
	}

	const CColor CColor::operator+(const CColor & color) const
	{
		return CColor((int)GetRed() + (int)color.GetRed(), static_cast<int>(GetGreen() + color.GetGreen()), static_cast<int>(GetBlue() + color.GetBlue()), static_cast<int>(GetAlpha() + color.GetAlpha()));
	}

	const CColor CColor::operator-(const CColor & color) const
	{
		return CColor(static_cast<int>(GetRed() - color.GetRed()), static_cast<int>(GetGreen() - color.GetGreen()), static_cast<int>(GetBlue() - color.GetBlue()), static_cast<int>(GetAlpha() - color.GetAlpha()));
	}

	const CColor CColor::operator*(const CColor & color) const
	{
		return CColor(static_cast<int>(GetRed() * color.GetRed()), static_cast<int>(GetGreen() * color.GetGreen()), static_cast<int>(GetBlue() * color.GetBlue()), static_cast<int>(GetAlpha() * color.GetAlpha()));
	}

	const CColor CColor::operator*(const float number) const
	{
		return CColor(static_cast<int>(GetRed() * number), static_cast<int>(GetGreen() * number), static_cast<int>(GetBlue() * number), static_cast<int>(GetAlpha() * number));
	}

	const CColor CColor::operator/(const float number) const
	{
		return CColor(static_cast<int>(GetRed() / number), static_cast<int>(GetGreen() / number), static_cast<int>(GetBlue() / number), static_cast<int>(GetAlpha() / number));
	}

	const CColor & CColor::operator+=(const CColor & color)
	{
		*this = *this + color;
		return *this;
	}

	const CColor & CColor::operator-=(const CColor & color)
	{
		*this = *this - color;
		return *this;
	}

	const CColor & CColor::operator*=(const CColor & color)
	{
		*this = *this * color;
		return *this;
	}

	const CColor & CColor::operator*=(const float number)
	{
		*this = *this * number;
		return *this;
	}

	const CColor & CColor::operator/=(const float number)
	{
		*this = *this / number;
		return *this;
	}

	const bool CColor::operator==(const CColor & color) const
	{
		return this->m_color == color.m_color;
	}

	const bool CColor::operator!=(const CColor & color) const
	{
		return !(*this == color);
	}

	void CColor::SetRed(const int red)
	{
		int r = (red >= 0) ? (red <= 255 ? red : 255) : 0;
		this->m_color |= r << 16;
	}

	void CColor::SetGreen(const int green)
	{
		int g = (green >= 0) ? (green <= 255 ? green : 255) : 0;
		this->m_color |= green << 8;
	}

	void CColor::SetBlue(const int blue)
	{
		int b = (blue >= 0) ? (blue <= 255 ? blue : 255) : 0;
		this->m_color |= blue;
	}

	void CColor::SetAlpha(const int alpha)
	{
		int a = (alpha >= 0) ? (alpha <= 255 ? alpha : 255) : 0;
		this->m_color |= alpha << 24;
	}

	void CColor::SetRed(const float red)
	{
		int r = static_cast<int>((red >= 0) ? (red <= 1.0f ? (red * 255) : 255) : 0);
		this->m_color |= r << 16;
	}

	void CColor::SetGreen(const float green)
	{
		int g = static_cast<int>((green >= 0) ? (green <= 1.0f ? (green * 255) : 255) : 0);
		this->m_color |= g << 8;
	}

	void CColor::SetBlue(const float blue)
	{
		int b = static_cast<int>((blue >= 0) ? (blue <= 1.0f ? (blue * 255) : 255) : 0);
		this->m_color |= b;
	}

	void CColor::SetAlpha(const float alpha)
	{
		int a = static_cast<int>((alpha >= 0) ? (alpha <= 1.0f ? (alpha * 255) : 255) : 0);
		this->m_color |= a << 24;
	}

	void CColor::SetFloat(const float r, const float g, const float b, const float a)
	{
		this->m_color = 0;
		SetRed(r);
		SetGreen(g);
		SetBlue(b);
		SetAlpha(a);
	}

	void CColor::SetUnsignedChar(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
	{
		this->m_color = (a << 24) | (r << 16) | (g << 8) | b;
	}

	void CColor::SetInt(const int r, const int g, const int b, const int a)
	{
		unsigned char red = (r >= 0) ? (r <= 255 ? r : 255) : 0;
		unsigned char green = (g >= 0) ? (g <= 255 ? g : 255) : 0;
		unsigned char blue = (b >= 0) ? (b <= 255 ? b : 255) : 0;
		unsigned char alpha = (a >= 0) ? (a <= 255 ? a : 255) : 0;

		SetUnsignedChar(red, green, blue, alpha);
	}

	std::istream & operator>>(std::istream & stream, CColor & color)
	{
		int r, g, b, a;
		stream >> r >> g >> b >> a;
		color.SetInt(r, g, b, a);

		return stream;
	}

    std::ostream & operator<<(std::ostream & stream, const CColor & color)
	{
		return stream	<< static_cast<int>(color.GetRed())   << " "
						<< static_cast<int>(color.GetGreen()) << " "
						<< static_cast<int>(color.GetBlue())  << " "
						<< static_cast<int>(color.GetAlpha());
	}
}
