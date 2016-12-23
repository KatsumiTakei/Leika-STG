#include "RectGauge.h"

RectGauge::RectGauge()
	:color_(D2D1::ColorF(0, 0, 0))
{
	value.value = 0.0f;
	value.min_value = 0.0f;
	value.max_value = 1.0f;

	value.position = 0.0f;
	value.min_position = 0;
	value.max_position = 0;

	x = 200;
	y = 200;
}

RectGauge::~RectGauge()
{
}

void RectGauge::setLineWight(float wight)
{
	SetLineWidth(wight);
}

void RectGauge::setColor(D2D1::ColorF color)
{
	color_ = color;
}

void RectGauge::Render()
{
	value.UpdatePosition();
	
	SetShadowEnable(true);
	SetColor(color_);
	FillRect(x, y, value.position, wight_);

}
