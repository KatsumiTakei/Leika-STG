#include "EnemyFactory.h"
#include "../SmallEnemy.h"
#include "../Boss.h"

#include "../../Item/Item.h"

EnemyFactory *EnemyFactory::enemyFactory_ = nullptr;

using namespace std;
using namespace DirectX::SimpleMath;

EnemyFactory::EnemyFactory()
{
	for (int i = 0; i < 32; i++)
	{
		enemy.push_back(new SmallEnemy);
	}
	enemy.push_back(new Boss);
}

void EnemyFactory::Initialize()
{
}

EnemyFactory *EnemyFactory::instance()
{
	if (!enemyFactory_)
	{
		enemyFactory_ = new EnemyFactory();
	}
	return enemyFactory_;
}

void EnemyFactory::SetEnemy(int id, int argCount, int argPattern, int argBrknd,
	Vector2 argPos, Vector2 argSpd, int argBltime, int argBlknd,
	int argCol, int argHp, int argWait, GameObject::ItemID itemID)
{
	for (list<Enemy*>::iterator it = enemy.begin(); it != enemy.end(); ++it)
	{
		Enemy *task = *it;
		if (task->isSetting())continue;

		task->setEnemy(argCount, argPattern,
			argBrknd, argPos, argSpd,
			argBltime, argBlknd, argCol,
			argHp, argWait, itemID);
		task->isSetting(true);
		break;
	}
}
