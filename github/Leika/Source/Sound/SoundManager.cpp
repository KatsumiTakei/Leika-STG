#include "SoundManager.h"
#include "Sound.h"
#include "../Window/Window.h"

using namespace std;

void SoundManager::Start()
{
	SOUND_Create(Window::window()->windowHundle());
	index_ = 0;
}

void SoundManager::AddAndLoadSound(const char *filename)
{
	string name = "Resources/Sounds/";
	name += filename;

	sound.insert(map<string, int>::value_type(filename, index_));

	SOUND_Load(index_, name.c_str());
	index_++;
}

/**
 * @fn     サウンドの再生
 * @param  再生したいサウンドのキーと再生形式
 */
void SoundManager::PlaySounds(string key, SOUND_TYPE argType)const
{
	if (argType == LOOP)
	{
		SOUND_PlayLoop(sound.at(key));
	}
	else
	{
		SOUND_Play(sound.at(key));
	}
}

/**
 * @fn     サウンドの停止
 * @param  停止したいサウンドのキーと停止形式
 */
void SoundManager::StopSounds(string key)const
{
	SOUND_Stop(sound.at(key));
}