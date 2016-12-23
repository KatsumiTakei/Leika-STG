#include "PlayerWeapon.h"

#include "ChargeBullet.h"
#include "../../Constant.h"
#include "../../Function.h"

#include "../../Utility/GraphManager.h"
#include "../../Sound/SoundManager.h"

#include "../../Utility/Debug.h"

using namespace std;
using namespace Constant;
using namespace DirectX::SimpleMath;

PlayerWeapon::PlayerWeapon()
{
}

float PlayerWeapon::HomingShot()
{
	float pX = pos.x;
	float pY = pos.y;
	float destinationLeft = 1000;
	float angleLeft = 1.f;
	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		GameObject *task = *it;
		if (!task->getIsLiving()) continue;

		if (task->getID() == OBJECT_ENEMY ||
			task->getID() == OBJECT_BOSS)
		{
			float eX = task->getPos().x;
			float eY = task->getPos().y;

			float destinationRight = sqrt((eX - pX) * (eX - pX) + (eY - pY) * (eY - pY));
			float angleRight = atan2(eY - pY, eX - pX);

			if (destinationLeft >= destinationRight)
			{
				destinationLeft = destinationRight;
				angleLeft = angleRight;
				this->spd = Vector2(18.f, 18.f);
			}
		}
	}
	return angleLeft;
}

#pragma region public_function
PlayerWeapon *PlayerWeapon::createChargeBullet(Vector2 argPos, double argPower)
{
	return new ChargeBullet(argPos, argPower);
}

void PlayerWeapon::createBullet(Vector2 argPos, int argMode)
{
	SoundManager::Instance()->PlaySounds("player_shot.wav", SE);
	is_living_ = true;
	pos = argPos;

	spd.x = 0.f;
	spd.y = BULLET_SPEED;
	angle_ = 1.f;

	if (argMode == BULEET_MOVE_HOMING)
	{
		angle_ = HomingShot();
	}

	GameObject::initialize();
}

void PlayerWeapon::initialize()
{
}

bool PlayerWeapon::update()
{
	if (is_living_)
	{
		pos.x += cos(angle_) * spd.x;
		pos.y += sin(angle_) * spd.y;

		if (!isInArea(pos)) is_living_ = false;
	}
	GameObject::update();

	return isInArea(pos);
}

void PlayerWeapon::draw()
{
	if (!is_living_) return;
}

void PlayerWeapon::finalize()
{

}
#pragma endregion