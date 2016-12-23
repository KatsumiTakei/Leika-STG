#include "Tween.h"

namespace Ease
{
	float EaseIn(float t)
	{
		return t * t;
	}

	float EaseOut(float t)
	{
		return 1.0f - t * t;
	}

	float Linear(float t)
	{
		return t;
	}
}