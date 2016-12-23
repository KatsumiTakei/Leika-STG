#pragma once
namespace Ease
{
	float EaseIn(float t);
	float EaseOut(float t);
	float Linear(float t);
}

typedef float(*EaseMethod)(float t);
template <typename T, typename U = T>
class Tween 
{
private:
	T &value;
	U start, end;
	float time;
	float step;
	float duration;
	float frame;
	EaseMethod ease;

public:
	Tween(T &vallue, U end, float d, EaseMethod ease = Ease::Linear) :
		value(vallue),start(vallue),end(end),	step(1.0f),
		duration(d),frame(0.0f),ease(ease) {}
	Tween(T &vallue, U start, U end, float d, EaseMethod ease = Ease::Linear) :
		value(vallue),start(start),end(end),	step(1.0f),
		duration(d),frame(0.0f),ease(ease) {}
	bool Update() 
	{
		frame += step;
		float t = ease(frame / duration);
		value = (end - start) * (1.0f - t * t * t) + start;
		return frame <= duration;
	}
};
