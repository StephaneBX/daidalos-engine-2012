#ifndef MATRIXTEST_HPP
#define MATRIXTEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class MatrixTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(MatrixTest);
	CPPUNIT_TEST(TestConstructors);
	CPPUNIT_TEST(TestOperators);
	CPPUNIT_TEST(TestInverse);
	CPPUNIT_TEST(TestTranspose);
	CPPUNIT_TEST(TestIdentity);
	CPPUNIT_TEST(TestSetTranslation);
	CPPUNIT_TEST(TestSetRotation);
	CPPUNIT_TEST(TestSetScale);
	CPPUNIT_TEST(TestGetScale);
	CPPUNIT_TEST(TestGetTranslation);
	CPPUNIT_TEST(TestGetRotation);
	CPPUNIT_TEST(TestTransformVector3);
	CPPUNIT_TEST(TestTransformVector4);
	CPPUNIT_TEST(TestDeterminant);
	CPPUNIT_TEST(TestTransformArray);
	CPPUNIT_TEST(TestRotationQuaternion);
	CPPUNIT_TEST(TestDecompose);
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown() {
			// Nothing to do
		}

		void TestConstructors();
		void TestOperators();
		void TestInverse();
		void TestTranspose();
		void TestIdentity();
		void TestSetTranslation();
		void TestSetRotation();
		void TestSetScale();
		void TestGetScale();
		void TestGetTranslation();
		void TestGetRotation();
		void TestTransformVector3();
		void TestTransformVector4();
		void TestDeterminant();
		void TestTransformArray();
		void TestRotationQuaternion();
		void TestDecompose();
};

#endif