#pragma once

#include "Utility/task.h"
#include "Collision/Shape.h"


enum ObjectID
{
	OBJECT_NONE,
	OBJECT_PLAYER,
	OBJECT_ENEMY,
	OBJECT_BOSS,
	OBJECT_PLAYER_CHARGE_BULLET,
	OBJECT_PLAYER_BULLET,
	OBJECT_PLAYER_TRIGER,
	OBJECT_ENEMY_BULLET,
	OBJECT_ITEM,

	OBJECT_NUM,

};
class Player;
class EnemyBullet;

class GameObject : public Task
{
private:
#pragma region collision
	static void PlayerOnObject(GameObject *player, GameObject *inter);
	static void PlayerBulletOnObject(GameObject *playerBullet, GameObject *inter);
	static void ClearDeadObject();
	bool HitDamage();
	void PotencyItem(GameObject *item);
#pragma endregion
public:
	enum ItemID
	{
		ITEM_POWER,
		ITEM_SCORE,
	};
protected:
	CircleShape collisionCircle_;
	ObjectID id_;
	static std::list<GameObject*>objList;

	//----のちにクラス分けする
	// プレイヤーの変数
	int holdingBombsNum_;
	double power_;
	bool is_invisible_;
	bool is_unrivaled_;
	bool isMaxPowerState_;

	// アイテムの変数
	ItemID type_;

	//-------
	
public:
	GameObject();
	~GameObject() {};
	static void ObjectOnObject();
	static void ClearObjectList();

	inline static std::list<GameObject*> objectList() { return objList; }
	inline ObjectID getID() const { return id_; }

	virtual void initialize()override;
	virtual bool update()override;
	virtual void draw()override;

	//----のちにクラス分けする
	// プレイヤーの関数
	inline bool GetIsUnrivaled()const { return is_unrivaled_; }
	inline void SetIsUnrivaled(bool flag) { is_unrivaled_ = flag; }
	inline bool isMaxPowerState() const { return isMaxPowerState_; }

	//-------

};