#include "SmallBallBullet.h"
#include "../Enemy.h"

#include "../../Utility/GraphManager.h"
#include "../../Control/Control.h"
#include "../../Constant.h"
#include "../../Utility/Debug.h"

using namespace std;
using namespace Constant;

SmallBallBullet::SmallBallBullet(Enemy *argEnemy)
{
	enemy = argEnemy;
	color_ = enemy->color();
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;

	spd.x = 2.f;
	spd.y = 2.f;
	radius_ = 5.f;

	count_ = 0;
	is_living_ = false;
	rate_ = enemy->data().bltkind_;

	(this->*pattern[enemy->data().barrageKind_])();
	patternNum_ = enemy->data().barrageKind_;
}

void SmallBallBullet::initialize()
{
}

bool SmallBallBullet::update()
{
	EnemyBullet::update();

	return true;
}

void SmallBallBullet::draw()
{
	EnemyBullet::draw();
	DrawRectRotaGraph(pos.x, pos.y, 15.f * rate_, 0.f, 15.f, 15.f, 1.0, angle_, graph_, color_);
}

void SmallBallBullet::finalize()
{
	EnemyBullet::finalize();
}