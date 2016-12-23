#pragma once

#include "Scene.h"

class LogoScene : public Scene
{
private:
	int count_;

public:
	LogoScene(SceneManager* sceneController);
	~LogoScene();

	void initialize();
	bool update();
	void draw();
	void finalize();
};
