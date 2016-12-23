#include "SceneManager.h"

#include "LogoScene.h"
#include "TitleScene.h"
#include "PlayScene.h"

SceneManager::SceneManager(SceneID startSceneID)
{
	scene_ = nullptr;
	nextSceneID_ = startSceneID;
}


SceneManager::~SceneManager()
{
	destroyCurrentScene();
}


bool SceneManager::update()
{
	if (nextSceneID_ != SCENE_NONE)
	{
		destroyCurrentScene();
		scene_ = createNextScene();
		nextSceneID_ = SCENE_NONE;
	}

	if (scene_ != nullptr)
	{
		return scene_->update();
	}
	else
	{
		return false;
	}
}


void SceneManager::draw()
{
	if (scene_ != nullptr)
	{
		scene_->draw();
	}
}


Scene* SceneManager::createNextScene()
{
	Scene* nextScene = nullptr;

	switch (nextSceneID_)
	{

		case SCENE_TITLE:
			nextScene = new TitleScene(this);
			break;

		case SCENE_PLAY:
			nextScene = new PlayScene(this);
			break;
	}

	if (nextScene != nullptr)
	{
		nextScene->initialize();
	}

	return nextScene;
}


void SceneManager::destroyCurrentScene()
{
	if (scene_ != nullptr)
	{
		scene_->finalize();
	}

	delete scene_;
	scene_ = nullptr;
}


void SceneManager::changeScene(SceneID nextSceneID)
{
	nextSceneID_ = nextSceneID;
}
