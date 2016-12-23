#pragma once
#ifndef _SOUND_H_
#define _SOUND_H_
#include <Windows.h>
#include <stdint.h>
void SOUND_Load(uint32_t index,const char* filename);
void SOUND_Play(int index);
void SOUND_PlayLoop(int index);
void SOUND_Stop(int index);
void SOUND_Create(HWND hwnd);
void SOUND_Release();

#include <memory>

class Sound
{
public:
	Sound();
	~Sound();

	bool Load(uint32_t index, const wchar_t* filename);
	void Play(uint32_t index);
	void PlayLoop(uint32_t index);
	void Stop(uint32_t index);
	bool Create(HWND hwnd);
	void Release();

private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};



#endif _SOUND_H_