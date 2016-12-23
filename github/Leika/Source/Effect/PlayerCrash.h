#pragma once

#include "../Utility/task.h"

class GameObject;

class PlayerCrashEffect : public Task
{
private:
	int count_;

public:
	PlayerCrashEffect(DirectX::SimpleMath::Vector2 argPos, float argAngle);
	~PlayerCrashEffect(){}

	void initialize()override;
	bool update()override;
	void draw()override;
	void finalize()override;
};