#pragma once
/**
* @file   EnemyFactory.h
* @brief  Enemy ÇÃê∂ê¨
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/
#include "../Enemy.h"

enum GameObject::ItemID;

class EnemyFactory
{
private:
	static EnemyFactory *enemyFactory_;
	EnemyFactory();

public:
	std::list<Enemy*> enemy;

	~EnemyFactory() {}

	static EnemyFactory* instance();	
	void Initialize();

	/**
	* 
	* @param id, count, pattern, Brknd, pos, spd, Bltime, Blknd, Col, Hp, Wait, itemID
	*
	*/
	void SetEnemy(int id, int argCount, int argPattern, int argBrknd,
		DirectX::SimpleMath::Vector2 argPos, DirectX::SimpleMath::Vector2 argSpd, int argBltime, int argBlknd,
		int argCol, int argHp, int argWait, GameObject::ItemID itemID);

	inline std::list<Enemy*> enemyList()const { return enemy; }

};