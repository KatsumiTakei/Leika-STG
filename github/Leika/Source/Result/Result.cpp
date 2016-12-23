#include "Result.h"

#include "../GameObject.h"
#include "../Constant.h"
#include "../Utility/GraphManager.h"
#include "../Control/Control.h"
#include "../Function.h"

using namespace std;
using namespace Constant;
using namespace DirectX::SimpleMath;

namespace
{
	const int MESSEGE = 0;
	const int NUM = 1;
}

Result::Result()
{
	is_living_ = true;
	count_ = 300;
	value_ = Control::Instance()->score();
	graph_ = GraphManager::Instance()->FindIndexTexture(L"resultbord.png");
	texture[MESSEGE] = GraphManager::Instance()->FindIndexTexture(L"resultMessege.png");
	texture[NUM] = GraphManager::Instance()->FindIndexTexture(L"number.png");
	priority_ = PRIORITY_UI;
	mode_ = DRAW_ALPHA;
}

void Result::initialize()
{
}

bool Result::update()
{
	if (--count_ < 0) 
	{
		is_living_ = false;
		finalize();
	}
	else if (count_ > 0)
	{
		list<GameObject*> gameObject = GameObject::objectList();
		for (list<GameObject*>::iterator it = gameObject.begin(); it != gameObject.end(); ++it)
		{
			GameObject *task = *it;
			if (task->getID() == OBJECT_ENEMY ||
				task->getID() == OBJECT_ENEMY_BULLET)
			{
				task->setIsLiving(false);
			}
		}
	}

	return true;
}

void Result::draw()
{
	DrawGraph((float)PLAYSCREEN_LEFT, (float)PLAYSCREEN_TOP, graph_);
	const int DigitNum = 9;

	DrawRectGraph(PLAYSCREEN_LEFT * 2.f, PLAYSCREEN_TOP * 2.f, 0, 0, 360.f, 75.f, texture[MESSEGE]);
	DrawNum(Vector2(PLAYSCREEN_LEFT * 15.f, PLAYSCREEN_TOP * 10.f), Vector2(12, 24), static_cast<float>(Control::Instance()->score()), texture[NUM], DigitNum, Color(1.f, 1.f, 1.f, 1.f));

	GraphManager::Instance()->ConfTextColor(D2D1::ColorF(1, 1, 1));
	GraphManager::Instance()->DrawTextWrite(L"Œ»Ý‚ÌƒXƒRƒA", Vector2(PLAYSCREEN_LEFT * 5.f, PLAYSCREEN_TOP * 15.f));
	GraphManager::Instance()->ConfTextColor(D2D1::ColorF(0, 0, 0));

}

void Result::finalize()
{
	Control::Instance()->isClear(true);
}