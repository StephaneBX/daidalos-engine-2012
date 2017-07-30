#ifndef MEMORYALLOCATORTEST_HPP
#define MEMORYALLOCATORTEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class MemoryAllocatorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(MemoryAllocatorTest);
	CPPUNIT_TEST(TestConstructors);
	CPPUNIT_TEST(TestAllocate);
	CPPUNIT_TEST(TestNewDelete);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

	void TestConstructors();
	void TestAllocate();
	void TestNewDelete();
};

#endif
