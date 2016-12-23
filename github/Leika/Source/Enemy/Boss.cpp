#include "Boss.h"
#include "../Constant.h"
#include "../Utility/Tween.h"
#include "../Effect/BossCrashEffect.h"
#include "EnemyBullet/SmallBallBullet.h"

#include "../Result/Result.h"

#include "../Utility/TextManager.h"
#include "../Utility//GraphManager.h"
#include "../Item/Item.h"
#include "../Control/Control.h"
#include "../Utility/TaskManager.h"
#include "../Enemy/EnemyBullet/EnemyBullet.h"
#include "../Function.h"
#include "../Utility/Debug.h"
#include "../Utility/Gauge/CircleGauge.h"
#include "../Utility/Gauge/RectGauge.h"

using namespace std;
using namespace Constant;
using namespace DirectX::SimpleMath;

namespace
{
	enum
	{
		BackEffect,
		Number,
	};
	const int BulletNum = 8;
}

Boss::Boss()
:textManager(TextManager::Instance()), finishCnt_(60)
{
	for (int i = 0; i < 500; i++)
	{
		enemyBullet.push_back(new SmallBallBullet(this));
	}
	is_living_ = false;
	isSetting_ = false;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"boss.png");
	image_[BackEffect] = GraphManager::Instance()->FindIndexTexture(L"magiceffect.png");
	image_[Number] = GraphManager::Instance()->FindIndexTexture(L"number.png");

}

void Boss::setEnemy(int argCount, int argPattern, int argBrknd, Vector2 argPos, Vector2 argSpd, 
	int argBltime, int argBlknd, int argCol, int argHp, int argWait, ItemID argItemID)
{
	id_ = OBJECT_BOSS;
	data_.enemyID = ENEMY_BOSS;
	count_ = argCount;
	data_.bltkind_ = argBlknd;
	data_.barrageKind_ = argBrknd;
	data_.bltTime_ = argBltime;
	data_.color_ = argCol;
	data_.itemID = argItemID;
	life_ = argHp;
	maxlife_ = life_;
	data_.wait_ = argWait;
	pos.x = argPos.x + 12.f;
	pos.y = argPos.y;
	spd.x = 1.f;
	spd.y = 2.f;
	lifeTank_ = 3;
	radius_ = 16.f;
	is_living_ = false;
	isActive_ = false;
	priority_ = PRIORITY_EFFECT;

	color_ = Color(1, 1, 1, 1);

	tween = nullptr;
	lifeGauge = new CircleGauge();
	lifeGauge->value.max_value = (float)maxlife_;
	lifeGauge->value.value = (float)life_;
	lifeGauge->radius(50.f);
	lifeGauge->setLineWight(3.f);
	lifeGauge->gaugeType(GAUGE_LINE);

	lifeBer = new RectGauge();
	lifeBer->value.max_value = (float)maxlife_;
	lifeBer->value.max_position = (float)PLAYSCREEN_RIGHT - PLAYSCREEN_LEFT - 120.f;
	lifeBer->value.value = (float)life_;
	lifeBer->setColor(D2D1::ColorF(0, 1, 0));
	lifeBer->wight(5.f);
	GameObject::initialize();
}

#pragma region private


void Boss::Shot()
{
	static bool isMainShot = false;
	if (count_ >= data_.bltTime_)
	{
		{
			if (count_ % 100 == 0 && lifeTank_ == 3)
			{
				data_.barrageKind_ = 9;
				data_.bltkind_ = BULLET_THIN_RED;

				for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
				{
					EnemyBullet *task = *it;

					if (task->getIsLiving()) continue;

					task->createBullet(this);

					TaskManager::Instance()->Add(task);
					break;
				}
			}
			else if (count_ % 100 == 50 && lifeTank_ == 3)
			{
				data_.barrageKind_ = 10;
				data_.bltkind_ = BULLET_THIN_GREEN;

				for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
				{
					EnemyBullet *task = *it;

					if (task->getIsLiving()) continue;

					task->createBullet(this);

					TaskManager::Instance()->Add(task);
					break;
				}
			}
		}
		{
			if (count_ % 100 == 0 && lifeTank_ == 2)
			{
				data_.barrageKind_ = 11;
				data_.bltkind_ = BULLET_CIRCLE_RED;
				for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
				{
					EnemyBullet *task = *it;

					if (task->getIsLiving()) continue;

					task->createBullet(this);

					TaskManager::Instance()->Add(task);
					break;
				}
			}
			else if (count_ % 120 == 0 && lifeTank_ == 2)
			{
				data_.barrageKind_ = 11;
				data_.bltkind_ = BULLET_CIRCLE_RED;
				for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
				{
					EnemyBullet *task = *it;

					if (task->getIsLiving()) continue;

					task->createBullet(this);

					TaskManager::Instance()->Add(task);
					break;
				}
			}
			else if (count_ % 140 == 0 && lifeTank_ == 2)
			{
				data_.barrageKind_ = 11;
				data_.bltkind_ = BULLET_CIRCLE_RED;
				for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
				{
					EnemyBullet *task = *it;

					if (task->getIsLiving()) continue;

					task->createBullet(this);

					TaskManager::Instance()->Add(task);
					break;
				}
			}

			if (count_ % 10 == 0 && lifeTank_ == 2)
			{
				data_.barrageKind_ = 12;
				data_.bltkind_ = BULLET_THIN_BULE;
				for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
				{
					EnemyBullet *task = *it;

					if (task->getIsLiving()) continue;

					task->createBullet(this);

					TaskManager::Instance()->Add(task);
					break;
				}
			}
		}
		{
			if (count_ % 10 == 0 && lifeTank_ == 1)
			{
				if (!isMainShot)
				{
					data_.barrageKind_ = 7;
					data_.bltkind_ = XorRand() % 4 + 2;
					for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
					{
						EnemyBullet *task = *it;

						if (task->getIsLiving()) continue;

						task->createBullet(this);

						TaskManager::Instance()->Add(task);
						break;
					}
				}

				{// óºÉTÉCÉhÇÃíe
					data_.barrageKind_ = 13;
					data_.bltkind_ = BULLET_CIRCLE_BULE;
					for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
					{
						EnemyBullet *task = *it;

						if (task->getIsLiving()) continue;

						task->createBullet(this);

						TaskManager::Instance()->Add(task);
						break;
					}
					data_.barrageKind_++;
					for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
					{
						EnemyBullet *task = *it;

						if (task->getIsLiving()) continue;

						task->createBullet(this);

						TaskManager::Instance()->Add(task);
						break;
					}
				}
				//{// íÜâõÇÃíe
				//	data_.barrageKind_ = 7;
				//	data_.bltkind_ = XorRand() % 4 + 2;
				//}
			}
		}
	}
}

void Boss::Move()
{
	if (lifeTank_ == 2)
	{
		static int localeCount = 0, moveCnt = 0;
		static const int MaxTarget = 3;

		if (data_.bltTime_ + BulletNum * 5.f <= moveCnt)
		{
			static const Vector2 TargetPos[MaxTarget] = {
				Vector2(pos.x - 124.f, pos.y + 20.f) ,
				Vector2(pos.x + 124.f, pos.y + 20.f) ,
				Vector2(pos.x, pos.y - 80.f) };
			
			Vector2 localePos;
			for (int i = 0; i < MaxTarget; i++)
			{
				if (localeCount == i)
				{
					localePos = TargetPos[i];
					break;
				}
			}

			if (!isActive_)
			{
				tween = new Tween<Vector2>(pos, localePos, 60.f, Ease::EaseOut);
				isActive_ = true;
			}
			
			moveCnt -= BulletNum * 5 + 1 + 99;
		}

		if (isActive_)
		{
			isActive_ = tween->Update();
			if (!isActive_)
			{
				localeCount++;
				if (localeCount >= MaxTarget)
				{
					localeCount = 0;
				}
			}
		}
		moveCnt++;
	}
	else
	{
		pos = identityPos;
	}
}

void Boss::SubLifetank()
{
	static  const int SpitOutItemNum = 10;
	if (life_ <= 0)
	{
		count_ = 0;
		life_ = maxlife_;
		lifeTank_--;
		if (lifeTank_ == 2)
		{
			tween = new Tween<Vector2>(pos, Vector2(pos.x, pos.y - 40.f), data_.bltTime_ - 2.f, Ease::EaseOut);
		}
		for (int i = 0; i < SpitOutItemNum; i++)
		{// ÉAÉCÉeÉÄÇÃê∂ê¨
			Vector2 itemPos = Vector2(pos.x - 50.f + static_cast<float>(XorRand() % (100)),
				pos.y - 50.f + static_cast<float>(XorRand() % (100)));
			TaskManager::Instance()->Add(item->createItem(Item::ITEM_POWER, itemPos));
		}

		for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
		{
			GameObject *task = *it;
			if (task->getID() == OBJECT_ENEMY ||
				task->getID() == OBJECT_ENEMY_BULLET)
			{
				task->setIsLiving(false);
			}
		}
	}
	if (lifeTank_ <= 0)
	{
		finalize();
	}
}

#pragma endregion

void Boss::initialize()
{
	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		GameObject *task = *it;
		if (task->getID() == OBJECT_ENEMY ||
			task->getID() == OBJECT_ENEMY_BULLET)
		{// ìGÇ∆ìGÇÃíeÇàÍë|
			task->setIsLiving(false);
		}
	}
}

bool Boss::update()
{
	static bool isTalked = false;

	if (!textManager->GetIsFinishedTalk())
	{
		if (count_ <= 80 && !isTalked)
		{
			pos.y += spd.y;
			life_ = maxlife_;
			count_++;
		}
		else if (count_ >= 80)
		{
			count_ = 0;
			isTalked = true;
		}
		if (isTalked)
		{
			identityPos = pos;
			textManager->Update();
		}
	}
	else
	{
		if (life_ > 0)
		{
			Shot();
			Move();

			static int finishCnt = 0;
			if (finishCnt > 60)
			{
				finishCnt = 0;
				finishCnt_--;
			}
			if (finishCnt_ < 0)
			{
				finishCnt_ = 60;
				count_ = 0;
				life_ = maxlife_;
				lifeTank_--;
			}
			finishCnt++;
		}
		SubLifetank();
		lifeGauge->value.value = (float)life_;
		lifeBer->value.value = (float)life_;

		count_++;
	}

	lifeGauge->x = pos.x;
	lifeGauge->y = pos.y;

	lifeBer->x = (float)PLAYSCREEN_LEFT + 60.f;
	lifeBer->y = (float)PLAYSCREEN_TOP + 12.f;
	GameObject::update();

	return true;
}

void Boss::draw()
{	
	if (textManager->GetIsFinishedTalk())
	{
		{// å„ÇÎÇ…èoÇƒÇ≠ÇÈÉGÉtÉFÉNÉg
			static float localeAngle = 0.f;
			localeAngle += DegreeToRadian(18.f);
			const float r = (float)(XorRand() % 10 + 1) / 10.f;
			const float g = (float)(XorRand() % 10 + 1) / 10.f;
			const float b = (float)(XorRand() % 10 + 1) / 10.f;
			const Color color = Color(r, g, b, 0.25f);
			DrawRotaGraph(pos.x, pos.y, 64.f, 64.f, 32.f, 32.f, 3.f, localeAngle, image_[BackEffect], color);
		}
		{// â~å^ÇÃÉâÉCÉtÉQÅ[ÉW
			if (maxlife_ / 3.f < life_ &&
				life_ < maxlife_ / 3.f * 2.f)
			{
				lifeGauge->setColor(D2D1::ColorF(1, 1, 0, 0.5f));
			}
			else if (maxlife_ / 3.f * 2.f < life_)
			{
				lifeGauge->setColor(D2D1::ColorF(0, 1, 0, 0.5f));
			}
			else
			{
				lifeGauge->setColor(D2D1::ColorF(1, 0, 0, 0.5f));
			}
			lifeGauge->Draw(GraphManager::Instance()->renderTarget());
		}

		{// ñ_èÛÇÃÉâÉCÉtÉQÅ[ÉW
			if (lifeTank_ == 3)
			{
				lifeBer->setColor(D2D1::ColorF(0, 1, 0));
			}
			else if (lifeTank_ == 2)
			{
				lifeBer->setColor(D2D1::ColorF(1, 1, 0));
			}
			else
			{
				lifeBer->setColor(D2D1::ColorF(1, 0, 0));
			}
			lifeBer->Draw(GraphManager::Instance()->renderTarget());
		}

		DrawNum(Vector2((float)PLAYSCREEN_RIGHT - 30.f, (float)PLAYSCREEN_TOP + 15.f), Vector2(12.f, 24.f), (float)finishCnt_, image_[Number], 2, Color(1.f, 1.f, 1.f, 1.f));

		GraphManager::Instance()->ConfTextColor(D2D1::ColorF(1, 1, 1));
		GraphManager::Instance()->DrawTextWrite(L"Boss", Vector2((float)(PLAYSCREEN_LEFT + 12), (float)PLAYSCREEN_TOP));
		GraphManager::Instance()->ConfTextColor(D2D1::ColorF(0, 0, 0));
	}

	DrawRotaGraph(pos.x , pos.y, 64, 64, 32.f, 32.f, 1.f, 0.f, graph_);

	Enemy::draw();
	GameObject::draw();
}

void Boss::finalize()
{
	static int cnt = 60;

	TaskManager::Instance()->Add(new BossCrashEffect(pos, cnt));

	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		GameObject *task = *it;
		if (task->getID() == OBJECT_ENEMY ||
			task->getID() == OBJECT_ENEMY_BULLET)
		{
			task->setIsLiving(false);
		}
	}
	
	if (--cnt < 0)
	{
		is_living_ = false;
		TaskManager::Instance()->Add(new Result());
	}
}