#pragma once

#include "../Utility/task.h"

class Player;

class PlayerCaughtEffect : public Task
{
private:
	int count_;

public:
	PlayerCaughtEffect(Player *player, float argAngle);
	~PlayerCaughtEffect(){}

	void initialize()override;
	bool update()override;
	void draw()override;
	void finalize()override;
};