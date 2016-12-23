#pragma once

/**
* @file   Item.h
* @brief  Item ÇÃçXêV
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "../GameObject.h"

class Item : public GameObject
{

private:

protected:

	bool isHoming_;

	void ChackPlayerState();

public:
	Item(ItemID id, DirectX::SimpleMath::Vector2 argPos);
	~Item() {}

	virtual void initialize() override;
	virtual bool update() override;
	virtual void draw() override;
	virtual void finalize() override;

	Item *createItem(ItemID id, DirectX::SimpleMath::Vector2 argPos);
};