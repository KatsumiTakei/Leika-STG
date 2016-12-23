#include "Pause.h"

#include "../Texture/Texture.h"

#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../Utility/GraphManager.h"
#include "../InputManager/InputManager.h"

#include "../Constant.h"

using namespace Constant;
using namespace DirectX;
using namespace DirectX::Colors;
using namespace DirectX::SimpleMath;

Pause::Pause(SceneManager *argSceneManager)
:is_pause_(false), select_(0),is_check_(false),is_back_(false),action_(0)
{
	graph_[0] = GraphManager::Instance()->FindIndexTexture(L"resultbord.png");
	graph_[1] = GraphManager::Instance()->FindIndexTexture(L"pausemenu.png");
	soundManager = SoundManager::Instance();
	sceneManager = argSceneManager;
	input = InputManager::Instance();
}

void Pause::ChackSelect()
{
	if (input->IsTouched(KEY_DOWN))
	{
		is_back_ = false;
		soundManager->PlaySounds("cursolmove.wav", SE);
	}
	if (input->IsTouched(KEY_UP))
	{
		is_back_ = true;
		soundManager->PlaySounds("cursolmove.wav", SE);
	}
	if (input->IsTouched(KEY_SHOT))
	{
		if (is_back_) 
		{// Yes.なら選択した行動(シーンに移動)
			if (select_ == StageBack) action_ = ActionBackRetry;
			if (select_ == TitleBack) action_ = ActionBackTitle;
			SoundManager::Instance()->PlaySounds("enter.wav", SE);
		}
		else 
		{// No.なら選択画面に戻る
			is_check_ = false;
			SoundManager::Instance()->PlaySounds("cancel.wav", SE);
		}
	}
}

bool Pause::Update()
{
	if (input->IsTouched(KEY_PAUSE))
	{// ポーズの切り替え

		if (!is_pause_) 
		{
			is_pause_ = true;
		}
	/*	else 
		{
			select_ = 0;
			is_pause_ = false;
			is_check_ = false;
			is_back_ = false;
			SoundManager::Instance()->PlaySounds("cancel.mp3");
		}*/
	}
	if (is_pause_) 
	{	 
		if (!is_check_)
		{// まだ何も選択されていなければ

			if (input->IsTouched(KEY_DOWN))
			{
				select_++;
				soundManager->PlaySounds("cursolmove.wav", SE);
			}
			else if (input->IsTouched(KEY_UP))
			{
				select_--;
				soundManager->PlaySounds("cursolmove.wav", SE);
			}
			else if (input->IsTouched(KEY_SHOT))
			{// カーソルのあっている選択肢を選択して確認画面へ
				if (select_ == GameBack) 
				{
					is_pause_ = false;
					SoundManager::Instance()->PlaySounds("cancel.wav", SE);
				}
				if (select_ == TitleBack || select_ == StageBack) 
				{
					is_check_ = true;
					SoundManager::Instance()->PlaySounds("enter.wav", SE);
				}
			}
			
			if (select_ > StageBack)
			{
				select_ = GameBack;
			}
			else if (select_ < GameBack)
			{
				select_ = StageBack;
			}
		}
		else 
		{// 選択されている場合
			ChackSelect();
		}
	}
	if (action_ == ActionBackTitle)
	{
		sceneManager->changeScene(SCENE_TITLE);
	}
	if (action_ == ActionBackRetry)
	{
		sceneManager->changeScene(SCENE_PLAY);
	}
	return true;
}

void Pause::Draw()
{
	if (is_pause_)
	{
		// 下敷き
		DrawGraph((float)PLAYSCREEN_LEFT, (float)PLAYSCREEN_TOP, graph_[0]);

		Color select1 = Color(0.f, 0.f, 0.f, 0.5f);
		Color select2 = Color(0.f, 0.f, 0.f, 0.5f);
		Color select3 = Color(0.f, 0.f, 0.f, 0.5f);
		
		if (select_ == GameBack)
		{
			select1 = Color(1.f, 1.f, 1.f, 1.f);
			select2 = Color(0.f, 0.f, 0.f, 0.5f);
			select3 = Color(0.f, 0.f, 0.f, 0.5f);
		}
		else if (select_ == TitleBack)
		{
			select1 = Color(0.f, 0.f, 0.f, 0.5f);
			select2 = Color(1.f, 1.f, 1.f, 1.f);
			select3 = Color(0.f, 0.f, 0.f, 0.5f);
		}
		else if (select_ == StageBack)
		{
			select1 = Color(0.f, 0.f, 0.f, 0.5f);
			select2 = Color(0.f, 0.f, 0.f, 0.5f);
			select3 = Color(1.f, 1.f, 1.f, 1.f);
		}

		DrawRectGraph(100, 300, 0, 0, 160, 22, graph_[1], select1);
		DrawRectGraph(100, 330, 0, 24, 160, 20, graph_[1], select2);
		DrawRectGraph(100, 360, 0, 48, 160, 20, graph_[1], select3);

		if (is_check_)
		{
			// 確認のお言葉
			DrawRectGraph(300, 300, 0, 68, 100, 24, graph_[1]);

			// Yes or Noの下敷き(黒塗り)
			Color colorYes = Color(0.f, 0.f, 0.f, 0.5f);
			Color colorNo = Color(0.f, 0.f, 0.f, 0.5f);

			if (is_back_)
			{
				colorYes = Color(1.f, 1.f, 1.f, 1.f);
				colorNo = Color(0.f, 0.f, 0.f, 0.5f);
			}
			else if (!is_back_)
			{
				colorYes = Color(0.f, 0.f, 0.f, 0.5f);
				colorNo = Color(1.f, 1.f, 1.f, 1.f);
			}

			DrawRectGraph(300, 330, 0, 92, 100, 22, graph_[1], colorYes);
			DrawRectGraph(300, 360, 0, 114, 100, 22, graph_[1], colorNo);
		}
	}
}