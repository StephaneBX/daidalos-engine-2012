#include "FileTest.h"
#include <Core/Core.h>
#include <Core/Utility/CFile.h>

CPPUNIT_TEST_SUITE_REGISTRATION(FileTest);

void FileTest::setUp() 
{
	this->fullname = "C:\\Windows\\notepad.exe";
	this->filename = "notepad.exe";
	this->shortname = "notepad";
	this->extension = "exe";
}

void FileTest::TestConstructors()
{
	DE::CFile file;
	CPPUNIT_ASSERT(!file.FileExists());
}

void FileTest::TestFileExists() 
{
	DE::CFile file("cppunit_dll.dll");
	CPPUNIT_ASSERT(file.FileExists());
}

void FileTest::TestGetFileSize() 
{
	DE::CFile file("cppunit_dll.dll");
	CPPUNIT_ASSERT(file.GetFileSize() == 659456);
}

void FileTest::TestGetFullname() 
{
	DE::CFile file(this->fullname);
	CPPUNIT_ASSERT_EQUAL(this->fullname, file.GetFullname());
}

void FileTest::TestGetFilename() 
{
	DE::CFile file(this->fullname);
	CPPUNIT_ASSERT_EQUAL(this->filename, file.GetFilename());
}

void FileTest::TestGetShortname() 
{
	DE::CFile file(this->fullname);
	CPPUNIT_ASSERT_EQUAL(this->shortname, file.GetShortname());
}

void FileTest::TestGetExtension() 
{
	DE::CFile file(this->fullname);
	CPPUNIT_ASSERT_EQUAL(this->extension, file.GetExtension());
}

