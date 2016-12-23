#pragma once

#include "Scene.h"

class Texture;
class RePlay;

class TitleScene: public Scene
{
private:
	enum
	{
		IMAGE_BACK,
		IMAGE_TEXT,
		IMAGE_TITLENAME,
		IMAGE_DEMOCALL,
	};
	int cnt_;
	Texture *graph[4];
	RePlay *replay;

public:
	TitleScene(SceneManager* sceneController);
	~TitleScene() {};

	void initialize();
	bool update();
	void draw();
	void finalize();
};
