#include "CircleGauge.h"

CircleGauge::CircleGauge()
	:color_(D2D1::ColorF(0, 0, 0))
{
	value.value = 0.0f;
	value.min_value = 0.0f;
	value.max_value = 1.0f;

	value.position = 0.0f;
	value.min_position = ToRadian(0);
	value.max_position = ToRadian(360);

	x = 200;
	y = 200;
	radius_ = 25.f;
	
}

CircleGauge::~CircleGauge()
{
}

void CircleGauge::setLineWight(float wight)
{
	SetLineWidth(wight);
}

void CircleGauge::setColor(D2D1::ColorF color)
{
	color_ = color;
}

void CircleGauge::Render()
{
	value.UpdatePosition();

	SetArcStartAngle(ToRadian(-90));

	//SetShadowEnable(true);
	SetColor(color_);
	if (type_ == GAUGE_LINE)
	{
		DrawArc(x, y, radius_, value.min_position, value.position);
	}
	else
	{
		FillArc(x, y, radius_, value.min_position, value.position);
	}
}
