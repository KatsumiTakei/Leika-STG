#include "Player.h"
#include "../Utility/Gauge/CircleGauge.h"

#include "../Utility/Debug.h"
#include "../Utility/TextManager.h"
#include "../Utility/TaskManager.h"
#include "../Utility/GraphManager.h"
#include "../Sound/SoundManager.h"
#include "../InputManager/InputManager.h"

#include "PlayerBullet/PlayerWeapon.h"
#include "PlayerBullet/Bullet.h"
#include "PlayerBullet/Bomb.h"

#include "../Effect/PlayerCrash.h"

namespace
{
	const double MaxPower = 5.;
	const float LowRate = 4.f;
}

using namespace std;
using namespace Constant;
using namespace DirectX::SimpleMath;

Player::Player(Vector2 argPos, Vector2 argSpd, double argPower)
	: state(normal_spd)
{
	pos.x = argPos.x;
	pos.y = argPos.y;
	spd.x = argSpd.x;
	spd.y = argSpd.y;
	life_ = 5;
	maxlife_ = life_;
	power_ = argPower;
	angle_ = 0.f;
	count_ = 0;
	radius_ = 6.f;
	holdingBombsNum_ = 2;
	is_living_ = true;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"player.png");
	image_ = GraphManager::Instance()->FindIndexTexture(L"vital.png");
	input = InputManager::Instance();
	id_ = OBJECT_PLAYER;
	is_invisible_ = false;
	is_unrivaled_ = false;

	for (int i = 0; i < MAX_BULLET; i++)
	{
		playerWeapon.push_back(new Bullet());
	}

	collisionMark = new CircleGauge;
	collisionMark->value.max_value = 1.f;
	collisionMark->value.value = 1.f;
	collisionMark->radius(radius_);
	collisionMark->setColor(D2D1::ColorF(1.f, 0.5, 0.f, 0.5f));
	GameObject::initialize();

}

Player::~Player()
{
	for (list<PlayerWeapon*>::iterator it = playerWeapon.begin(); it != playerWeapon.end(); ++it)
	{
		PlayerWeapon *task = *it;
		
		SafeDelete(task);
	}
}

#pragma region private_function

void Player::ShotBullet()
{
	static int shotNum[5] = { 1,3,5,7,9 };
	static Vector2 shotPos[9] =
	{ Vector2(0.f,-30.f),Vector2(-10.f,-40.f),Vector2(10.f,-40.f),
		Vector2(-30.f,-60.f), Vector2(30.f,-60.f),Vector2(-50.f,-20.f),
		Vector2(50.f,-20.f),  Vector2(-20.f,-30.f),
		Vector2(20.f,-30.f) };

	for (int i = 0; i < shotNum[(int)(power_ - 1.)]; i++)
	{
		for (list<PlayerWeapon*>::iterator it = playerWeapon.begin(); it != playerWeapon.end(); ++it)
		{
			PlayerWeapon *task = *it;
			if (task->getIsLiving()) continue;

			if (state == low_spd)
			{
				if (i % 2 == 1)
				{
					task->createBullet(pos + shotPos[i], BULEET_MOVE_HOMING);
				}
				else
				{
					task->createBullet(pos + shotPos[i], BULEET_MOVE_NOMEL);
				}
			}
			else if (state == normal_spd)
			{
				task->createBullet(pos + shotPos[i], BULEET_MOVE_NOMEL);
			}
			TaskManager::Instance()->Add(task);
			break;
		}
	}
}

void Player::Shot()
{
	if (input->IsPreesed(KEY_SHOT))
	{
		if (++count_ % 4 == 0)
		{
			ShotBullet();
		}
	}
	else if (!input->IsPreesed(KEY_SHOT))
	{
		count_ = 0;
	}
}

void Player::Move()
{
	if (input->IsPreesed(KEY_UP))
	{
		pos.y -= spd.y;
	}
	if (input->IsPreesed(KEY_DOWN))
	{
		pos.y += spd.y;
	}
	if (input->IsPreesed(KEY_RIGHT))
	{
		pos.x += spd.x;
	}
	if (input->IsPreesed(KEY_LEFT))
	{
		pos.x -= spd.x;
	}
	if (state == normal_spd)
	{
		spd.x = StartSpd.x;
		spd.y = StartSpd.y;
	}
	else
	{
		spd.x = StartSpd.x / LowRate;
		spd.y = StartSpd.y / LowRate;
	};
	
	if (pos.x + 10 > PLAYSCREEN_RIGHT) pos.x -= spd.x;
	if (pos.x - 10 < PLAYSCREEN_LEFT) pos.x += spd.x;
	if (pos.y + 20 > PLAYSCREEN_BOTTOM) pos.y -= spd.y;
	if (pos.y - 20 < PLAYSCREEN_TOP) pos.y += spd.y;
}

void Player::KeyAction()
{
	if (input->IsPreesed(KEY_MODE_CHANGE))
	{
		state = low_spd;
	}
	else if (!input->IsPreesed(KEY_MODE_CHANGE))
	{
		state = normal_spd;
	}

	if (TextManager::Instance()->GetIsFinishedTalk())
	{
		Shot();
		if (input->IsPreesed(KEY_BOMB) && !is_unrivaled_
			&& holdingBombsNum_ > 0)
		{
			TaskManager::Instance()->Add(new Bomb());
			holdingBombsNum_--;
			is_unrivaled_ = true;
		}
	}

	Move();
}

#pragma endregion

#pragma region public_function

void Player::initialize()
{
}

bool Player::update()
{
	KeyAction();
	if (is_invisible_)
	{
		static int cnt = 0;
		cnt++;

		if (cnt >= invisibleTime)
		{
			is_invisible_ = false;
			cnt = 0;
		}
	}
	if (power_ >= MaxPower)
	{
		power_ = MaxPower;
		isMaxPowerState_ = true;
	}
	if (life_ < 0)
	{
		is_living_ = false;
	}

	angle_ += DegreeToRadian(30.f);
	collisionMark->x = pos.x;
	collisionMark->y = pos.y;

	GameObject::update();

	return true;
}

void Player::draw()
{
	float alpha;
	if (is_invisible_) 
	{
		alpha = 0.5f;
	}
	else
	{
		alpha = 1.f;
	}

	if (state == low_spd)
	{// ‚ ‚½‚è”»’è‚Ì•\Ž¦
		DrawRotaGraph(pos.x, pos.y, 50.f, 50.f, 25.f, 25.f, 1.0, angle_, image_, Color(1.f, 1.f, 1.f, 0.4f));
	}

	const Color color = Color(1.f, 1.f, 1.f, alpha);
	DrawRotaGraph(pos.x, pos.y, 20.f, 40.f, 10.f, 20.f, 1.0, 0.f, graph_, color);

	if (state == low_spd)
	{// ‚ ‚½‚è”»’è‚Ì•\Ž¦
		collisionMark->Draw(GraphManager::Instance()->renderTarget());
	}
}

void Player::finalize()
{

}

#pragma endregion