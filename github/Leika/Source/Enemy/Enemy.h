#pragma once

/**
* @file   Enemy.h
* @brief  Enemy ‚Ì’ŠÛƒNƒ‰ƒX
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "../GameObject.h"

class EnemyBullet;
class Item;

class Enemy : public GameObject
{
public:
	enum EnemyID
	{
		ENEMY_SMALL,

		ENEMY_BOSS,
	};

protected:
	struct Date{
		int bltTime_, barrageKind_, bltkind_,
			color_, wtime_, wait_, pattern_, itemID;
		EnemyID enemyID;
	};

	bool isSetting_;
	Date data_;
	std::list<EnemyBullet*> enemyBullet;
	Item *item;
	DirectX::SimpleMath::Color color_;

public:
	Enemy();
	~Enemy();

	std::list<EnemyBullet*> bulletList() { return enemyBullet; }
	DirectX::SimpleMath::Color color()const { return color_; }
	inline Date data()const { return data_; }
	inline bool isSetting()const { return isSetting_; }
	inline void isSetting(bool flag) { isSetting_ = flag; }

	virtual void setEnemy(int argCount, int argPattern, int argBrknd,
		DirectX::SimpleMath::Vector2 argPos, DirectX::SimpleMath::Vector2 argSpd, int argBltime, int argBlknd,
		int argCol, int argHp, int argWait, ItemID argItemID) = 0;
	void createEnemy();

	virtual void initialize() override;
	virtual bool update() override;
	virtual void draw() override;
	virtual void finalize() override;

#pragma region patterns
protected:
	void (Enemy::*selectPattern[11])() = {
		&Enemy::pattern0,&Enemy::pattern1,&Enemy::pattern2,&Enemy::pattern3, &Enemy::pattern4,
		&Enemy::pattern5,&Enemy::pattern6,&Enemy::pattern7,&Enemy::pattern8,&Enemy::pattern9,
		&Enemy::pattern10,
	};

	void pattern0();
	void pattern1();
	void pattern2();
	void pattern3();
	void pattern4();
	void pattern5();
	void pattern6();
	void pattern7();
	void pattern8();
	void pattern9();
	void pattern10();
#pragma endregion
};