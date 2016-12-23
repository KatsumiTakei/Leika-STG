#include "Function.h"
#include "Constant.h"
#include "Utility/task.h"
#include "Utility/GraphManager.h"

using namespace Constant;
using namespace DirectX::SimpleMath;

double rang(double ang)
{
	return (-ang + (int)ang * 2 * (int)XorRand() % (10000 / 10000));
}

bool isInArea(Vector2 vec)
{
	if (vec.x < PLAYSCREEN_LEFT - 24.f||
		vec.x > PLAYSCREEN_RIGHT + 24.f ||
		vec.y < PLAYSCREEN_TOP - 24.f ||
		vec.y > PLAYSCREEN_BOTTOM + 24.f)
	{
		return false;
	}
	return true;
}

float DegreeToRadian(float degree)
{
	return degree * PI / 180.f;
}

int XorRand()
{
	static int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	int t;
	t = (x ^ (x << 11)); 
	x = y; 
	y = z; 
	z = w; 
	return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

void DrawNum(Vector2 pos, Vector2 size, const float n, Texture *graph, int max, Color color)
{
	float w = n;
	//  ì‹Æ—p‚Ì•Ï”
	int i = 0;
	//  •¶š‚Ì•\¦
	if (w == 0.f)
	{
		for (int i = 0; i < max; i++)
			DrawRectGraph(pos.x - i * size.x, pos.y, 0, 0, size.x, size.y, graph, color);
	}
	else
	{
		while (w)
		{
			DrawRectGraph(pos.x - i * size.x, pos.y, ((static_cast<int>(w) % 10) * size.x), 0, size.x, size.y, graph, color);
			w /= 10.f;
			i++;
			if (i >= max) break;
		}
	}
}

bool compareByA(const Task *left, const Task *right)
{
	return (left->priority() < right->priority());
}
