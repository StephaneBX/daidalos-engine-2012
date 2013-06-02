#include "ColorTest.h"
#include <Core/Utility/CColor.h>

CPPUNIT_TEST_SUITE_REGISTRATION(ColorTest);

void ColorTest::TestConstructors()
{
	// Default constructor
	DE::CColor color;
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetBlue());

	// Float constructor
	color = DE::CColor(1.0f, 0.0f, 0.5f, 0.2f);
	CPPUNIT_ASSERT_EQUAL(51, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(255, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(127, (int)color.GetBlue());

	// Unsigned char constructor
	color = DE::CColor(0x33, 0x00, 0xFF, 0xAB);
	CPPUNIT_ASSERT_EQUAL(0xAB, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x33, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0xFF, (int)color.GetBlue());

	// Unsigned long
	color = DE::CColor(0x11223344);
	CPPUNIT_ASSERT_EQUAL(0x11, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x22, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x33, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0x44, (int)color.GetBlue());
}

void ColorTest::TestOperators()
{
	DE::CColor color1 = DE::CColor(1.0f, 0.0f, 0.5f, 0.2f);
	DE::CColor color2 = DE::CColor(0.22f, 0.35f, 0.6f, 0.4f);
	DE::CColor color3;
	float number = 0.33f;

	// Addition operator between two colors
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetRed() + color2.GetRed(), 0, 255), (int)(color1 + color2).GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetGreen() + color2.GetGreen(), 0, 255), (int)(color1 + color2).GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetBlue() + color2.GetBlue(), 0, 255), (int)(color1 + color2).GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetAlpha() + color2.GetAlpha(), 0, 255), (int)(color1 + color2).GetAlpha());

	// Addition (+=) operator between two colors
	color3 = color1;
	color3 += color2;
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetRed() + color2.GetRed(), 0, 255), (int)color3.GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetGreen() + color2.GetGreen(), 0, 255), (int)color3.GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetBlue() + color2.GetBlue(), 0, 255), (int)color3.GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetAlpha() + color2.GetAlpha(), 0, 255), (int)color3.GetAlpha());

	// Substraction operator between two colors
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetRed() - color2.GetRed(), 0, 255), (int)(color1 - color2).GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetGreen() - color2.GetGreen(), 0, 255), (int)(color1 - color2).GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetBlue() - color2.GetBlue(), 0, 255), (int)(color1 - color2).GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetAlpha() - color2.GetAlpha(), 0, 255), (int)(color1 - color2).GetAlpha());

	// Substraction (-=) operator between two colors
	color3 = color1;
	color3 -= color2;
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetRed() - color2.GetRed(), 0, 255), (int)color3.GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetGreen() - color2.GetGreen(), 0, 255), (int)color3.GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetBlue() - color2.GetBlue(), 0, 255), (int)color3.GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetAlpha() - color2.GetAlpha(), 0, 255), (int)color3.GetAlpha());

	// Multiplication operator between two colors
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetRed() * color2.GetRed(), 0, 255), (int)(color1 * color2).GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetGreen() * color2.GetGreen(), 0, 255), (int)(color1 * color2).GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetBlue() * color2.GetBlue(), 0, 255), (int)(color1 * color2).GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetAlpha() * color2.GetAlpha(), 0, 255), (int)(color1 * color2).GetAlpha());

	// Multiplication (*=) operator between two colors
	color3 = color1;
	color3 *= color2;
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetRed() * color2.GetRed(), 0, 255), (int)color3.GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetGreen() * color2.GetGreen(), 0, 255), (int)color3.GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetBlue() * color2.GetBlue(), 0, 255), (int)color3.GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>(color1.GetAlpha() * color2.GetAlpha(), 0, 255), (int)color3.GetAlpha());

	// Multiplication operator between a color and a number
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetRed() * number), 0, 255), (int)(color1 * number).GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetGreen() * number), 0, 255), (int)(color1 * number).GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetBlue() * number), 0, 255), (int)(color1 * number).GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetAlpha() * number), 0, 255), (int)(color1 * number).GetAlpha());

	// Multiplication (*=) operator between a color and a number
	color3 = color1;
	color3 *= number;
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetRed() * number), 0, 255), (int)color3.GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetGreen() * number), 0, 255), (int)color3.GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetBlue() * number), 0, 255), (int)color3.GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetAlpha() * number), 0, 255), (int)color3.GetAlpha());

	// Division operator between a color and a number
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetRed() * number), 0, 255), (int)(color1 * number).GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetGreen() * number), 0, 255), (int)(color1 * number).GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetBlue() * number), 0, 255), (int)(color1 * number).GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetAlpha() * number), 0, 255), (int)(color1 * number).GetAlpha());

	// Division (/=) operator between a color and a number
	color3 = color1;
	color3 /= number;
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetRed() / number), 0, 255), (int)color3.GetRed());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetGreen() / number), 0, 255), (int)color3.GetGreen());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetBlue() / number), 0, 255), (int)color3.GetBlue());
	CPPUNIT_ASSERT_EQUAL(DE::ClampNumber<int>((int)(color1.GetAlpha() / number), 0, 255), (int)color3.GetAlpha());

	// Equal operator
	color3 = color1;
	CPPUNIT_ASSERT_EQUAL(color1.ToARGB() == color3.ToARGB(), color1 == color3);

	// Not Equal operator
	CPPUNIT_ASSERT_EQUAL(color1.ToARGB() != color3.ToARGB(), color1 != color3);

	// Input stream (>>) operator

	// Output stream (<<) operator
}

void ColorTest::TestGetters()
{
	DE::CColor color = DE::CColor(1.0f, 0.0f, 0.5f, 0.2f);

	// Red
	CPPUNIT_ASSERT_EQUAL(255, (int)color.GetRed());
	
	// Green
	CPPUNIT_ASSERT_EQUAL(0, (int)color.GetGreen());

	// Blue
	CPPUNIT_ASSERT_EQUAL(127, (int)color.GetBlue());

	// Alpha
	CPPUNIT_ASSERT_EQUAL(51, (int)color.GetAlpha());
}

void ColorTest::TestSetters()
{
	// Red as float
	DE::CColor color;
	color.SetRed(0.5f);
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(127 , (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetBlue());

	// Green as float
	color = DE::CColor();
	color.SetGreen(0.2f);
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(51  , (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetBlue());

	// Blue as float
	color = DE::CColor();
	color.SetBlue(1.0f);
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(255 , (int)color.GetBlue());

	// Alpha as float
	color = DE::CColor();
	color.SetAlpha(0.2f);
	CPPUNIT_ASSERT_EQUAL(51  , (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetBlue());

	// Red as unsigned char
	color = DE::CColor();
	color.SetRed(0x22);
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x22, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetBlue());

	// Green as unsigned char
	color = DE::CColor();
	color.SetGreen(0x22);
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x22, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetBlue());

	// Blue as unsigned char
	color = DE::CColor();
	color.SetBlue(0x22);
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0x22, (int)color.GetBlue());

	// Alpha as unsigned char
	color = DE::CColor();
	color.SetAlpha(0x22);
	CPPUNIT_ASSERT_EQUAL(0x22, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(0x00, (int)color.GetBlue());

	// RGBA as float
	color.SetFloat(1.0f, 0.0f, 0.2f, 0.5f);
	CPPUNIT_ASSERT_EQUAL(127, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(255, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0  , (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(51 , (int)color.GetBlue());

	// RGBA as integer
	color.SetInt(255, 0, 51, 127);
	CPPUNIT_ASSERT_EQUAL(127, (int)color.GetAlpha());
	CPPUNIT_ASSERT_EQUAL(255, (int)color.GetRed());
	CPPUNIT_ASSERT_EQUAL(0  , (int)color.GetGreen());
	CPPUNIT_ASSERT_EQUAL(51 , (int)color.GetBlue());

	// RGBA as unsigned char
}

void ColorTest::TestConverters()
{
	DE::CColor color = DE::CColor(1.0f, 0.4f, 0.5f, 0.2f);

	// To ARGB
	CPPUNIT_ASSERT_EQUAL((unsigned long)((static_cast<int>(0.2f * 255) << 24) | (255 << 16) | (static_cast<int>(0.4f * 255) << 8) | (int)(0.5f * 255)), color.ToARGB());

	// To RBGA
	CPPUNIT_ASSERT_EQUAL((unsigned long)((255 << 24) | (static_cast<int>(0.4f * 255) << 16) | (static_cast<int>(0.5f * 255) << 8) | (int)(0.2f * 255)), color.ToRGBA());

	// To ABGR
	CPPUNIT_ASSERT_EQUAL((unsigned long)((static_cast<int>(0.2f * 255) << 24) | (static_cast<int>(0.5f * 255) << 16) | (static_cast<int>(0.4f * 255) << 8) | 255), color.ToABGR());
}
