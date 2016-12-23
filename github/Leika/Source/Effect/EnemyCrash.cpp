#include "EnemyCrash.h"

#include "../Utility/GraphManager.h"
#include"../Enemy/Enemy.h"

#include "../Function.h"

EnemyCrashEffect::EnemyCrashEffect(Enemy *enemy)
: count_(0),size_(0.5f)
{
	is_living_ = true;
	pos = enemy->getPos();
	mode_ = DRAW_ALPHA;
	priority_ = PRIORITY_PLAYSCREEN;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"enemyCrashEffect.png");
}

void EnemyCrashEffect::initialize()
{

}

bool EnemyCrashEffect::update()
{
	if (!is_living_) return false;

	if (++count_ > 10) is_living_ = false;

	size_ += 0.06f;
	return true;
}

void EnemyCrashEffect::draw()
{
	if (count_ < 10) 
	{
		DrawRotaGraph(pos.x, pos.y, 32, 32, size_, 0.f, graph_);
		DrawRotaGraph(pos.x, pos.y, 32, 32, size_, DegreeToRadian(180.f), graph_);
	}
}

void EnemyCrashEffect::finalize()
{

}