#pragma once

#include "../Utility/task.h"

class EnemyGeneration : public Task
{
private:
	DirectX::SimpleMath::Vector2 ran;
	int count_;
public:
	EnemyGeneration(DirectX::SimpleMath::Vector2 argPos);
	~EnemyGeneration() {}

	void initialize() override;
	bool update() override;
	void draw() override;
	void finalize() override;

};