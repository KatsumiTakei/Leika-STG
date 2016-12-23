#pragma once

#include "../Stdafx/stdafx.h"

class Window;
class SceneManager;
class GraphManager;
class InputManager;
class SoundManager;
class Control;
class Direct3D;

class Game
{
private:
	Window *window;
	SceneManager *sceneManager;
	GraphManager *graphManager;
	InputManager *inputManager;
	SoundManager *soundManager;
	Direct3D *direct3d;
	Control *control;

public:

	Game();
	~Game() {}

	void Run();

};