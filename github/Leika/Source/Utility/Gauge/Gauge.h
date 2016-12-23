#pragma once
#include "PaintBrush.h"

class Gauge : public PaintBrush
{
public:
	Gauge();
	~Gauge();

	// �v���p�e�B
	struct Property
	{
		float min_value;
		float max_value;
		float value;

		float min_position;
		float max_position;
		float position;

		operator float() const;
		float operator=(float value);

		void UpdatePosition();
	};

	Property value;

	float x;
	float y;

protected:
	// PaintBrush ����Čp������܂���
	virtual void Render() override;
};