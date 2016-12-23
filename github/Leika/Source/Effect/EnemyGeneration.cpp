#include "EnemyGeneration.h"

#include "../Utility/TaskManager.h"
#include "../Utility/GraphManager.h"
#include "../Enemy/EnemyFactory/EnemyFactory.h"
#include "../Constant.h"
#include "../Function.h"

#include <random>
#include <time.h>

using namespace DirectX::SimpleMath;

EnemyGeneration::EnemyGeneration(Vector2 argPos)
:count_(180)
{
	pos = argPos;
	ran.x = pos.x + (float)(XorRand() % 150 - 100.f);
	ran.y = pos.y + (float)(XorRand() % 150 - 100.f);
	is_living_ = true;
	mode_ = DRAW_ALPHA;
	priority_ = PRIORITY_PLAYSCREEN;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"magiceffect.png");
}

void EnemyGeneration::initialize()
{
}

bool EnemyGeneration::update()
{
	EnemyFactory *enemyFactory = new EnemyFactory();
	angle_ += DegreeToRadian(18.f);

	if (--count_ <= 0) 
	{
		TaskManager::Instance()->Add(enemyFactory->SetEnemy(0, 0, 11, 1,
			Vector2(ran.x, ran.y), Vector2(0.f, 0.f), 60, 0, 0, 5, 100, (GameObject::ItemID)0));
		is_living_ = false;
	}
	return true;
}

void EnemyGeneration::draw()
{
	if (is_living_)
	{
		const Color color = Color(1.f, 1.f, 1.f, 1.f);
		DrawRotaGraph(ran.x, ran.y, 64.f, 64.f, 32.f, 32.f, count_ / 180.f + 0.5f, angle_, graph_);
	}
}

void EnemyGeneration::finalize()
{
}