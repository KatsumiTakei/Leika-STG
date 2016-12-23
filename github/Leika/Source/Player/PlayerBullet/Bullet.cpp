#include "Bullet.h"
#include "../../Utility/GraphManager.h"

using namespace std;

Bullet::Bullet()
{
	spd.y = BULLET_SPEED;
	angle_ = 1.f;
	radius_ = 9.f;
	id_ = OBJECT_PLAYER_BULLET;
	graph_ = GraphManager::Instance()->FindIndexTexture(L"PlayerWeapon.png");
	is_living_ = false;
}

void Bullet::initialize()
{
	PlayerWeapon::initialize();
	GameObject::initialize();

}

bool Bullet::update()
{
	PlayerWeapon::update();

	return true;
}

void Bullet::draw()
{
	PlayerWeapon::draw();
	DrawRotaGraph(pos.x, pos.y, 18, 18, radius_, radius_, 1.0, angle_, graph_);
	GameObject::draw();
}

void Bullet::finalize()
{

}