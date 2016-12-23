#include "EnemyBullet.h"

#include "../../Utility/GraphManager.h"
#include "../../Constant.h"
#include "../../Function.h"
#include "../../Control/Control.h"
#include "../../Player/Player.h"
#include "../Enemy.h"
#include "../../Utility/TaskManager.h"

#include "SmallBallBullet.h"
#include "../../Utility/Debug.h"

using namespace std;
using namespace Constant;
using namespace DirectX::SimpleMath;

namespace 
{
	const float RemoveValue = 24.f;
}

EnemyBullet::EnemyBullet()
{
	id_ = OBJECT_ENEMY_BULLET;
	radius_ = 6.f;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"bullets.png");
}

void EnemyBullet::createBullet(Enemy *argEnemy)
{
	is_living_ = true;
	pos = argEnemy->getPos();
	rate_ = enemy->data().bltkind_;

	(this->*pattern[enemy->data().barrageKind_])();
	patternNum_ = enemy->data().barrageKind_;

	GameObject::initialize();
}

void EnemyBullet::calc()
{
	if (patternNum_ == 10)
	{
		angle_ += DegreeToRadian(30.f) / 60.f;
	}
	else if (patternNum_ == 9)
	{
		angle_ -= DegreeToRadian(30.f) / 60.f;
	}
	else if (patternNum_ == 13 ||
		patternNum_ == 14)
	{
		if (pos.x - spd.x < PLAYSCREEN_LEFT)
		{
			angle_ += DegreeToRadian(90.f);
		}
		else if (pos.x + spd.x > PLAYSCREEN_RIGHT)
		{
			angle_ += DegreeToRadian(-90.f);
		}
	}
	else if (patternNum_ == 7)
	{
		if (count_ == 5)
		{
			list<EnemyBullet*> enemyBullet = enemy->bulletList();
			for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
			{
				EnemyBullet *task = *it;

				if (task->getIsLiving()) continue;

				task->createBullet(enemy);

				TaskManager::Instance()->Add(task);
				break;
			}
		}
		if (count_ == 60)
		{
			static float localeSpd = 1.f;
			spd = Vector2(sin(localeSpd) + 1.f, sin(localeSpd) + 1.f);
			localeSpd++;
			if (localeSpd > 4.f)
			{
				localeSpd = 1.f;
			}
		}
	}
	
	pos.x += cos(angle_) * spd.x;
	pos.y += sin(angle_) * spd.y;
	count_++;
}

float EnemyBullet::homingShot() {

	Vector2 playerPos;
	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		GameObject *task = *it;
		if (task->getID() == OBJECT_PLAYER)
		{
			playerPos = task->getPos();
		}
	}
	return atan2(playerPos.y - enemy->getPos().y, playerPos.x - enemy->getPos().x);
}

float EnemyBullet::removeShot() {

	Vector2 playerPos;
	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		GameObject *task = *it;
		if (task->getID() == OBJECT_PLAYER)
		{
			playerPos = task->getPos();
		}
	}
	return atan2((playerPos.y + RemoveValue) - enemy->getPos().y, (playerPos.x + RemoveValue) - enemy->getPos().x);
}

void EnemyBullet::initialize()
{
}

bool EnemyBullet::update()
{
	if (is_living_)
	{
		calc();
		if (!isInArea(pos))
		{
			is_living_ = false;
		}
	}
	GameObject::update();

	return isInArea(pos);
}

void EnemyBullet::draw()
{
	if (!is_living_) return;
	GameObject::draw();
}

void EnemyBullet::finalize()
{
}
/**
* @brief 1発だけ、自機に向かって直線発射
*/
void EnemyBullet::bulletH000()
{
	angle_ = homingShot();
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(4.f, 4.f);
}
/**
* @brief 100カウント中に1発、自機に向かって直線発射(常に自機狙い)
*/
void EnemyBullet::bulletH001()
{
	angle_ = homingShot();
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(4.f, 4.f);
}
/**
* @brief 100カウント中に5発、自機に向かって直線発射(角度記憶)
*/
void EnemyBullet::bulletH002()
{
	float base = 0;
	base = homingShot();
	angle_ = base;
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(5.f, 5.f);
}
/**
* @brief 100カウント中に10発、自機に向かってスピード変化直線発射
*/
void EnemyBullet::bulletH003()
{
	angle_ = homingShot();
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(5.f + 5.0f / 100.f* (float)(count_), 5.f + 5.0f / 100.f * (float)(count_));
}
/**
* @brief 0.1秒に1回ずつ円形発射
*/
void EnemyBullet::bulletH004()
{
	float rot = homingShot();
	static int range = 0;

	angle_ = (float)(rot + PI2 / 20 * range);
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(4.f, 4.f);
	range++;

	if (range > 20)
	{
		range = 0;
	}
	angle_ = angle_ * 2;
}
/**
* @brief ばらまきショット
*/
void EnemyBullet::bulletH005()
{
	angle_ = (float)(homingShot() + rang(PI / 4));
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(2.5f + (float)rang(1.5), 2.5f + (float)rang(1.5));
}

/**
* @brief ばらまきショット(減速)
*/
void EnemyBullet::bulletH006()
{
	angle_ = (float)(homingShot() + rang(PI / 4));
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(3.f + (float)rang(1.5), 3.f + (float)rang(1.5));
}
/**
* @brief ボスの弾幕（1面）バラ降らし
* @brief ボスの残機１の弾幕
*/
void EnemyBullet::bulletH007()
{
	count_ = 0;
	static int range = 0;
	angle_ = DegreeToRadian(90.f);
	pos.x = enemy->getPos().x + cosf((float)range) * 100.f;
	pos.y = enemy->getPos().y - sinf(sinf((float)range) * 100.f) * 20.f + 20.f;
	spd = Vector2(0.f, 0.f);
	range++;
	if (range > 20)
	{
		range = 0;
	}
}

/**
* @brief ボスの弾幕（1面）左オプション
* @brief ボスの残機３の弾幕
*/
void EnemyBullet::bulletH008()
{
	static int range = 0;
	angle_ = -(PI2 / 20 * range);
	pos.x = enemy->getPos().x - 100.f;
	pos.y = enemy->getPos().y - 100.f;
	spd = Vector2(2.f, 2.f);
	range++;
	if (range > 20)
	{
		range = 0;
	}
	else
	{
		list<EnemyBullet*> enemyBullet = enemy->bulletList();
		for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
		{
			EnemyBullet *task = *it;

			if (task->getIsLiving()) continue;

			task->createBullet(enemy);

			TaskManager::Instance()->Add(task);
			break;
		}
	}
}
/**
* @brief ボスの弾幕（1面）米粒クロス右 赤色
* @brief ボスの残機3の弾幕
*/

void EnemyBullet::bulletH009()
{
	static int rot = 10;
	angle_ = DegreeToRadian(PI2 / 60.f) + DegreeToRadian((float)rot);

	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(2.f, 2.f);
	rot += 10;

	if (rot > 360)
	{
		rot = 10;
	}
	else
	{
		list<EnemyBullet*> enemyBullet = enemy->bulletList();
		for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
		{
			EnemyBullet *task = *it;

			if (task->getIsLiving()) continue;

			task->createBullet(enemy);

			TaskManager::Instance()->Add(task);
			break;
		}
	}
}

/**
* @brief ボスの弾幕（1面）米粒クロス右左 緑色
* @brief ボスの残機3の弾幕
*/
void EnemyBullet::bulletH010()
{
	static int rot = 10;
	angle_ = DegreeToRadian(PI2 / 60.f) + DegreeToRadian((float)rot);

	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(2.f, 2.f);
	rot += 10;

	if (rot > 360)
	{
		rot = 10;
	}
	else
	{
		list<EnemyBullet*> enemyBullet = enemy->bulletList();
		for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
		{
			EnemyBullet *task = *it;

			if (task->getIsLiving()) continue;

			task->createBullet(enemy);

			TaskManager::Instance()->Add(task);
			break;
		}
	}
}

/**
* @brief ボスの弾幕（1面）円形ホーミング
* @brief ボスの残機２の弾幕
*/
void EnemyBullet::bulletH011()
{
	// 弾幕の発射範囲
	static int rot = 10;
	// 範囲の計算
	pos.x = enemy->getPos().x + cos(DegreeToRadian((float)rot)) * 100.f;
	pos.y = enemy->getPos().y + sin(DegreeToRadian((float)rot)) * 100.f;
	angle_ = homingShot();
	spd = Vector2(1.f, 1.f);
	rot += 10;
	if (rot > 360)
	{
		rot = 10;
	}
	else
	{
		list<EnemyBullet*> enemyBullet = enemy->bulletList();
		for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
		{
			EnemyBullet *task = *it;

			if (task->getIsLiving()) continue;

			task->createBullet(enemy);

			TaskManager::Instance()->Add(task);
			break;
		}
	}
}

/**
* @brief ボスの弾幕（1面）円形ばらまき
* @brief ボスの残機２の弾幕
*/
void EnemyBullet::bulletH012()
{
	float rot = homingShot();
	static int range = 0;

	angle_ = (float)(rot + PI2 / 20 * range);
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y;
	spd = Vector2(2.f, 2.f);
	range++;

	if (range > 20)
	{
		range = 0;
	}
	else
	{
		list<EnemyBullet*> enemyBullet = enemy->bulletList();
		for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
		{
			EnemyBullet *task = *it;

			if (task->getIsLiving()) continue;

			task->createBullet(enemy);

			TaskManager::Instance()->Add(task);
			break;
		}
	}
	angle_ = angle_ * 2;
}
/**
* @brief ボスの弾幕（1面）菱形反射
* @brief ボスの残機１の弾幕
*/
void EnemyBullet::bulletH013()
{
	static int range = 0;

	angle_ = DegreeToRadian(45.f);
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y + range * 10.f;
	spd = Vector2(4.f, 4.f);
	range++;

	if (range > 3)
	{
		range = 0;
	}
	else
	{
		list<EnemyBullet*> enemyBullet = enemy->bulletList();
		for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
		{
			EnemyBullet *task = *it;

			if (task->getIsLiving()) continue;

			task->createBullet(enemy);

			TaskManager::Instance()->Add(task);
			break;
		}
	}
}
/**
* @brief ボスの弾幕（1面）菱形反射
* @brief ボスの残機１の弾幕
*/
void EnemyBullet::bulletH014()
{
	static int range = 0;

	angle_ = DegreeToRadian(135.f);
	pos.x = enemy->getPos().x;
	pos.y = enemy->getPos().y + range * 10.f;
	spd = Vector2(4.f, 4.f);
	range++;

	if (range > 3)
	{
		range = 0;
	}
	else
	{
		list<EnemyBullet*> enemyBullet = enemy->bulletList();
		for (list<EnemyBullet*>::iterator it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
		{
			EnemyBullet *task = *it;

			if (task->getIsLiving()) continue;

			task->createBullet(enemy);

			TaskManager::Instance()->Add(task);
			break;
		}
	}
}
///**
//* @brief ボスの弾幕（1面）両波左
//* @brief ボスの残機２の弾幕
//* @param なし
//* @return なし
//*/
//void EnemyBullet::bulletH011() 
//{
//	int t = count_;
//	if (t >= 0 && t <= 150 && t % 10 == 0)
//	{
//		// 弾幕の発射範囲
//		static int range = 10;
//		static int completeNum = 0;
//		// 範囲の計算
//		angle_ = sin(DegreeToRadian(PI / 20 * range) * range) + DegreeToRadian(90.f);
//		pos.x = enemy->getPos().x - enemy->getRadius() + 40.f;
//		pos.y = enemy->getPos().y;
//		count_ = 0;
//		spd = Vector2(2.f, 3.5f);
//		range++;
//		if (range > 18)
//		{// 弾幕の範囲制限
//			range = 10;
//			completeNum++;
//		}
//	}
//}
///**
//* @brief ボスの弾幕（1面）両波右
//* @brief ボスの残機２の弾幕
//* @param なし
//* @return なし
//*/
//void EnemyBullet::bulletH012() 
//{
//	int t = count_;
//	if (t >= 0 && t <= 150 && t % 10 == 0)
//	{
//		// 弾幕の発射範囲
//		static int range = 10;
//		static int completeNum = 0;
//		// 範囲の計算
//		angle_ = -sin(DegreeToRadian(PI / 20 * range) * range) + DegreeToRadian(90.f);
//		pos.x = enemy->getPos().x - enemy->getRadius();
//		pos.y = enemy->getPos().y;
//		count_ = 0;
//		spd = Vector2(2.f, 3.5f);
//		range++;
//		if (range > 18)
//		{// 弾幕の範囲制限
//			range = 10;
//			completeNum++;
//		}
//	}
//}

///**
//* @brief ボスの弾幕（1面）本体弾幕、矢形左側
//* @brief ボスの残機３の弾幕
//* @param なし
//* @return なし
//*/
//void EnemyBullet::bulletH009()
//{
//	int t = count_;
//	if (t >= 0 && t <= 150 && t % 10 == 0)
//	{
//		// 弾幕の発射範囲
//		static int range = 1;
//		static int completeNum = 0;
//		// 弾幕の角度（ラジアン）
//		// 範囲の計算
//		angle_ = sin(DegreeToRadian(range + 45.f)) *
//			(float)range / 10.f + DegreeToRadian(90.f) +
//			DegreeToRadian(45.f * completeNum);
//		angle_ += DegreeToRadian(180.f);
//		pos.x = enemy->getPos().x + 10.f;
//		pos.y = enemy->getPos().y;
//		count_ = 0;
//		spd = Vector2(5.f, 3.f);
//		range++;
//		if (range > 5)
//		{// 弾幕の範囲制限
//			range = 1;
//			completeNum++;
//		}
//	}
//}
///**
//* @brief ボスの弾幕（1面）本体弾幕、矢形右側
//* @brief ボスの残機３の弾幕
//* @param なし
//* @return なし
//*/
//void EnemyBullet::bulletH010()
//{
//	int t = count_;
//	if (t >= 0 && t <= 150 && t % 10 == 0)
//	{
//		// 弾幕の発射範囲
//		static int range = 1;
//		static int completeNum = 0;
//		// 弾幕の角度（ラジアン）
//		// 範囲の計算
//		angle_ = sin(-DegreeToRadian(range + 45.f)) *
//			(float)range / 10.f + DegreeToRadian(90.f) +
//			DegreeToRadian(45.f * completeNum);
//		angle_ += DegreeToRadian(180.f);
//		pos.x = enemy->getPos().x - 10.f;
//		pos.y = enemy->getPos().y;
//		count_ = 0;
//		spd = Vector2(5.f, 3.f);
//		range++;
//		if (range > 5)
//		{// 弾幕の範囲制限
//			range = 1;
//			completeNum++;
//		}
//	}
//}

//
//// ボスの弾幕（1面）米粒クロス右
//void EnemyBullet::bulletH009() {
//	int t = count_;
//	if (t >= 0 && t < 1200 && t % 90 == 0)
//	{
//		static int rot = 0;
//		double angle = rang(PI);
//
//		angle_ = angle + sin(PI2 / 60 * rot);
//		pos.x = enemy->getPos().x;
//		pos.y = enemy->getPos().y;
//		count_ = 0;
//		spd = Vector2(2.f, 2.f);
//		rot++;
//		if (rot > 60) rot = 0;
//	}
//	int state = count_ % 2;
//	int cnt = count_;
//	if (30 < cnt && cnt < 120)
//	{
//		spd = spd - (1.2 / 90.0);//90カウントかけて1.2減らす
//		angle_ += (PI / 2) / 90.0 * (state ? -1 : 1);//90カウントかけて90°傾ける
//	}
//}
//
//// ボスの弾幕（1面）米粒クロス左
//void EnemyBullet::bulletH010() {
//	int t = count_;
//	if (t >= 0 && t < 1200 && t % 90 == 0)
//	{
//		static int rot = 0;
//		double angle = rang(PI);
//
//		angle_ = -(angle + sin(PI2 / 60 * rot));
//		pos.x = enemy->getPos().x;
//		pos.y = enemy->getPos().y;
//		count_ = 0;
//		spd = Vector2(2.f, 2.f);
//		rot++;
//		if (rot > 1) rot = 0;
//	}
//	int state = count_ % 2;
//	int cnt = count_;
//	if (30 < cnt && cnt < 120)
//	{
//		spd = spd - (1.2 / 90.0);
//		angle_ += (PI / 2) / 90.0 * (state ? -1 : 1);//90カウントかけて90°傾ける
//	}
//}



//// ボスの弾幕（1面）ワイドショット
//void EnemyBullet::bulletH011() {
//	int t = count_;
//	if (t >= 0 && t <= 150 && t % 10 == 0)
//	{
//		static int rot = 10;
//		angle_ = (PI2 / 20 * rot);
//		angle_ = sin(angle_ * PI / 180.f) * rot;
//		pos.x = enemy->getPos().x + cos(PI / 2.f + PI / 150.f * t) * 100;
//		pos.y = enemy->getPos().y + sin(PI / 2.f + PI / 150.f * t) * 100;
//		count_ = 0;
//		spd = Vector2(1.f, 1.2f);
//		rot++;
//		if (rot > 20) rot = 10;
//	}
//}