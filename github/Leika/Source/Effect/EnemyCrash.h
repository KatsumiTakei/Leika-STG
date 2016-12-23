#pragma once

#include "../Utility/task.h"

class Enemy;

class EnemyCrashEffect : public Task
{
private:
	int count_;
	float size_;

public:
	EnemyCrashEffect(Enemy *enemy);
	~EnemyCrashEffect(){}

	void initialize()override;
	bool update()override;
	void draw()override;
	void finalize()override;
};