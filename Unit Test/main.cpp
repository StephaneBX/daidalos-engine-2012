#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>

#pragma comment(lib, "cppunit_dll.lib")
#pragma comment(lib, "TestRunner.lib")
#pragma comment(lib, "DaidalosEngine.lib")

int main() {
	// Create the event manager and test controller
	CppUnit::TestResult controller;

	// Add a listener that collects test result
	CppUnit::TestResultCollector result;

	controller.addListener(&result);

	// Add a listener that print dots as test run.
	CppUnit::BriefTestProgressListener progress;

	controller.addListener(&progress);

	// Add the top suite to the test runner
	CppUnit::TestRunner runner;
	runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
	runner.run(controller);

	// Print test in a compiler compatible format.
	CppUnit::CompilerOutputter outputter(&result, CppUnit::stdCOut());
	outputter.write();

	// Uncomment this for XML output
	std::ofstream file("cppunit-report.xml");

	CppUnit::XmlOutputter xml(&result, file);

	xml.write();

	file.close();

	return result.wasSuccessful() ? 0 : 1;
}