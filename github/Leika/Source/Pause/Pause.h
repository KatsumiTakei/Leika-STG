#pragma once

class SceneManager;
class SoundManager;
class InputManager;
class Texture;

namespace 
{
	const int GameBack = 0;
	const int TitleBack = 1;
	const int StageBack = 2;

	const int ActionBackTitle = 1;
	const int ActionBackRetry = 2;
}

class Pause
{
private:
	
	bool is_pause_;
	bool is_check_;
	bool is_back_;
	int select_;
	int action_;
	Texture *graph_[2];

	InputManager *input;
	SceneManager *sceneManager;
	SoundManager *soundManager;
	void ChackSelect();

public:

	Pause(SceneManager *argSceneManager);
	~Pause() {}

	bool Update();
	void Draw();
	
	inline bool GetIsPause()const { return is_pause_; }
};