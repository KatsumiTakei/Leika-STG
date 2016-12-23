#pragma once

#include "../Utility/task.h"

class Texture;

class Result : public Task
{
private:
	int value_;
	Texture *texture[2];

public:

	Result();
	~Result() {}
	void initialize()override;
	bool update()override;
	void draw()override;
	void finalize()override;
};