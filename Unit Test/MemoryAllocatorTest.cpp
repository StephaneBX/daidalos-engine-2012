#include "MemoryAllocatorTest.h"
#include "Core\Memory\CMemoryAllocator.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MemoryAllocatorTest);

void MemoryAllocatorTest::TestConstructors() {
	DE::CMemoryAllocator<bool> allocator;

	CPPUNIT_ASSERT_EQUAL(0, allocator.GetSize());
	CPPUNIT_ASSERT_EQUAL(0, allocator.GetCount());
}

void MemoryAllocatorTest::TestAllocate() {
	DE::CMemoryAllocator<bool> allocator;
	allocator.Allocate(10);

	CPPUNIT_ASSERT_EQUAL(10, allocator.GetSize());
	CPPUNIT_ASSERT_EQUAL(0, allocator.GetCount());
}

void MemoryAllocatorTest::TestNewDelete() {
	DE::CMemoryAllocator<bool> allocator;
	allocator.Allocate(10);

	int index = allocator.New();
	CPPUNIT_ASSERT_EQUAL(0, index);
	CPPUNIT_ASSERT_EQUAL(10, allocator.GetSize());
	CPPUNIT_ASSERT_EQUAL(1, allocator.GetCount());

	index = allocator.New();
	CPPUNIT_ASSERT_EQUAL(1, index);
	CPPUNIT_ASSERT_EQUAL(10, allocator.GetSize());
	CPPUNIT_ASSERT_EQUAL(2, allocator.GetCount());

	allocator.Delete(0);
	CPPUNIT_ASSERT_EQUAL(10, allocator.GetSize());
	CPPUNIT_ASSERT_EQUAL(1, allocator.GetCount());

	index = allocator.New();
	CPPUNIT_ASSERT_EQUAL(0, index);
	CPPUNIT_ASSERT_EQUAL(10, allocator.GetSize());
	CPPUNIT_ASSERT_EQUAL(2, allocator.GetCount());

	index = allocator.New();
	CPPUNIT_ASSERT_EQUAL(2, index);
	CPPUNIT_ASSERT_EQUAL(10, allocator.GetSize());
	CPPUNIT_ASSERT_EQUAL(3, allocator.GetCount());
}
