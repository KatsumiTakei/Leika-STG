#include "Stdafx/stdafx.h"
#include "Game/Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	Game game;
	game.Run();

	return 0;
}