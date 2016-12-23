#include "Enemy.h"

#include "../Utility/TaskManager.h"
#include "../Item/Item.h"
#include "../Effect/EnemyCrash.h"
#include "EnemyBullet/EnemyBullet.h"
#include "EnemyBullet/SmallBallBullet.h"
#include "../Control/Control.h"
#include "../Constant.h"
#include "../Function.h"
#include "../Player/Player.h"
#include "../Player/PlayerBullet/PlayerWeapon.h"

#include "../Utility/GraphManager.h"
#include "../Utility/TextManager.h"
#include "../Sound/SoundManager.h"
#include "../Utility/Debug.h"

using namespace std;
using namespace Constant;
using namespace DirectX::SimpleMath;

namespace 
{
	const int MAX_ENEMY_BULLET = 350;
}

Enemy::Enemy()
	:color_(Color(1, 1, 1, 1))
{
	for (int i = 0; i < MAX_ENEMY_BULLET; i++)
	{
		enemyBullet.push_back(new SmallBallBullet(this));
	}
}

Enemy::~Enemy()
{
	for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
	{
		EnemyBullet *task = *it;
		if (task)
		{
			delete task;
		}
	}
	delete item;
}
#pragma region private_function


#pragma endregion

#pragma region public_function

void Enemy::createEnemy()
{
	is_living_ = true;
	isSetting_ = false;
	if (this->data_.enemyID == ENEMY_BOSS)
	{
		for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
		{
			GameObject *task = *it;
			if (task->getID() == OBJECT_ENEMY ||
				task->getID() == OBJECT_ENEMY_BULLET)
			{
				task->setIsLiving(false);
			}
		}
		TextManager::Instance()->SetStartText();
	}

	GameObject::initialize();
}

void Enemy::initialize()
{	
	id_ = OBJECT_ENEMY;
}

bool Enemy::update()
{
	if (is_living_)
	{
		(this->*selectPattern[data_.pattern_])();
		pos.x += cos(angle_) * spd.x;
		pos.y += sin(angle_) * spd.y;
		count_++;
		if ((count_ >= data_.bltTime_) && (count_ % 20 == 0))
		{
			for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
			{
				EnemyBullet *task = *it;

				if (task->getIsLiving()) continue;

				task->createBullet(this);

				TaskManager::Instance()->Add(task);
				break;
			}
		}
		if (pos.x < PLAYSCREEN_LEFT - 20.f || PLAYSCREEN_RIGHT + 20 < pos.x
			|| pos.y < PLAYSCREEN_TOP - 20.f || PLAYSCREEN_BOTTOM + 20 < pos.y)
		{
			is_living_ = false;
		}

		if (life_ <= 0)
		{
			is_living_ = false;
			finalize();
		}
		GameObject::update();

	}
	else
	{
		return false;
	}
	return true;
}

void Enemy::draw()
{
	if (!is_living_) return;
}

void Enemy::finalize()
{
	TaskManager *taskManager = TaskManager::Instance();
	taskManager->Add(new EnemyCrashEffect(this));
	taskManager->Add(item->createItem(static_cast<Item::ItemID>(data_.itemID), pos));

	Control::Instance()->score(Control::Instance()->score() + 100);
	SoundManager::Instance()->PlaySounds("enemy_crash.wav", SE);
}

#pragma endregion

#pragma region pattern_function

//移動パターン0
//下がってきて停滞して上がっていく
void Enemy::pattern0()
{
	int t = count_;
	if (t == 0)
	{
		spd.y = 3.f;
	}
	else if (t == 40)
	{
		spd.y = 0.f;
	}
	else if (t == 40 + data_.wait_)
	{
		spd.y = -3.f;
	}
}

//移動パターン1
//下がってきて停滞して左下に行く
void Enemy::pattern1()
{
	int t = count_;
	if (t == 0)
	{
		spd.y = 3.f;
	}
	else if (t == 40)
	{
		spd.y = 0.f;
	}
	else if (t == 40 + data_.wait_)
	{
		spd.x = -1.f;
		spd.y = 2.f;
	}
}

//移動パターン2
//下がってきて停滞して右下に行く
void Enemy::pattern2()
{
	int t = count_;
	if (t == 0)
	{
		spd.y = 3.f;
	}
	else if (t == 40)
	{
		spd.y = 0.f;
	}
	else if (t == 40 + data_.wait_)
	{
		spd.x = 1.f;
		spd.y = 2.f;
	}
}

//行動パターン3
//すばやく降りてきて左へ
void Enemy::pattern3()
{
	int t = count_;
	if (t == 0)
	{
		spd.y = 5.f;
	}
	else if (t < 100)
	{
		spd.x -= 5.f / 100.f;
		spd.y -= 5.f / 100.f;
	}
}

//行動パターン4
//すばやく降りてきて右へ
void Enemy::pattern4()
{
	int t = count_;
	if (t == 0)
	{
		spd.y = 5.f;
	}
	else if (t < 100)
	{
		spd.x += 5.f / 100.f;
		spd.y -= 5.f / 100.f;
	}
}

//行動パターン5
//斜め左下へ
void Enemy::pattern5()
{
	int t = count_;
	if (t == 0) 
	{
		spd.x -= 1.f;
		spd.y = 2.f;
	}
}

//行動パターン6
//斜め右下へ
void Enemy::pattern6()
{
	int t = count_;
	if (t == 0) 
	{
		spd.x += 1.f;
		spd.y = 2.f;
	}
}

//移動パターン7
//停滞してそのまま左上に
void Enemy::pattern7()
{
	int t = count_;
	if (t == data_.wait_) 
	{
		spd.x = -0.7f;
		spd.y = -0.3f;
	}
}

//移動パターン8
//停滞してそのまま右上に
void Enemy::pattern8()
{
	int t = count_;
	if (t == data_.wait_) 
	{
		spd.x = 0.7f;
		spd.y = -0.3f;
	}
}

//移動パターン9
//停滞してそのまま上に
void Enemy::pattern9()
{
	int t = count_;
	if (t == data_.wait_)
	{
		spd.y = -1.0f;
	}
}

//移動パターン10
//下がってきてウロウロして上がっていく
void Enemy::pattern10()
{
	int t = count_;
	if (t == 0)
	{
		spd.y = 4.f;
	}
	else if (t == 40)
	{
		spd.y = 0.f;
	}
	else if (t >= 40)
	{
		if (t % 60 == 0)
		{
			int r = cos(angle_) < 0 ? 0 : 1;
			angle_ = (float)(rang(PI / 4) + PI * r);
		}
	}
	if (t >= 40 + data_.wait_)
	{
		spd.y -= 0.05f;
	}
}

#pragma endregion
