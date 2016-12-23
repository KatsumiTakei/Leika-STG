#include "InputManager.h"

using namespace std;
using namespace DirectX;

std::unique_ptr<DirectX::Keyboard> InputManager::keyboard_(new Keyboard);
std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> InputManager::keyTracker_(new Keyboard::KeyboardStateTracker);

void InputManager::Start()
{
	{// キーの初期化
		// 上
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_UP, keyboard_->Up));
		// 下
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_DOWN, keyboard_->Down));
		// 右
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_RIGHT, keyboard_->Right));
		// 左
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_LEFT, keyboard_->Left));
		// ショット
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_SHOT, keyboard_->Z));
		// ボム
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_BOMB, keyboard_->X));
		// ポーズ
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_PAUSE, keyboard_->A));
		// 低速
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_MODE_CHANGE, keyboard_->LeftShift));
		// デバックに使うキー
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_DEBUG, keyboard_->Space));
	}
	{// パッドの初期化

		gamePad.reset(new GamePad());

		// パッドに接続する人数の設定
		state = gamePad->GetState(0);
		
		// 上
		pad_.insert(map<KeyCode, bool>::value_type(KEY_UP, state.IsDPadUpPressed()));
		// 下
		pad_.insert(map<KeyCode, bool>::value_type(KEY_DOWN, state.IsDPadDownPressed()));
		// 右
		pad_.insert(map<KeyCode, bool>::value_type(KEY_RIGHT, state.IsDPadRightPressed()));
		// 左
		pad_.insert(map<KeyCode, bool>::value_type(KEY_LEFT, state.IsDPadLeftPressed()));
		// 低速
		pad_.insert(map<KeyCode, bool>::value_type(KEY_MODE_CHANGE, state.IsLeftShoulderPressed()));
		// ボム
		pad_.insert(map<KeyCode, bool>::value_type(KEY_BOMB, state.IsRightShoulderPressed()));
		// ショット
		pad_.insert(map<KeyCode, bool>::value_type(KEY_SHOT, state.IsAPressed()));
		// ポーズ
		pad_.insert(map<KeyCode, bool>::value_type(KEY_PAUSE, state.IsXPressed()));
	}

}

bool InputManager::Update()
{
	key_ = keyboard_->GetState();
	keyTracker_->Update(key_);
	state = gamePad->GetState(0);

	return true;
}

bool InputManager::IsPreesed(KeyCode argKeyCode)
{
	if (state.IsConnected())
	{
		return pad_.at(argKeyCode);
	}
	else
	{
		return key_.IsKeyDown(keyArray_.at(argKeyCode));
	}
}

bool InputManager::IsTouched(KeyCode argKeyCode)
{
	if (state.IsConnected())
	{
		return pad_.at(argKeyCode);
	}
	else
	{
		return keyTracker_->IsKeyPressed(keyArray_.at(argKeyCode));
	}
}

