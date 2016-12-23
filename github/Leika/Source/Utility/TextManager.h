#pragma once

#include "../Stdafx/stdafx.h"
#include "../Texture/Texture.h"

class TextManager
{
private:
	float drawPointX_, drawPointY_;
	int stringP_, charP_;
	int count_;
	bool is_waiting_;
	bool is_finished_;
	char text[256][256];
	std::string drawtext_;
	Texture *graph_;

	void textUpdate(const char str);

public:
	TextManager() {};
	~TextManager() {}

	static TextManager *Instance() {
		static TextManager instance;
		return &instance;
	}

	inline void SetStartText() { is_finished_ = false; }
	inline bool GetIsFinishedTalk()const { return is_finished_; }

	void Initialize();
	bool Update();
	void Draw();
};