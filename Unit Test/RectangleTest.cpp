#include "RectangleTest.hpp"
#include <Core/Core.hpp>
#include <Core/Utility/CRectangle.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(RectangleTest);

void RectangleTest::setUp() {
	this->m_origin = DE::CVector2I(4, 8);
	this->m_size = DE::CVector2I(14, 8);
	this->m_factor = 2;
}

void RectangleTest::TestConstructors() {
	DE::CRectangleI rectangle;
	CPPUNIT_ASSERT_EQUAL(0, rectangle.GetLeft());
	CPPUNIT_ASSERT_EQUAL(0, rectangle.GetRight());
	CPPUNIT_ASSERT_EQUAL(0, rectangle.GetBottom());
	CPPUNIT_ASSERT_EQUAL(0, rectangle.GetTop());
	CPPUNIT_ASSERT_EQUAL(0, rectangle.GetHeight());
	CPPUNIT_ASSERT_EQUAL(0, rectangle.GetWidth());


	// Value constructor
	rectangle = DE::CRectangleI(this->m_origin.x, this->m_origin.y, this->m_size.x, this->m_size.y);
	CPPUNIT_ASSERT_EQUAL(this->m_origin.x, rectangle.GetLeft());
	CPPUNIT_ASSERT_EQUAL(this->m_origin.y, rectangle.GetTop());
	CPPUNIT_ASSERT_EQUAL(this->m_origin.x + this->m_size.x, rectangle.GetRight());
	CPPUNIT_ASSERT_EQUAL(this->m_origin.y + this->m_size.y, rectangle.GetBottom());
	CPPUNIT_ASSERT_EQUAL(this->m_size.x, rectangle.GetWidth());
	CPPUNIT_ASSERT_EQUAL(this->m_size.y, rectangle.GetHeight());

	// Value constructor
	rectangle = DE::CRectangleI(this->m_origin, this->m_size);
	CPPUNIT_ASSERT_EQUAL(this->m_origin.x, rectangle.GetLeft());
	CPPUNIT_ASSERT_EQUAL(this->m_origin.y, rectangle.GetTop());
	CPPUNIT_ASSERT_EQUAL(this->m_origin.x + this->m_size.x, rectangle.GetRight());
	CPPUNIT_ASSERT_EQUAL(this->m_origin.y + this->m_size.y, rectangle.GetBottom());
	CPPUNIT_ASSERT_EQUAL(this->m_size.x, rectangle.GetWidth());
	CPPUNIT_ASSERT_EQUAL(this->m_size.y, rectangle.GetHeight());

	// Copy constructor
	DE::CRectangleI rectangle2(rectangle);
	CPPUNIT_ASSERT_EQUAL(rectangle.GetLeft(), rectangle2.GetLeft());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetTop(), rectangle2.GetTop());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetRight(), rectangle2.GetRight());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetBottom(), rectangle2.GetBottom());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetWidth(), rectangle2.GetWidth());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetHeight(), rectangle2.GetHeight());
}

void RectangleTest::TestOperators() {
	// + operator
	DE::CRectangleI rectangle = DE::CRectangleI(this->m_origin, this->m_size);
	CPPUNIT_ASSERT_EQUAL(rectangle.GetLeft() + this->m_origin.x, (rectangle + this->m_origin).GetLeft());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetTop() + this->m_origin.y, (rectangle + this->m_origin).GetTop());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetRight() + this->m_origin.x, (rectangle + this->m_origin).GetRight());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetBottom() + this->m_origin.y, (rectangle + this->m_origin).GetBottom());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetWidth(), (rectangle + this->m_origin).GetWidth());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetHeight(), (rectangle + this->m_origin).GetHeight());

	// += operator
	DE::CRectangleI tmp = rectangle;
	tmp += this->m_origin;
	CPPUNIT_ASSERT_EQUAL(rectangle.GetLeft() + this->m_origin.x, tmp.GetLeft());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetTop() + this->m_origin.y, tmp.GetTop());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetRight() + this->m_origin.x, tmp.GetRight());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetBottom() + this->m_origin.y, tmp.GetBottom());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetWidth(), tmp.GetWidth());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetHeight(), tmp.GetHeight());

	// - operator
	CPPUNIT_ASSERT_EQUAL(rectangle.GetLeft() - this->m_origin.x, (rectangle - this->m_origin).GetLeft());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetTop() - this->m_origin.y, (rectangle - this->m_origin).GetTop());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetRight() - this->m_origin.x, (rectangle - this->m_origin).GetRight());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetBottom() - this->m_origin.y, (rectangle - this->m_origin).GetBottom());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetWidth(), (rectangle - this->m_origin).GetWidth());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetHeight(), (rectangle - this->m_origin).GetHeight());

	// -= operator
	tmp = rectangle;
	tmp -= this->m_origin;
	CPPUNIT_ASSERT_EQUAL(rectangle.GetLeft() - this->m_origin.x, tmp.GetLeft());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetTop() - this->m_origin.y, tmp.GetTop());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetRight() - this->m_origin.x, tmp.GetRight());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetBottom() - this->m_origin.y, tmp.GetBottom());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetWidth(), tmp.GetWidth());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetHeight(), tmp.GetHeight());

	// * opearator
	CPPUNIT_ASSERT_EQUAL(rectangle.GetLeft() * this->m_factor, (rectangle * this->m_factor).GetLeft());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetTop() * this->m_factor, (rectangle * this->m_factor).GetTop());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetRight() * this->m_factor, (rectangle * this->m_factor).GetRight());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetBottom() * this->m_factor, (rectangle * this->m_factor).GetBottom());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetWidth() * this->m_factor, (rectangle * this->m_factor).GetWidth());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetHeight() * this->m_factor, (rectangle * this->m_factor).GetHeight());

	// *= operator
	tmp = rectangle;
	tmp *= this->m_factor;
	CPPUNIT_ASSERT_EQUAL(rectangle.GetLeft() * this->m_factor, tmp.GetLeft());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetTop() * this->m_factor, tmp.GetTop());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetRight() * this->m_factor, tmp.GetRight());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetBottom() * this->m_factor, tmp.GetBottom());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetWidth() * this->m_factor, tmp.GetWidth());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetHeight() * this->m_factor, tmp.GetHeight());

	// / operator
	CPPUNIT_ASSERT_EQUAL(rectangle.GetLeft() / this->m_factor, (rectangle / this->m_factor).GetLeft());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetTop() / this->m_factor, (rectangle / this->m_factor).GetTop());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetRight() / this->m_factor, (rectangle / this->m_factor).GetRight());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetBottom() / this->m_factor, (rectangle / this->m_factor).GetBottom());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetWidth() / this->m_factor, (rectangle / this->m_factor).GetWidth());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetHeight() / this->m_factor, (rectangle / this->m_factor).GetHeight());

	// /= operator
	tmp = rectangle;
	tmp /= this->m_factor;
	CPPUNIT_ASSERT_EQUAL(rectangle.GetLeft() / this->m_factor, tmp.GetLeft());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetTop() / this->m_factor, tmp.GetTop());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetRight() / this->m_factor, tmp.GetRight());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetBottom() / this->m_factor, tmp.GetBottom());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetWidth() / this->m_factor, tmp.GetWidth());
	CPPUNIT_ASSERT_EQUAL(rectangle.GetHeight() / this->m_factor, tmp.GetHeight());
}

void RectangleTest::TestGetTop() {
	DE::CRectangleI rectangle = DE::CRectangleI(this->m_origin, this->m_size);

	CPPUNIT_ASSERT_EQUAL(this->m_origin.y, rectangle.GetTop());
}

void RectangleTest::TestGetLeft() {
	DE::CRectangleI rectangle = DE::CRectangleI(this->m_origin, this->m_size);

	CPPUNIT_ASSERT_EQUAL(this->m_origin.x, rectangle.GetLeft());
}

void RectangleTest::TestGetBottom() {
	DE::CRectangleI rectangle = DE::CRectangleI(this->m_origin, this->m_size);

	CPPUNIT_ASSERT_EQUAL(this->m_origin.y + this->m_size.y, rectangle.GetBottom());
}

void RectangleTest::TestGetRight() {
	DE::CRectangleI rectangle = DE::CRectangleI(this->m_origin, this->m_size);

	CPPUNIT_ASSERT_EQUAL(this->m_origin.x + this->m_size.x, rectangle.GetRight());
}

void RectangleTest::TestGetSize() {
	DE::CRectangleI rectangle = DE::CRectangleI(this->m_origin, this->m_size);

	CPPUNIT_ASSERT_EQUAL(this->m_size.x, rectangle.GetSize().x);
	CPPUNIT_ASSERT_EQUAL(this->m_size.y, rectangle.GetSize().y);
}

void RectangleTest::TestGetWidth() {
	DE::CRectangleI rectangle = DE::CRectangleI(this->m_origin, this->m_size);

	CPPUNIT_ASSERT_EQUAL(this->m_size.x, rectangle.GetWidth());
}

void RectangleTest::TestGetHeight() {
	DE::CRectangleI rectangle = DE::CRectangleI(this->m_origin, this->m_size);

	CPPUNIT_ASSERT_EQUAL(this->m_size.y, rectangle.GetHeight());
}

void RectangleTest::TestIntersectRectangle() {
	// Intersect
	DE::CRectangleI rectangle1 = DE::CRectangleI(1, 2, 15, 5);
	DE::CRectangleI rectangle2 = DE::CRectangleI(9, 4, 8, 9);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_INTERSECT, rectangle1.Intersect(rectangle2));

	// Lower than rectangle 1
	rectangle2 = DE::CRectangleI(9, 8, 8, 9);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_OUTSIDE, rectangle1.Intersect(rectangle2));

	// Left of rectangle 1
	rectangle2 = DE::CRectangleI(-10, 2, 10, 5);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_OUTSIDE, rectangle1.Intersect(rectangle2));

	// Right of rectangle 1
	rectangle2 = DE::CRectangleI(17, 2, 10, 5);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_OUTSIDE, rectangle1.Intersect(rectangle2));

	// Top of rectangle 1
	rectangle2 = DE::CRectangleI(3, -8, 10, 9);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_OUTSIDE, rectangle1.Intersect(rectangle2));

	// Intersect - equals
	rectangle2 = rectangle1;
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_INSIDE, rectangle1.Intersect(rectangle2));
}

void RectangleTest::TestIntersectPoint() {
	// Inside
	DE::CRectangleI rectangle = DE::CRectangleI(1, 2, 15, 5);
	DE::CVector2I point = DE::CVector2I(7, 4);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_INSIDE, rectangle.Intersect(point));

	// Lower
	point = DE::CVector2I(7, 8);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_OUTSIDE, rectangle.Intersect(point));

	// Right
	point = DE::CVector2I(17, 4);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_OUTSIDE, rectangle.Intersect(point));

	// Left
	point = DE::CVector2I(0, 4);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_OUTSIDE, rectangle.Intersect(point));

	// Top
	point = DE::CVector2I(11, 1);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_OUTSIDE, rectangle.Intersect(point));

	// Bottom right corner
	point = DE::CVector2I(16, 7);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_INSIDE, rectangle.Intersect(point));

	// Bottom left corner
	point = DE::CVector2I(1, 7);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_INSIDE, rectangle.Intersect(point));

	// Top right corner
	point = DE::CVector2I(16, 2);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_INSIDE, rectangle.Intersect(point));

	// Top left corner
	point = DE::CVector2I(1, 2);
	CPPUNIT_ASSERT_EQUAL(DE::RECTANGLE_INTERSECTION_INSIDE, rectangle.Intersect(point));
}
