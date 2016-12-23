#pragma once

/**
* @file   Stage.h
* @brief  Stage ÇÃä«óùÅAê∂ê¨
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "Utility/task.h"
#include "Stdafx/stdafx.h"

class EnemyFactory;
class Enemy;
class Camera;

namespace
{
	const int CloudNum = 3;
}

class Stage : public Task
{
private:
	struct ENEMYDATA
	{
		int count[100], id[100], x[100], y[100];
		float spd_x[100], spd_y[100];
		int life[100], pattern[100],
			bltime[100], blknd[100], brknd[100], wait[100], itemID[100];
	};
	std::list<Enemy*> usedEnemy;
	std::list<Enemy*> usedList;

	EnemyFactory *enemyFactory;
	ENEMYDATA EnemyDate;

	Camera *camera;

	DirectX::SimpleMath::Matrix localFoward_, localDepth_, cloudWorld_[CloudNum];
	DirectX::Model *background_, *cloud_[CloudNum];
	
	bool is_player_unrivaled_;


	bool LoadEnemy(wchar_t* filename, ENEMYDATA* pENEMYDATA);
	void ScrollBack();
	void setLighting();

public:

	Stage();
	~Stage();

	void GenerationEnemy();
	
	void initialize() override;
	bool update() override;
	void draw() override;
	void finalize() override;

};