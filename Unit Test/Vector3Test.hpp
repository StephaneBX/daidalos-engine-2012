#ifndef VECTOR3TEST_HPP
#define VECTOR3TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class Vector3Test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Vector3Test);
	CPPUNIT_TEST(TestConstructors);
	CPPUNIT_TEST(TestOperators);
	CPPUNIT_TEST(TestLength);
	CPPUNIT_TEST(TestDotProduct);
	CPPUNIT_TEST(TestCrossProduct);
	CPPUNIT_TEST(TestDistance);
	CPPUNIT_TEST(TestNormalize);
	CPPUNIT_TEST(TestGetFrom);
	CPPUNIT_TEST(TestLinearInterpolation);
	CPPUNIT_TEST(TestCosineInterpolation);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

	void TestConstructors();
	void TestOperators();
	void TestLength();
	void TestDotProduct();
	void TestCrossProduct();
	void TestDistance();
	void TestNormalize();
	void TestGetFrom();
	void TestLinearInterpolation();
	void TestCosineInterpolation();
};

#endif