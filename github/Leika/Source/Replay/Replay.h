#pragma once

class Player;
class Stage;
class UI;
class PlayerWeapon;
class InputManager;

enum
{
	ReplayReproduce,
	ReplayRecode,
};

class RePlay
{
private:
	
	typedef struct RECDATA
	{// ����f�[�^�ۑ��p�f�[�^�\����

		DirectX::Keyboard::State InputKey;
		int  count;
		float x, y, spdX, spdY;
		double power;
	} *LPRECDATA;

	// ����f�[�^�ۑ��p�������̈�̃|�C���^
	LPRECDATA RecData;

	int recFrame_;
	int rePlayFrame_;
	int mode_;

	DirectX::Keyboard::State key_;
	bool isUpdate_;
	clock_t time;
	Player *player;
	Stage *stage;
	UI *ui;
	InputManager *inputManager;

public:

	RePlay(int mode);
	~RePlay();

	inline bool isUpdate() const { return isUpdate_; }

	void Init(int argCount, double argPower, DirectX::SimpleMath::Vector2 argPos, DirectX::SimpleMath::Vector2 argSpd);
	void Initialize();
	
	void Update();
	void Draw();
	void Finalize();

};
