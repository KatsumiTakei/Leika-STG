#include "ChargeBullet.h"

#include "../../Collision/Collision.h"
#include "../../Function.h"
#include "../../Utility/GraphManager.h"
#include "../../Sound/SoundManager.h"

using namespace std;
using namespace DirectX::SimpleMath;

ChargeBullet::ChargeBullet(Vector2 argPos, double argPower)
{
	id_ = OBJECT_PLAYER_CHARGE_BULLET;
	radius_ = 30.f;
	spd = Vector2(0, -20.f);
	pos = argPos;
	initial_ = pos;
	count_ = 15 * (int)argPower;
	angle_ = 1.f;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"shockWaveEffect.png");
	texture = GraphManager::Instance()->FindIndexTexture(L"chargeShotEffect.png");
	SoundManager::Instance()->PlaySounds("chargeShot.wav", SE);

	is_living_ = true;
	initialize();
}

void ChargeBullet::initialize()
{
	PlayerWeapon::initialize();
	GameObject::initialize();
}

bool ChargeBullet::update()
{
	if (--count_ > 0)
	{
		PlayerWeapon::update();
		const float correction = radius_ * count_ / 10.f;
		CircleShape node = CircleShape(initial_.x - correction, initial_.y - correction, correction);

		for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
		{
			GameObject *task = *it;
			if (task->getID() == OBJECT_ENEMY ||
				task->getID() == OBJECT_ENEMY_BULLET)
			{
				CircleShape inter = CircleShape(task->getPos().x, task->getPos().y, task->getRadius());
				if (CircleToCircle(node, inter))
				{
					task->setIsLiving(false);
				}
			}
		}
	}
	else
	{
		is_living_ = false;
	}
	return true;
}

void ChargeBullet::draw()
{
	PlayerWeapon::draw();
	DrawRotaGraph(pos.x, pos.y, 60.f, 60.f, radius_, radius_, 1.0, DegreeToRadian(135.f), graph_, Color(1, 0, 0, 1));
	DrawRotaGraph(initial_.x, initial_.y, 50.f, 50.f, 25.f, 25.f, 2.0f - (float)count_ / 30.f, 0.f, texture, Color(1.f, 0.5f, 0.5f, 0.5f));
	GameObject::draw();
}

void ChargeBullet::finalize()
{
}