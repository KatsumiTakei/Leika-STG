#include "Bomb.h"

#include "../../Constant.h"
#include "../../Function.h"
#include "../../Sound/SoundManager.h"
#include "../../Utility/GraphManager.h"

using namespace std;
using namespace Constant;
using namespace DirectX::Colors;
using namespace DirectX::SimpleMath;

Bomb::Bomb()
{
	pos = Vector2();
	count_ = 180;
	radius_ = 32.f;
	angle_ = 0.f;
	is_living_ = true;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"screweffect.png");

	mode_ = DRAW_ADD;
	SoundManager::Instance()->PlaySounds("bomb_use.wav", SE);
}

void Bomb::initialize()
{
}

bool Bomb::update()
{
	if (is_living_) 
	{
		if (count_ < 150.f && count_ % 2 == 0)
		{
			SoundManager::Instance()->PlaySounds("bomb.wav", SE);
		}

		for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
		{
			GameObject *task = *it;
			if (task->getID() == OBJECT_ENEMY ||
				task->getID() == OBJECT_ENEMY_BULLET)
			{
				task->setIsLiving(false);
			}
		}

		angle_ += DegreeToRadian(5.f);
		if (--count_ < 0)
		{
			is_living_ = false;
			finalize();
		}
	}
	return true;
}

void Bomb::draw()
{
	if (is_living_)
	{
		// Ô‰Q
		const Color red = Red;
		pos = Vector2((float)(XorRand() % (PLAYSCREEN_RIGHT / 2) + 100), (float)(XorRand() % (PLAYSCREEN_BOTTOM / 2) + 100));
		DrawRotaGraph(pos.x, pos.y, 64, 64, radius_, radius_, (360 - count_) / 180.f, angle_, graph_, red);
		// —Î‰Q
		const Color green = Green;
		pos = Vector2((float)(XorRand() % (PLAYSCREEN_RIGHT / 2) + 100), (float)(XorRand() % (PLAYSCREEN_BOTTOM / 2) + 100));
		DrawRotaGraph(pos.x, pos.y, 64, 64, radius_, radius_, (360 - count_) / 180.f, angle_, graph_, green);
		// Â‰Q
		const Color blue = Blue;
		pos = Vector2((float)(XorRand() % (PLAYSCREEN_RIGHT / 2) + 100), (float)(XorRand() % (PLAYSCREEN_BOTTOM / 2) + 100));
		DrawRotaGraph(pos.x, pos.y, 64, 64, radius_, radius_, (360 - count_) / 180.f, angle_, graph_, blue);
	}
}

void Bomb::finalize()
{
	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		GameObject* task = *it;
		if (task->getID() == OBJECT_PLAYER)
		{
			task->SetIsUnrivaled(false);
			break;
		}
	}
}