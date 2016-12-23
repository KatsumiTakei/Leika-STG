#pragma once

#include "../Stdafx/stdafx.h"

enum KeyCode
{
	KEY_NONE,
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_BOMB,
	KEY_SHOT,
	KEY_PAUSE,
	KEY_MODE_CHANGE,
	KEY_DEBUG,
};

class RePlay;

class InputManager
{
private:
	
	// �L�[�{�[�h�̏��
	static std::unique_ptr<DirectX::Keyboard> keyboard_;
	// �L�[��o�^����}�b�v
	std::map<KeyCode, DirectX::Keyboard::Keys> keyArray_;
	// �������ςȂ��̓��͔��肷��State
	DirectX::Keyboard::State key_;
	// ��x�����̓��͔��肷��State
	static std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> keyTracker_;

	std::unique_ptr<DirectX::GamePad> gamePad;
	std::map<KeyCode, bool> pad_;
	DirectX::GamePad::State state;
protected:

	InputManager() {};
	~InputManager() {};

public:

	friend RePlay;

	static InputManager *Instance()
	{
		static InputManager instance;
		return &instance;
	}

	void Start();
	bool Update();

	bool IsPreesed(KeyCode argKeyCode);
	bool IsTouched(KeyCode argKeyCode);
	inline static DirectX::Keyboard::State keyState(){ return keyboard_.get()->GetState(); }
};