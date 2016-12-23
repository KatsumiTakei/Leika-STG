#include "Gauge.h"

Gauge::Gauge()
{	
	value.value = 0;
	value.min_value = 0;
	value.max_value = 100;

	value.position = 0;
	value.min_position = 0;
	value.max_position = 400;

	x = 50;
	y = 100;
}

Gauge::~Gauge()
{
}



void Gauge::Render()
{
	value.UpdatePosition();

	GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Skew(30, 0));

	SetLineWidth(2);
	SetColor(D2D1::ColorF(D2D1::ColorF::LightBlue));
	FillRect(x, y, value.max_position, 10);

	SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	FillRect(x, y, value.position, 10);

	SetColor(D2D1::ColorF(D2D1::ColorF::White));
	DrawRect(x-5, y, value.max_position+10, 10);
}

Gauge::Property::operator float() const
{
	return value;
}

float Gauge::Property::operator=(float value)
{
	//return this->value = std::clamp(value,min_value,max_value); // C++17 <algorithm>
	return this->value = max(min_value, min(max_value, value));
}

void Gauge::Property::UpdatePosition()
{
	// ê¸å`ï‚äÆÇ∆ìØÇ∂éÆ
	//                             s                         * (v2-v1)                       * v1            
	position = (value - min_value) / (max_value - min_value) * (max_position - min_position) + min_position;
}
