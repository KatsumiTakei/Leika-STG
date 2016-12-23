#pragma once

/**
* @file   PlayScene.h
* @brief  PlayScene ÇÃçXêVÅAï`âÊ
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "Scene.h"

class Player;
class Stage;
class RePlay;
class Pause;
class Fps;
class Texture;
class InputManager;

class PlayScene: public Scene
{
private:
	Player *player;
	Stage *stage;
	RePlay *replay;
	Pause *pause;
	Fps *fps;
	Texture *graph_[2];
	InputManager *input;
	int select_;

public:
	PlayScene(SceneManager* sceneManager);
	~PlayScene();

	void initialize();
	bool update();
	void draw();
	void finalize();
};
