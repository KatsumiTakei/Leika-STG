#include "PlayScene.h"

#include "../Player/Player.h"
#include "../Stage.h"
#include "../Pause/Pause.h"
#include "../Replay/Replay.h"

#include "../UI/UI.h"
#include "../Utility/FPS.h"
#include "../Utility/GraphManager.h"
#include "../Utility/TextManager.h"
#include "../Utility/TaskManager.h"
#include"../Sound/SoundManager.h"
#include "../InputManager/InputManager.h"
#include "../Control/Control.h"

#include "../Constant.h"
#include "../Utility/Debug.h"

using namespace Constant;
using namespace DirectX::Colors;
using namespace DirectX::SimpleMath;

PlayScene::PlayScene(SceneManager* SceneManager)
: Scene(SceneManager),select_(0)
{
	GraphManager *graphManager = GraphManager::Instance();

	fps = new Fps();
	input = InputManager::Instance();
	player = new Player(StartPos, StartSpd, 1.);
	stage = new Stage();
	pause = new Pause(SceneManager);
	replay = new RePlay(ReplayReproduce);

	graph_[0] = graphManager->FindIndexTexture(L"resultbord.png");
	graph_[1] = graphManager->FindIndexTexture(L"pausemenu.png");
}

PlayScene::~PlayScene()
{
}

#pragma region public_function

void PlayScene::initialize()
{
	SoundManager::Instance()->PlaySounds("stage01bgm.wav", LOOP);
	// テキストマネージャーの初期化
	TextManager::Instance()->Initialize();
	TaskManager *taskManager = TaskManager::Instance();
	{// タスクの登録
		taskManager->Add(stage);
		taskManager->Add(player);
	}
	TaskManager::Instance()->Add(new UI(player));

}

bool PlayScene::update()
{
	if ((player->getIsLiving()))
	{// プレイヤーが生きている時

		if (!pause->GetIsPause())
		{// ポーズしてなかったらら
			TaskManager::Instance()->Update();
			Control::Instance()->Update();

			if (replay->isUpdate())
			{
				replay->Update();
			}
#if defined(_DEBUG)
			//if (input->IsTouched(KEY_DEBUG))
			//{
			//	replay->Initialize(this);
			//	replay->Init(stage->getCount(), player->getPower(), player->getPos(), player->getSpd());
			//}
#endif
		}
		pause->Update();
	}
	else
	{// プレイヤーが死んだら
		if (input->IsTouched(KEY_DOWN))
		{
			select_--;
			SoundManager::Instance()->PlaySounds("cursolmove.wav", SE);
		}
		else if (input->IsTouched(KEY_UP))
		{
			select_++;
			SoundManager::Instance()->PlaySounds("cursolmove.wav", SE);
		}
		else if (input->IsTouched(KEY_SHOT))
		{
			if (select_ == GameBack) SceneManager_->changeScene(SCENE_PLAY);
			if (select_ == TitleBack) SceneManager_->changeScene(SCENE_TITLE);
		}

		if (select_ < GameBack)
		{
			select_ = TitleBack;
		}
		else if (select_ > TitleBack)
		{
			select_ = GameBack;
		}
		SoundManager::Instance()->StopSounds("stage01bgm.wav");
	}

	if ((Control::Instance()->isClear()))
	{// ステージクリアしたら次のマップをロードする
		Control::Instance()->isClear(false);
	}
	
	fps->Update();
	fps->Wait();

	return true;
}

void PlayScene::draw()
{
	TaskManager::Instance()->Draw();
	TextManager::Instance()->Draw();

	pause->Draw();

#if defined(_DEBUG)
	replay->Draw();
#endif
	fps->Draw();

	if (!player->getIsLiving())
	{
		// 下敷き
		DrawGraph((float)PLAYSCREEN_LEFT, (float)PLAYSCREEN_TOP, graph_[0]);
		
		// 選択肢の下敷き(黒塗り)
		Color select1 = Color(0.f, 0.f, 0.f, 0.5f);
		Color select2 = Color(0.f, 0.f, 0.f, 0.5f);
		if (select_ == TitleBack)
		{
			select1 = Color(1.f, 1.f, 1.f, 1.f);
			select2 = Color(0.f, 0.f, 0.f, 0.5f);
		}
		else if(select_ == GameBack)
		{
			select1 = Color(0.f, 0.f, 0.f, 0.5f);
			select2 = Color(1.f, 1.f, 1.f, 1.f);
		}
		
		DrawRectGraph(250, 330, 0, 24, 160, 20, graph_[1], select1);
		DrawRectGraph(250, 360, 0, 48, 160, 20, graph_[1], select2);

	}
}

void PlayScene::finalize()
{
	SoundManager::Instance()->StopSounds("stage01bgm.wav");
	TaskManager::Instance()->Clear();
	Control::Instance()->Finalize();
	delete fps;
	delete player;
	delete pause;
	delete stage;

	delete replay;
}
#pragma endregion