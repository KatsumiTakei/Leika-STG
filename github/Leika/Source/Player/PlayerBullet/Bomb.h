#pragma once

#include "../../GameObject.h"

class Bomb : public GameObject
{
private:

public:
	Bomb();
	~Bomb() {}

	void initialize()override;
	bool update()override;
	void draw()override;
	void finalize()override;
};