#include "LogoScene.h"


LogoScene::LogoScene(SceneManager* SceneManager)
: Scene(SceneManager)
{
	count_ = 0;
}


LogoScene::~LogoScene()
{
}


void LogoScene::initialize()
{
	count_ = 120;
}


bool LogoScene::update()
{
	count_--;

	if (count_ == 0)
	{
		SceneManager_->changeScene(SCENE_TITLE);
	}

	return true;
}


void LogoScene::draw()
{
	DrawFormatString(10, 10, GetColor(255, 0, 0), "Logo Scene");
}


void LogoScene::finalize()
{
}
