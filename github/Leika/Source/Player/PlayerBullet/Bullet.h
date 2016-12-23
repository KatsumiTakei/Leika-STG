#pragma once

/**
* @file   Bullet.h
* @brief  Bullet ÇÃçXêV
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "PlayerWeapon.h"

class Player;

class Bullet : public PlayerWeapon
{
private:

public:
	Bullet();
	~Bullet() {};


	void initialize() override;
	bool update() override;
	void draw() override;
	void finalize() override;

};