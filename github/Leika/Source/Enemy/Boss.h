#pragma once

/**
* @file   Boss.h
* @brief  Boss ÇÃçXêV
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "Enemy.h"
#include "../Utility/Tween.h"

enum ItemID;
class Texture;
class TextManager;
class CircleGauge;
class RectGauge;

class Boss : public Enemy
{
private:
	int finishCnt_;
	int lifeTank_;
	bool isActive_;
	Texture *image_[2];
	TextManager *textManager;
	Tween<DirectX::SimpleMath::Vector2> *tween;
	CircleGauge *lifeGauge;
	RectGauge *lifeBer;
	DirectX::SimpleMath::Vector2 identityPos;

private:
	void Shot();
	void Move();
	void SubLifetank();

public:

	Boss();
	~Boss() {}
	
	void setEnemy(int argCount, int argPattern, int argBrknd,
		DirectX::SimpleMath::Vector2 argPos, DirectX::SimpleMath::Vector2 argSpd, int argBltime, int argBlknd,
		int argCol, int argHp, int argWait, ItemID argItemID)override;

	inline int GetLifeTank()const { return lifeTank_; }


	void initialize()override;
	bool update()override;
	void draw()override;
	void finalize()override;
};