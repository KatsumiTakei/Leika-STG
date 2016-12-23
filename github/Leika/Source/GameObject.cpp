#include "GameObject.h"

#include "Player/Player.h"
#include "Constant.h"
#include "Control/Control.h"
#include "Collision/Collision.h"

#include "Effect/PlayerCrash.h"
#include "Utility/TaskManager.h"
#include "Sound/SoundManager.h"
#include "Utility/Debug.h"

using namespace std;

list<GameObject*> GameObject::objList;

GameObject::GameObject()
:collisionCircle_(pos.x, pos.y, radius_)
{
	id_ = OBJECT_NONE;
	priority_ = PRIORITY_PLAYSCREEN;
	mode_ = DRAW_ALPHA;

}
#pragma region private_function

void GameObject::PlayerOnObject(GameObject *player, GameObject *inter)
{
	if (inter->getID() == OBJECT_ENEMY ||
		inter->getID() == OBJECT_BOSS)
	{// ボスかエネミーと接触したとき

		bool hit = player->HitDamage();
	}
	if (inter->getID() == OBJECT_ENEMY_BULLET)
	{// 敵の弾に接触したとき

		bool hit = player->HitDamage();
		if (hit)
		{
			inter->setIsLiving(false);
		}
	}
	if (inter->getID() == OBJECT_ITEM)
	{// アイテムと接触したとき

		player->PotencyItem(inter);
	}
}

void GameObject::PlayerBulletOnObject(GameObject *playerBullet, GameObject *inter)
{
	if (inter->getID() == OBJECT_ENEMY ||
		inter->getID() == OBJECT_BOSS)
	{// プレイヤーの弾と接触したとき
		inter->setLife(inter->getLife() - 1);
		playerBullet->setIsLiving(false);
		SoundManager::Instance()->PlaySounds("enemyDamage.wav", SE);
	}
}

bool GameObject::HitDamage()
{
	static const int effect_num = 18;
	static const float effect_angle = 10.f;
	bool result = false;
	if (!is_invisible_ && !is_unrivaled_)
	{
		result = true;
		isMaxPowerState_ = false;
		life_--;
		is_invisible_ = true;
		for (int i = 0; i < effect_num; i++)
		{
			TaskManager::Instance()->Add(new PlayerCrashEffect(pos, effect_angle * (float)i));
		}
		if (life_ != 0)
		{
			SoundManager::Instance()->PlaySounds("damage.wav", SE);
		}
		else
		{
			SoundManager::Instance()->PlaySounds("lastlife.wav", SE);
		}
	}
	return result;
}

void GameObject::PotencyItem(GameObject *item)
{
	if (item->type_ == ITEM_SCORE)
	{
		Control::Instance()->score(Control::Instance()->score() + 100);
	}
	else if (item->type_ == ITEM_POWER)
	{
		power_ += 0.1;
		if (power_ == 5.)
		{
			isMaxPowerState_ = true;
			item->type_ = ITEM_SCORE;

		}
		if (fmod(power_, 1.) == 0.)
		{
			SoundManager::Instance()->PlaySounds("power_up.wav", SE);
		}
	}
	item->setIsLiving(false);
	SoundManager::Instance()->PlaySounds("item_get.wav", SE);
}

void GameObject::ClearDeadObject()
{
	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		GameObject *task = *it;

		if (!task->getIsLiving())
		{// オブジェクトが死んでいたら、あたり判定配列から削除
			it = objList.erase(it);
			it = objList.begin();
		}
	}
	// 配列の切りつめ
	list<GameObject*>(objList).swap(objList);
}
#pragma endregion

#pragma region public_function
void GameObject::ClearObjectList()
{
	objList.clear();
}

void GameObject::ObjectOnObject()
{
	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		GameObject *leftTask = *it;
		// オブジェクトが死んでたら判定を飛ばす
		if (!leftTask->getIsLiving()) continue;

		for (list<GameObject*>::iterator itr = objList.begin(); itr != objList.end(); ++itr)
		{
			GameObject *rightTask = *itr;
			if (!rightTask->getIsLiving()) continue;

			if (CircleToCircle(leftTask->collisionCircle_, rightTask->collisionCircle_))
			{// 円vs円で当たれば

				if (leftTask->getID() == OBJECT_PLAYER
					&& rightTask->getID() != OBJECT_PLAYER)
				{// プレイヤーvs不特定の判定
					PlayerOnObject(leftTask, rightTask);
				}
				if (leftTask->getID() == OBJECT_PLAYER_BULLET
					&& rightTask->getID() != OBJECT_PLAYER_BULLET)
				{// プレイヤーの弾vs不特定の判定
					PlayerBulletOnObject(leftTask, rightTask);
				}
			}
		}
	}
	ClearDeadObject();
}

void GameObject::initialize()
{
	objList.push_back(this);
	collisionCircle_ = CircleShape(pos.x, pos.y, radius_);
}

bool GameObject::update()
{
	collisionCircle_ = CircleShape(pos.x, pos.y, radius_);

	return true;
}

void GameObject::draw()
{
#if defined(_DEBUG)
	//DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(radius_), COLOR(1), true);
#endif 
}
#pragma endregion