#include "application.h"

INT WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, INT)
{
	Application app;
	app.Init();
	app.Run();
	return 0;
}