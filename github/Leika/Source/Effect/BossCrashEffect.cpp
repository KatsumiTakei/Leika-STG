#include "BossCrashEffect.h"
#include "../Utility/GraphManager.h"
#include "../Sound/SoundManager.h"
#include "../Function.h"

using namespace DirectX::SimpleMath;

BossCrashEffect::BossCrashEffect(Vector2 argPos, int count)
{
	pos = argPos + Vector2((float)(XorRand() % 30 - 15), (float)(XorRand() % 30 - 15));
	const float localeSpd = (float)(XorRand() % 3 - 3);
	spd = Vector2(localeSpd, localeSpd);
	angle_ = DegreeToRadian((float)(XorRand() % 360));
	radius_ = 25.f;
	count_ = count * 2;
	initial_ = count_;
	priority_ = PRIORITY_PLAYSCREEN;
	mode_ = DRAW_ADD;

	//image_[0] = GraphManager::Instance()->FindIndexTexture(L"bossCrashEffect.png");
	graph_ = GraphManager::Instance()->FindIndexTexture(L"bossCrashEffect.png");
	//SoundManager::Instance()->PlaySounds("bossCrash.wav", SE);

	is_living_ = true;
}

void BossCrashEffect::initialize()
{

}

bool BossCrashEffect::update()
{
	if (--count_ < 0)
	{
		is_living_ = false;
	}

	if (count_ < initial_ / 1.5f)
	{
		pos.x += cos(angle_) * spd.x;
		pos.y += sin(angle_) * spd.y;
	}
	return true;
}

void BossCrashEffect::draw()
{
	const float r = (float)(XorRand() % 10 + 1) / 10.f;
	const float g = (float)(XorRand() % 10 + 1) / 10.f;
	const float b = (float)(XorRand() % 10 + 1) / 10.f;
	DrawRotaGraph(pos.x, pos.y, 50, 50, radius_, radius_, 1.0, angle_, graph_, Color(r, g, b, 1.f));
}

void BossCrashEffect::finalize()
{
}