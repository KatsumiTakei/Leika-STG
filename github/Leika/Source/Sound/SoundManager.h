#pragma once

/**
* @file   SoundManager.h
* @brief  Sound ‚Ì“o˜^Aæ“¾
* @author Katsumi Takei
* @date   2016 / 07 / 11
*/

#include <map>
#include <string>
#include <memory>

enum SOUND_TYPE
{
	SE,
	LOOP,
};

class Sound;

class SoundManager
{
private:
	int index_;
	std::map<std::string, int> sound;

protected:
	SoundManager() {};
	~SoundManager() {};

public:

	static SoundManager *Instance() 
	{
		static SoundManager instance;
		return &instance;
	}

	void Start();
	void AddAndLoadSound(const char *filename);

	void PlaySounds(std::string key, SOUND_TYPE argType)const;
	void StopSounds(std::string key)const;
};