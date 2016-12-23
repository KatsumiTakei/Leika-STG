#include "PlayerCrash.h"

#include "../Utility/GraphManager.h"
#include "../Constant.h"
#include "../GameObject.h"

using namespace Constant;
using namespace DirectX::SimpleMath;

PlayerCrashEffect::PlayerCrashEffect(Vector2 argPos, float argAngle)
: count_(0)
{
	is_living_ = true;
	pos = argPos;
	angle_ = argAngle;
	spd = Vector2(6.f, 6.f);
	mode_ = DRAW_ALPHA;
	priority_ = PRIORITY_PLAYSCREEN;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"playerCrashEffect.png");
}

void PlayerCrashEffect::initialize()
{

}

bool PlayerCrashEffect::update()
{
	if (!is_living_) return false;

	if (++count_ > invisibleTime) is_living_ = false;

	pos.x += cos(angle_) * spd.x;
	pos.y += sin(angle_) * spd.y;

	return true;
}

void PlayerCrashEffect::draw()
{
	if (count_ < invisibleTime)
	{
		if (count_ % 2 == 0)
		{
			DrawRotaGraph(pos.x, pos.y, 16, 16, 1.f, angle_, graph_);
		}
		else
		{
			DrawRotaGraph(pos.x, pos.y, 16, 16, 1.f, angle_, graph_);
		}
	}
}

void PlayerCrashEffect::finalize()
{

}