#pragma once

/**
* @file   Control.h
* @brief  ‚ ‚½‚è”»’è ‚Ì“o˜^Aˆ—
* @author Katsumi Takei
* @date   2016 / 11 / 09
*/

class Texture;

class Control
{
private:
	Texture *graph_[2];
	std::unique_ptr<DirectX::Model> model_;

	unsigned int score_;
	unsigned int hiScore_;
	bool isCeared_;
	bool isLoading_;

	Control() {};

public:

	~Control() {};

	static Control *Instance() 
	{
		static Control instance;
		return &instance;
	}

	void Start();
	void Initialize();
	bool Update();
	void Draw();
	void Finalize();

public:

#pragma region setter_and_getter
	inline unsigned int hiScore()const { return hiScore_; }
	inline unsigned int score()const { return score_; }
	inline void score(int score) { score_ = score; }
	 
	inline bool isClear()const { return isCeared_; }
	inline void isClear(bool flag) { isCeared_ = flag; }

#pragma endregion

};