#pragma once
#include "Gauge.h"

enum GaugeType
{
	GAUGE_FILL,
	GAUGE_LINE,
};

class CircleGauge : public Gauge
{
private:
	D2D1::ColorF color_;
	float radius_;
	GaugeType type_;
public:
	CircleGauge();
	~CircleGauge();
	
	void gaugeType(GaugeType type) { type_ = type; }

	void setLineWight(float wight);
	void setColor(D2D1::ColorF color);
	void radius(float value) { radius_ = value; }

	void Render() override;
};