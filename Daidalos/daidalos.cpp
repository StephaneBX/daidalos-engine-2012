#include "CDaidalosApplication.hpp"

#pragma comment(lib, "DaidalosEngine.lib")
#pragma comment(lib, "GameEngine.lib")

void mainTest(HINSTANCE hInstance, int nCmdShow, const std::string levelName) {
	DAIDALOS::CDaidalosApplication app(hInstance, nCmdShow, levelName);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#ifdef _WIN32
	// Debuging code
	int BreakAlloc = -1;
	_CrtSetBreakAlloc(BreakAlloc);
	// Debuging code
#endif

	std::string levelName = "factory.des";

	// Read the command lines
	if (__argc > 1) {
		for (auto i = 0; i < __argc; ++i) {
			if (std::string(__argv[i]) == "-sc") {
				levelName = std::string(__argv[i + 1]);
			}
		}
	}

	mainTest(hInstance, nCmdShow, levelName);

#ifdef _WIN32
	// Debuging code
	_CrtDumpMemoryLeaks();
	// Debuging code
#endif

	return 0;
}
