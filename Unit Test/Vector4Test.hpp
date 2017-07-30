#ifndef VECTOR4TEST_HPP
#define VECTOR4TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class Vector4Test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Vector4Test);
	CPPUNIT_TEST(TestConstructors);
	CPPUNIT_TEST(TestOperators);
	CPPUNIT_TEST(TestLength);
	CPPUNIT_TEST(TestNormalize);
	CPPUNIT_TEST(TestGetFrom);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

	void TestConstructors();
	void TestOperators();
	void TestLength();
	void TestNormalize();
	void TestGetFrom();
};

#endif