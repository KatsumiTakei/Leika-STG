#pragma once

#include "../Utility/task.h"

class Texture;

class BossCrashEffect : public Task
{
private:
	int initial_;
	float alpha_;
	Texture *image_[2];

public:

	BossCrashEffect(DirectX::SimpleMath::Vector2 argPos, int count);
	~BossCrashEffect() {};

	void initialize()override;
	bool update()override;
	void draw()override;
	void finalize()override;
};