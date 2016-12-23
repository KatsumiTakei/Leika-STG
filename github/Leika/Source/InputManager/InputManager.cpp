#include "InputManager.h"

using namespace std;
using namespace DirectX;

std::unique_ptr<DirectX::Keyboard> InputManager::keyboard_(new Keyboard);
std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> InputManager::keyTracker_(new Keyboard::KeyboardStateTracker);

void InputManager::Start()
{
	{// �L�[�̏�����
		// ��
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_UP, keyboard_->Up));
		// ��
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_DOWN, keyboard_->Down));
		// �E
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_RIGHT, keyboard_->Right));
		// ��
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_LEFT, keyboard_->Left));
		// �V���b�g
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_SHOT, keyboard_->Z));
		// �{��
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_BOMB, keyboard_->X));
		// �|�[�Y
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_PAUSE, keyboard_->A));
		// �ᑬ
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_MODE_CHANGE, keyboard_->LeftShift));
		// �f�o�b�N�Ɏg���L�[
		keyArray_.insert(map<KeyCode, DirectX::Keyboard::Keys>::
			value_type(KEY_DEBUG, keyboard_->Space));
	}
	{// �p�b�h�̏�����

		gamePad.reset(new GamePad());

		// �p�b�h�ɐڑ�����l���̐ݒ�
		state = gamePad->GetState(0);
		
		// ��
		pad_.insert(map<KeyCode, bool>::value_type(KEY_UP, state.IsDPadUpPressed()));
		// ��
		pad_.insert(map<KeyCode, bool>::value_type(KEY_DOWN, state.IsDPadDownPressed()));
		// �E
		pad_.insert(map<KeyCode, bool>::value_type(KEY_RIGHT, state.IsDPadRightPressed()));
		// ��
		pad_.insert(map<KeyCode, bool>::value_type(KEY_LEFT, state.IsDPadLeftPressed()));
		// �ᑬ
		pad_.insert(map<KeyCode, bool>::value_type(KEY_MODE_CHANGE, state.IsLeftShoulderPressed()));
		// �{��
		pad_.insert(map<KeyCode, bool>::value_type(KEY_BOMB, state.IsRightShoulderPressed()));
		// �V���b�g
		pad_.insert(map<KeyCode, bool>::value_type(KEY_SHOT, state.IsAPressed()));
		// �|�[�Y
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

