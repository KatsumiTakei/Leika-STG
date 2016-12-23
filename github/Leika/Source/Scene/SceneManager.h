#pragma once

/**
* @file   SceneManager.h
* @brief  Scene ‚ÌŠÇ—AˆÚ“®
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/


enum SceneID
{
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_REPLAY,
	SCENE_PLAY,
};

class Scene;

class SceneManager
{
private:
	Scene* scene_;
	SceneID nextSceneID_;

public:
	SceneManager(SceneID startSceneID);
	~SceneManager();

	bool update();
	void draw();

	Scene* createNextScene();
	void destroyCurrentScene();

	void changeScene(SceneID nextSceneID);
};
