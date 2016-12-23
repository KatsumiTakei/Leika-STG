#pragma once

#include "../../GameObject.h"

enum WeaponID
{
	BULLET_ID,
	TRIGER_ID,
	CHARGE_ID,
};

namespace
{
	const int BULEET_MOVE_NOMEL = 0;
	const int BULEET_MOVE_HOMING = 1;
	const float BULLET_SPEED = -18.f;
	const int MAX_IAMGE = 2;
	const int MAX_BULLET = 400;
}
class PlayerWeapon : public GameObject
{
protected:
	int image_[MAX_IAMGE];

	float HomingShot();

public:

	PlayerWeapon();
	~PlayerWeapon() {}

	void createBullet(DirectX::SimpleMath::Vector2 argPos, int argMode);
	PlayerWeapon *createChargeBullet(DirectX::SimpleMath::Vector2 argPos, double argPower);

	virtual void initialize();
	virtual bool update();
	virtual void draw();
	virtual void finalize();
};