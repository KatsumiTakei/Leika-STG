#include "UI.h"
#include "../Player/Player.h"

#include "../Control/Control.h"
#include "../Texture/Texture.h"
#include "../Utility/GraphManager.h"
#include "../Constant.h"
#include "../Function.h"

using namespace std;
using namespace Constant;
using namespace DirectX::SimpleMath;


UI::UI(Player * argPlayer)
{
	player = argPlayer;
	is_living_ = true;
	mode_ = DRAW_ALPHA;
	priority_ = PRIORITY_UI;
	graphManager = GraphManager::Instance();
	uiTexture[SIDES] = graphManager->FindIndexTexture(L"ui.png");
	uiTexture[FLEAM] = graphManager->FindIndexTexture(L"back.png");
	uiTexture[ROGO] = graphManager->FindIndexTexture(L"titlename.png");
	uiTexture[NUM] = graphManager->FindIndexTexture(L"number.png");
	uiTexture[GAUGE] = graphManager->FindIndexTexture(L"gauge.png");

}

void UI::initialize()
{
}

bool UI::update()
{
	return true;
}

void UI::draw()
{
	// 画面背景
	DrawGraph(0, 0, uiTexture[FLEAM]);
	//// タイトル
	//DrawRotaGraph(540, 380, 640, 480, 0, 0, 0.5, 0.f, graph_[ROGO]);

	const Vector2 hiScoreTextPos = Vector2((float)PLAYSCREEN_RIGHT + 6.f, (float)PLAYSCREEN_TOP + 18.f);
	const Vector2 currentScoreTextPos = Vector2(hiScoreTextPos.x,hiScoreTextPos.y + 30);
	static const Color black = Color(0.5f, 0.5f, 0.5f, 1.f);
	static const Color white = Color(1.f, 1.f, 1.f, 1.f);

	{// スコア
		const int DigitNum = 9;

		graphManager->DrawTextWrite(L"HiScore", hiScoreTextPos);
		graphManager->DrawTextWrite(L"Score", currentScoreTextPos);
		
		DrawNum(Vector2(hiScoreTextPos.x + 190, hiScoreTextPos.y), Vector2(12, 24), static_cast<float>(Control::Instance()->hiScore()), uiTexture[NUM], DigitNum, white);
		DrawNum(Vector2(currentScoreTextPos.x + 190, currentScoreTextPos.y), Vector2(12, 24), static_cast<float>(Control::Instance()->score()), uiTexture[NUM], DigitNum, white);
	}

	const Vector2 playerLifeMarkPos(hiScoreTextPos.x + 70.f, hiScoreTextPos.y + 75.f);

	{// ライフ
		const float LifeMarkSize = 21.f;
		graphManager->DrawTextWrite(L"Player", Vector2(playerLifeMarkPos.x - 70.f, playerLifeMarkPos.y - 5.f));
		
		for (int i = 0; i < 5; i++)
		{// 最大ライフ
			DrawRectGraph(playerLifeMarkPos.x + (LifeMarkSize * i) + 22.f, playerLifeMarkPos.y, 0, 0, LifeMarkSize, LifeMarkSize, uiTexture[SIDES], black);
			
			if (i < player->getLife())
			{// 現在のライフ
				DrawRectGraph(playerLifeMarkPos.x + (LifeMarkSize * i) + 22.f, playerLifeMarkPos.y, 0, 0, LifeMarkSize, LifeMarkSize, uiTexture[SIDES], white);
			}
		}
	}

	const Vector2 playerBombsMarkPos(playerLifeMarkPos.x, playerLifeMarkPos.y + 25.f);

	{// ボム

		graphManager->DrawTextWrite(L"Bomb", Vector2(playerBombsMarkPos.x - 70.f, playerBombsMarkPos.y - 5.f));
		const float BombMarkSize = 21.f;
		for (int i = 0; i < 5; i++)
		{// 最大ボム
			DrawRectGraph(playerBombsMarkPos.x + (BombMarkSize * i) + 22.f, playerBombsMarkPos.y, BombMarkSize, 0, BombMarkSize, BombMarkSize, uiTexture[SIDES], black);
			
			if (i < player->getBombsNum())
			{// 現在のボム
				DrawRectGraph(playerBombsMarkPos.x + (BombMarkSize * i) + 22.f, playerBombsMarkPos.y, BombMarkSize, 0, BombMarkSize, BombMarkSize, uiTexture[SIDES], white);
			}
		}
	}

	const Vector2 playerPowerTextPos = Vector2(playerBombsMarkPos.x - 70.f, playerBombsMarkPos.y + 45.f);
	{// パワー
		wchar_t powerStr[100];
		swprintf_s(powerStr, 100, L"Power 　　%.1f / 5.00", player->power());
		graphManager->DrawTextWrite(powerStr, playerPowerTextPos);
	}
}

void UI::finalize()
{
}