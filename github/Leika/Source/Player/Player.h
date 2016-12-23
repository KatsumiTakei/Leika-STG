#pragma once

/**
* @file   Player.h
* @brief  Player ÇÃä«óùÅAçXêV
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "../GameObject.h"
#include "../Constant.h"

class Texture;
class PlayerWeapon;
class InputManager;
class CircleGauge;

namespace
{
	const float MaxActivityPower = 140.f;
	const DirectX::SimpleMath::Vector2 StartPos(Constant::PLAYSCREEN_RIGHT / 2.f, 300.f);
	const DirectX::SimpleMath::Vector2 StartSpd(4.f, 4.f);
}

class Player : public GameObject
{
private:

	enum State
	{
		normal_spd,
		low_spd,
	};

	std::list<PlayerWeapon*> playerWeapon;
	State state;
	InputManager *input;
	Texture *image_;
	CircleGauge *collisionMark;

	void Move();
	void Shot();

	void KeyAction();
public:

	Player(DirectX::SimpleMath::Vector2 srgPos, DirectX::SimpleMath::Vector2 argSpd, double argPower);
	~Player();

	inline int getBombsNum()const { return holdingBombsNum_; }
	inline double power()const { return power_; }

	void ShotBullet();

	void initialize() override;
	bool update() override;
	void draw() override;
	void finalize() override;
};