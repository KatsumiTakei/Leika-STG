#pragma once

#include "PlayerWeapon.h"

class Texture;

class ChargeBullet : public PlayerWeapon
{
private:
	
	Texture *texture;
	DirectX::SimpleMath::Vector2 initial_;

public:

	ChargeBullet(DirectX::SimpleMath::Vector2 argPos, double argPower);
	~ChargeBullet() {};

	void initialize()override;
	bool update()override;
	void draw()override;
	void finalize()override;
};