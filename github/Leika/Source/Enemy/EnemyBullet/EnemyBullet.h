#pragma once

/**
* @file   EnemyBullet.h
* @brief  EnemyBullet ÇÃê∂ê¨ÅAçXêV
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "../../GameObject.h"

class Enemy;
class Texture;

enum
{
	BULLET_ORTHODOX_PINK,
	BULLET_ORTHODOX_RED,
	BULLET_THIN_RED,
	BULLET_THIN_GREEN,
	BULLET_THIN_BULE,
	BULLET_THIN_YELLOW,
	BULLET_CIRCLE_BULE,
	BULLET_CIRCLE_RED,
	BULLET_CIRCLE_YERROW,
	BULLET_CIRCLE_RICK,
};

class EnemyBullet : public GameObject
{
protected:

	Enemy *enemy;
	int patternNum_;
	int rate_;

	DirectX::SimpleMath::Color color_;
	enum
	{
		Small,
	};

public:
	EnemyBullet();
	~EnemyBullet() {};

	void calc();

	float homingShot();
	float removeShot();
	void createBullet(Enemy *argEnemy);

	virtual void initialize();
	virtual bool update();
	virtual void draw();
	virtual void finalize();

public:
	void (EnemyBullet::* pattern[15])() = {
		&EnemyBullet::bulletH000,&EnemyBullet::bulletH001, &EnemyBullet::bulletH002,
		&EnemyBullet::bulletH003,&EnemyBullet::bulletH004, &EnemyBullet::bulletH005,
		&EnemyBullet::bulletH006,&EnemyBullet::bulletH007, &EnemyBullet::bulletH008,
		&EnemyBullet::bulletH009,&EnemyBullet::bulletH010, &EnemyBullet::bulletH011,
		&EnemyBullet::bulletH012,&EnemyBullet::bulletH013 ,&EnemyBullet::bulletH014,
	};

	void bulletH000();
	void bulletH001();
	void bulletH002();
	void bulletH003();
	void bulletH004();
	void bulletH005();
	void bulletH006();
	void bulletH007();
	void bulletH008();
	void bulletH009();
	void bulletH010();
	void bulletH011();
	void bulletH012();
	void bulletH013();
	void bulletH014();


};