#include "PlayerCaught.h"

#include "../Utility/GraphLoader.h"

#include "../Player/Player.h"

PlayerCaughtEffect::PlayerCaughtEffect(Player *player, float argAngle)
: count_(0)
{
	is_living_ = true;
	pos = player->getPos();
	angle_ = argAngle;
	spd = Vector2(6.f, 6.f);
	graph_ = GraphLoader::Instance()->FindIndex(".png");
}

void PlayerCaughtEffect::initialize()
{

}

bool PlayerCaughtEffect::update()
{
	if (!is_living_) return false;

	if (++count_ > invisibleTime) is_living_ = false;

	pos.x += cos(angle_) * spd.x;
	pos.y += sin(angle_) * spd.y;

	return true;
}

void PlayerCaughtEffect::draw()
{
	if (count_ < invisibleTime) {
		if (count_ % 2 == 0)	DrawRotaGraphF(pos.x, pos.y, 1.f, angle_, graph_, true, true);
		else DrawRotaGraphF(pos.x, pos.y, 1.f, angle_, graph_, true, false);
	}
}

void PlayerCaughtEffect::finalize()
{

}