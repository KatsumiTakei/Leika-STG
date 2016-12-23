#pragma once

class Texture;

class Fps 
{
	int startTime_;
	int count_;
	float fps_;

	Texture *graph_;

public:
	Fps();
	~Fps() {};
	bool Update();
	void Draw();
	void Wait();
};
