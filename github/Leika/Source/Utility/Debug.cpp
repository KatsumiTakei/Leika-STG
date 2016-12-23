#include "Debug.h"
#include "../Stdafx/stdafx.h"

Debug *debug = Debug::Instance();

void Debug::CreateConsoleWindow()
{
	AllocConsole();
	freopen_s(&dummy_, "CONOUT$", "w", stdout);
	freopen_s(&dummy_, "CONIN$", "r", stdin);
	fclose(dummy_);
}

void Debug::CloseConsoleWindow()
{
	FreeConsole();
}