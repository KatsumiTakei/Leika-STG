#include "Item.h"

#include "../Utility/GraphManager.h"
#include "../Constant.h"
#include "../Function.h"
#include "../Utility/Debug.h"

using namespace std;
using namespace Constant;
using namespace DirectX::SimpleMath;

Item::Item(ItemID id, Vector2 argPos)
{
	isHoming_ = false;
	type_ = id;
	pos.x = argPos.x;
	pos.y = argPos.y;
	spd.y = 1.5f;
	angle_ = 1.f;
	radius_ = 16.f;
	is_living_ = true;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"item.png");
	id_ = OBJECT_ITEM;
	initialize();

}
#pragma region private_function
void Item::ChackPlayerState()
{
	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		GameObject* task = *it;
		if (task->getID() == OBJECT_PLAYER)
		{
			if (task->isMaxPowerState())
			{
				type_ = ITEM_SCORE;
			}
			if (task->getPos().y < PLAYSCREEN_TOP + 64.f)
			{
				isHoming_ = true;
			}
			if (isHoming_)
			{
				float dest = atan2(task->getPos().y - pos.y, task->getPos().x - pos.x);
				angle_ = dest;
				spd = Vector2(8.f, 8.f);
			}
			break;
		}
	}
}

#pragma endregion
#pragma region public_function
Item *Item::createItem(ItemID id, Vector2 argPos)
{
	return new Item(id, argPos);
}

void Item::initialize()
{
	GameObject::initialize();
	ChackPlayerState();
}

bool Item::update()
{
	if (is_living_)
	{
		ChackPlayerState();
		pos.x += cos(angle_) * spd.x;
		pos.y += sin(angle_) * spd.y;

		if (!isInArea(pos))
		{
			is_living_ = false;
		}
		GameObject::update();
	}
	else
	{
		return false;
	}
	return isInArea(pos);
}

void Item::draw()
{
	if (is_living_) 
	{
		if (type_ == ITEM_POWER)
		{
			DrawRotaGraph(pos.x, pos.y, 16, 16, 1.0, 0, graph_);
		}
		else if (type_ == ITEM_SCORE)
		{
			DrawRectRotaGraph(pos.x, pos.y, 16, 0, 16, 16, 1.0, 0, graph_);
		}
		GameObject::draw();
	}
}

void Item::finalize()
{
}
#pragma endregion