#pragma once

#include "Enemy.h"

enum ItemID;

class SmallEnemy : public Enemy
{

protected:

public:

	SmallEnemy(/*int argCount,int argPattern,int argBrknd,
		DirectX::SimpleMath::Vector2 argPos, DirectX::SimpleMath::Vector2 argSpd,int argBltime,int argBlknd,
		int argCol,int argHp, int argWait,ItemID argItemID*/);
	~SmallEnemy();

	void setEnemy(int argCount, int argPattern, int argBrknd,
		DirectX::SimpleMath::Vector2 argPos, DirectX::SimpleMath::Vector2 argSpd, int argBltime, int argBlknd,
		int argCol, int argHp, int argWait, ItemID argItemID)override;

	void initialize() override;
	bool update() override;
	void draw() override;
	void finalize() override;
};