#ifndef VECTOR2TEST_HPP
#define VECTOR2TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class Vector2Test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Vector2Test);
	CPPUNIT_TEST(TestConstructors);
	CPPUNIT_TEST(TestOperators);
	CPPUNIT_TEST(TestLength);
	CPPUNIT_TEST(TestDotProduct);
	CPPUNIT_TEST(TestDistance);
	CPPUNIT_TEST(TestNormalize);
	CPPUNIT_TEST(TestGetFrom);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {
		// Nothing to do
	}

	void tearDown() {
		// Nothing to do
	}

	void TestConstructors();
	void TestOperators();
	void TestLength();
	void TestDotProduct();
	void TestDistance();
	void TestNormalize();
	void TestGetFrom();
};

#endif