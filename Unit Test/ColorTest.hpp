#ifndef COLORTEST_HPP
#define COLORTEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class ColorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ColorTest);
	CPPUNIT_TEST(TestConstructors);
	CPPUNIT_TEST(TestOperators);
	CPPUNIT_TEST(TestGetters);
	CPPUNIT_TEST(TestSetters);
	CPPUNIT_TEST(TestConverters);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

	void TestConstructors();
	void TestOperators();
	void TestGetters();
	void TestSetters();
	void TestConverters();
};

#endif
