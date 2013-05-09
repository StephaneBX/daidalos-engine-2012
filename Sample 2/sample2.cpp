#include "CSample2Application.h"

#pragma comment(lib, "Daidalos Engine.lib")

void mainTest(HINSTANCE hInstance, int nCmdShow)
{
	SAMPLE2::CSample2Application app(hInstance, nCmdShow);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
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
