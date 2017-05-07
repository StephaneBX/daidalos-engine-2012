#include "CDaidalosApplication.hpp"

#pragma comment(lib, "DaidalosEngine.lib")
#pragma comment(lib, "GameEngine.lib")

void mainTest(HINSTANCE hInstance, int nCmdShow) {
	DAIDALOS::CDaidalosApplication app(hInstance, nCmdShow);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#ifdef _WIN32
	// Debuging code
	int BreakAlloc = -1;
	_CrtSetBreakAlloc(BreakAlloc);
	// Debuging code
#endif

	mainTest(hInstance, nCmdShow);

#ifdef _WIN32
	// Debuging code
	_CrtDumpMemoryLeaks();
	// Debuging code
#endif

	return 0;
}
