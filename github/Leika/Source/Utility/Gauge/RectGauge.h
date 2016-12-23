#pragma once
#include "Gauge.h"

class RectGauge : public Gauge
{
private:
	D2D1::ColorF color_;
	float wight_;

public:
	RectGauge();
	~RectGauge();

	void setLineWight(float wight);
	void setColor(D2D1::ColorF color);
	void wight(float value) { wight_  = value; }

	void Render() override;
};