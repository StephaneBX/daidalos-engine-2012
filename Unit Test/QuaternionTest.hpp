#ifndef QUATERNIONTEST_HPP
#define QUATERNIONTEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class QuaternionTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(QuaternionTest);
	CPPUNIT_TEST(TestConstructors);
	CPPUNIT_TEST(TestOperators);
	CPPUNIT_TEST(TestInverse);
	CPPUNIT_TEST(TestNormalize);
	CPPUNIT_TEST(TestLog);
	CPPUNIT_TEST(TestExp);
	CPPUNIT_TEST(TestPower);
	CPPUNIT_TEST(TestYawPitchRoll);
	CPPUNIT_TEST(TestLerp);
	CPPUNIT_TEST(TestSlerp);
	CPPUNIT_TEST(TestLength);
	CPPUNIT_TEST(TestDotProduct);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

	void TestConstructors();
	void TestOperators();
	void TestInverse();
	void TestNormalize();
	void TestLog();
	void TestExp();
	void TestPower();
	void TestYawPitchRoll();
	void TestLerp();
	void TestSlerp();
	void TestLength();
	void TestDotProduct();
};

#endif