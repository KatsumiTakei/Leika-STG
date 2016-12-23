#pragma once

#include "EnemyBullet.h"

class Enemy;

class SmallBallBullet : public EnemyBullet
{
private:

public:
	SmallBallBullet(Enemy *argEnemy);
	~SmallBallBullet(){}

	void initialize()override;
	bool update()override;
	void draw()override;
	void finalize()override;
};
