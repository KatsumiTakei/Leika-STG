#include "Fps.h"

#include "../Constant.h"
#include "../Function.h"

#include "../Utility/GraphManager.h"

using namespace Constant;
using namespace DirectX::SimpleMath;

namespace
{
	//���ς����T���v����
	const int AVERAGE = 60;
	//�ݒ肵��FPS
	const int FPS_FLEAM = 60;
}

Fps::Fps()
:startTime_(0),count_(0),fps_(0)
{
	graph_ = GraphManager::Instance()->FindIndexTexture(L"number.png");
}

bool Fps::Update()
{
	if (count_ == 0) 
	{ //1�t���[���ڂȂ玞�����L��
		startTime_ = clock();
	}
	if (count_ == AVERAGE) 
	{ //60�t���[���ڂȂ畽�ς��v�Z����
		int t = clock();
		fps_ = 1000.f / ((t - startTime_) / (float)AVERAGE);
		count_ = 0;
		startTime_ = t;
	}
	count_++;
	return true;
}

void Fps::Draw()
{
	DrawNum(Vector2((float)SCREEN_RIGHT - 30.f, (float)SCREEN_BOTTOM - 30.f), Vector2(12.f, 24.f), fps_, graph_, 2);
}
/**
 *  @brief �w�肳�ꂽ���ԕ������ҋ@
 *  @param[in] waitTime - �ҋ@����[�~���b]
 */
void Fps::Wait()
{
	//������������
	int tookTime = clock() - startTime_;
	//�҂ׂ�����
	int waitTime = count_ * 1000 / FPS_FLEAM - tookTime;
	if (waitTime > 0)
	{// �ҋ@���Ԃ��^�C�}�[���݂̒P�ʂɕϊ�
		clock_t waitClocks = waitTime * CLOCKS_PER_SEC / 1000;

		// �v���J�n���Ԃ̎擾
		clock_t start = clock();

		// �w�肳�ꂽ���Ԃ܂őҋ@������
		while (clock() - start < waitClocks);
	}
}