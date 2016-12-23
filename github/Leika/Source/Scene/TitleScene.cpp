#include "TitleScene.h"

#include "../Window/Window.h"
#include "../Utility/GraphManager.h"
#include "../Sound/SoundManager.h"
#include "../Utility/TaskManager.h"
#include "../InputManager/InputManager.h"

#include "../Utility/Debug.h"
#include "../Constant.h"
#include "../Replay/Replay.h"

namespace
{
	const int Play = 0;
	const int Quit = 1;
	const int DEMO_TIME = 3000;

}

using namespace Constant;
using namespace DirectX;
using namespace DirectX::SimpleMath;

TitleScene::TitleScene(SceneManager* SceneManager)
	: Scene(SceneManager), cnt_(0)
{
	GraphManager *graphManager = GraphManager::Instance();
	
	replay = new RePlay(ReplayRecode);
	replay->Initialize();

	graph[IMAGE_BACK] = graphManager->FindIndexTexture(L"titleback.png");
	graph[IMAGE_TEXT] = graphManager->FindIndexTexture(L"titletext.png");
	graph[IMAGE_TITLENAME] = graphManager->FindIndexTexture(L"titlename.png");
	graph[IMAGE_DEMOCALL] = graphManager->FindIndexTexture(L"democall.png");
	SoundManager::Instance()->PlaySounds("title.wav", LOOP);

}

void TitleScene::initialize()
{
}

bool TitleScene::update()
{
	InputManager *input = InputManager::Instance();
	static int time = 0;
	time++;

	if (time > DEMO_TIME) 
	{// ��莞�Ԃ������烊�v���C�J�n
		replay->Update();

		if(input->IsTouched(KEY_SHOT))
		{// �V���b�g�{�^���������ă��v���C��������~
			replay->Finalize();
		}
		if (!replay->isUpdate())
		{// ���v���C�I�����̏�����
			time = 0;
			replay->Initialize();
		}
	}
	else
	{// ���v���C�Đ�����Ă��Ȃ���΃^�C�g����ʂ̑��������

		if (input->IsTouched(KEY_DOWN))
		{
			time = 0;
			cnt_++;
			SoundManager::Instance()->PlaySounds("cursolmove.wav", SE);
		}
		else if (input->IsTouched(KEY_UP))
		{
			time = 0;
			cnt_--;
			SoundManager::Instance()->PlaySounds("cursolmove.wav", SE);
		}
		else if (input->IsTouched(KEY_SHOT))
		{// �J�[�\�����I�����Ă���I�����s��

			time = 0;
			SoundManager::Instance()->PlaySounds("enter.wav", SE);
			if (cnt_ == Play)
			{
				SceneManager_->changeScene(SCENE_PLAY);
			}
			else if (cnt_ == Quit)
			{
				PostMessage(Window::window()->windowHundle(), WM_CLOSE, 0, 0);
			}
		}

		if (cnt_ > Quit)
		{
			cnt_ = Play;
		}
		else if (cnt_ < Play)
		{
			cnt_ = Quit;
		}
	}

	return true;
}


void TitleScene::draw()
{
	if (!replay->isUpdate()) 
	{// �^�C�g����ʑ��쎞

		DrawGraph(0, 0, graph[IMAGE_BACK]);
		DrawGraph(100, 50, graph[IMAGE_TITLENAME]);
		Color select1 = Color(0.f, 0.f, 0.f, 0.5f);
		Color select2 = Color(0.f, 0.f, 0.f, 0.5f);
		if (cnt_ == Play)
		{
			select1 = Color(1.f, 1.f, 1.f, 1.f);
			select2 = Color(0.f, 0.f, 0.f, 0.5f);
		}
		else if (cnt_ == Quit)
		{
			select1 = Color(0.f, 0.f, 0.f, 0.5f);
			select2 = Color(1.f, 1.f, 1.f, 1.f);
		}
		DrawRectGraph(440, 350, 0, 0, 110, 24, graph[IMAGE_TEXT], select1);
		DrawRectGraph(440, 400, 0, 24, 48, 24, graph[IMAGE_TEXT], select2);

	}
	else 
	{// ���v���C�`��
		replay->Draw();

		static float add = 0;
		static bool addFlag = true;
		if (addFlag)	add += 0.05f;
		else	add -= 0.05f;
		if (add > 1.f || add < 0.f) addFlag = !addFlag;

		// ��ʂ̒����Ƀ��v���C�m�F����������
		const Color color = Color(1.f, 1.f, 1.f, add);
		DrawRectGraph(PLAYSCREEN_RIGHT / 2.f, PLAYSCREEN_BOTTOM / 2.f, 0, 0, 120, 40, graph[IMAGE_DEMOCALL], color);
	}

}


void TitleScene::finalize()
{
	SoundManager::Instance()->StopSounds("title.wav");
	TaskManager::Instance()->Clear();
	delete replay;
}
