#ifndef SHAPE3DTEST_H
#define SHAPE3DTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class Shape3DTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(Shape3DTest);
	CPPUNIT_TEST(TestCollisionBoundingBoxSphere);
	CPPUNIT_TEST(TestCollisionBoundingBoxEllipse);
	CPPUNIT_TEST(TestCollisionBoundingBoxBoundingBox);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

	void TestCollisionBoundingBoxSphere();
	void TestCollisionBoundingBoxEllipse();
	void TestCollisionBoundingBoxBoundingBox();
};

#endif
