#ifndef FILETEST_HPP
#define FILETEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class FileTest : public CppUnit::TestFixture
{
private:
	std::string fullname;
	std::string filename;
	std::string shortname;
	std::string extension;

	CPPUNIT_TEST_SUITE(FileTest);
	CPPUNIT_TEST(TestConstructors);
	CPPUNIT_TEST(TestFileExists);
	CPPUNIT_TEST(TestGetFileSize);
	CPPUNIT_TEST(TestGetFullname);
	CPPUNIT_TEST(TestGetFilename);
	CPPUNIT_TEST(TestGetShortname);
	CPPUNIT_TEST(TestGetExtension);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown() {}

	void TestConstructors();
	void TestFileExists();
	void TestGetFileSize();
	void TestGetFullname();
	void TestGetFilename();
	void TestGetShortname();
	void TestGetExtension();
};

#endif