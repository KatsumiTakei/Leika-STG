#include "SmallEnemy.h"

#include "../Utility/GraphManager.h"
#include "../Enemy/EnemyFactory/EnemyFactory.h"

using namespace DirectX::SimpleMath;

SmallEnemy::SmallEnemy()
{
	isSetting_ = false;
	is_living_ = false;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"smallenemy.png");
}

SmallEnemy::~SmallEnemy()
{

}

void SmallEnemy::setEnemy(int argCount, int argPattern, int argBrknd, Vector2 argPos, Vector2 argSpd, 
	int argBltime, int argBlknd, int argCol, int argHp, int argWait, ItemID argItemID)
{
	id_ = OBJECT_ENEMY;
	data_.enemyID = ENEMY_SMALL;
	count_ = argCount;
	data_.pattern_ = argPattern;
	data_.bltkind_ = argBlknd;
	data_.barrageKind_ = argBrknd;
	data_.bltTime_ = argBltime;
	data_.color_ = argCol;
	data_.itemID = argItemID;
	life_ = argHp;
	maxlife_ = life_;
	data_.wait_ = argWait;
	pos.x = argPos.x;
	pos.y = argPos.y;
	spd.x = argSpd.x;
	spd.y = argSpd.y;
	angle_ = 1.f;
	radius_ = 10.f;
	is_living_ = false;
}

#pragma region public_function

void SmallEnemy::initialize()
{

}

bool SmallEnemy::update()
{
	Enemy::update();
	GameObject::update();

	return true;
}

void SmallEnemy::draw()
{
	Enemy::draw();
	DrawRotaGraph(pos.x, pos.y, 20, 20, 1.0, 0.f, graph_);
	GameObject::draw();
}

void SmallEnemy::finalize()
{
	Enemy::finalize();
}

#pragma endregion