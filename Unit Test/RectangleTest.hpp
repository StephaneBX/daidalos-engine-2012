#ifndef RECTANGLETEST_HPP
#define RECTANGLETEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Core/Utility/CVector2.hpp"

class RectangleTest : public CppUnit::TestFixture
{
private:
	daidalosengine::CVector2I m_origin;
	daidalosengine::CVector2I m_size;
	int m_factor;

	CPPUNIT_TEST_SUITE(RectangleTest);
	CPPUNIT_TEST(TestConstructors);
	CPPUNIT_TEST(TestOperators);
	CPPUNIT_TEST(TestGetTop);
	CPPUNIT_TEST(TestGetLeft);
	CPPUNIT_TEST(TestGetBottom);
	CPPUNIT_TEST(TestGetRight);
	CPPUNIT_TEST(TestGetSize);
	CPPUNIT_TEST(TestGetWidth);
	CPPUNIT_TEST(TestGetHeight);
	CPPUNIT_TEST(TestIntersectRectangle);
	CPPUNIT_TEST(TestIntersectPoint);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown() {
		// Nothing to do
	}

	void TestConstructors();
	void TestOperators();
	void TestGetTop();
	void TestGetLeft();
	void TestGetBottom();
	void TestGetRight();
	void TestGetSize();
	void TestGetWidth();
	void TestGetHeight();
	void TestIntersectRectangle();
	void TestIntersectPoint();
};

#endif