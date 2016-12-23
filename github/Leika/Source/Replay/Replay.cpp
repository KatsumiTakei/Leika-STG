#include "Replay.h"
#include "../Constant.h"

#include "../Player/Player.h"
#include "../Stage.h"
#include "../UI/UI.h"

#include "../Control/Control.h"
#include "../Utility//GraphManager.h"
#include "../Utility//TaskManager.h"
#include "../InputManager/InputManager.h"

namespace
{
	const int RECTIME = 20;

	const bool MODE_REC = false;
	const bool MODE_REPLAY = true;

}

using namespace std;
using namespace Constant;
using namespace DirectX::SimpleMath;

RePlay::RePlay(int mode)
: isUpdate_(false),mode_(mode)
{
}

RePlay::~RePlay()
{
	if (player)	delete player;
	if (ui) delete ui;
	if (stage) delete stage;
}

void RePlay::Init(int argCount, double argPower,Vector2 argPos, Vector2 argSpd)
{
	RecData[0].x = argPos.x;
	RecData[0].y = argPos.y;
	RecData[0].spdX = argSpd.x;
	RecData[0].spdY = argSpd.y;
	RecData[0].count = argCount;
	RecData[0].power = argPower;
	isUpdate_ = true;

}

void RePlay::Initialize()
{
	if (mode_ == MODE_REC)
	{// 動作保存時の初期化処理
		// 秒間６０フレームの計算でデータ領域を確保
		RecData = (LPRECDATA)malloc(sizeof(RECDATA) * 60 * RECTIME);

		// 保存データ数の初期化
		recFrame_ = 0;
	}
	else
	{// 動作再生時の初期化処理

		FILE *fp;
		errno_t err;
		err = fopen_s(&fp, "MoveRec.dat", "rb");

		assert(fp);

		fread(&recFrame_, sizeof(int), 1, fp);

		// 保存されているフレーム数分の動作保存データを読みこめるだけのメモリ領域を確保
		RecData = (LPRECDATA)malloc(sizeof(RECDATA) * recFrame_);

		fread(RecData, sizeof(RECDATA), recFrame_, fp);

		
		player = new Player(Vector2(static_cast<float>(RecData[1].x), static_cast<float>(RecData[1].y)), Vector2(RecData[0].spdX, RecData[0].spdY), RecData[0].power);

		stage = new Stage();
		stage->setCount(RecData[0].count);

		UI *ui = new UI(player);

		TaskManager::Instance()->Add(stage);
		TaskManager::Instance()->Add(ui);

		// 再生が終ったフレーム数を初期化
		rePlayFrame_ = 0;

		fclose(fp);
	}
}

void RePlay::Update()
{
	isUpdate_ = true;

	// リプレイの速度を適正にするために
	// 経過時間を使う
	time = clock();

	if (mode_ == MODE_REC)
	{// 動作保存モードの処理
		// キーの入力状態を得る
		key_ = InputManager::Instance()->keyState();
	}
	else
	{// 動作再生時の処理
		key_ = RecData[rePlayFrame_].InputKey;
	}

	// 自機を移動させる
	if (key_.IsKeyDown(InputManager::Instance()->keyArray_.at(KEY_LEFT))) RecData->x -= RecData->spdX;
	if (key_.IsKeyDown(InputManager::Instance()->keyArray_.at(KEY_RIGHT))) RecData->x += RecData->spdX;
	if (key_.IsKeyDown(InputManager::Instance()->keyArray_.at(KEY_UP))) RecData->y -= RecData->spdY;
	if (key_.IsKeyDown(InputManager::Instance()->keyArray_.at(KEY_DOWN))) RecData->y += RecData->spdY;

	// 画面外に出た場合の補正
	if (RecData->x + 10 > PLAYSCREEN_RIGHT) RecData->x -= RecData->spdX;
	if (RecData->x - 10 < PLAYSCREEN_LEFT) RecData->x += RecData->spdX;
	if (RecData->y + 20 > PLAYSCREEN_BOTTOM) RecData->y -= RecData->spdY;
	if (RecData->y - 20 < PLAYSCREEN_TOP) RecData->y += RecData->spdY;

	// リプレイの間隔を適正にするための空ループ
	while (clock() - time < 10 * CLOCKS_PER_SEC / 1000);

	if (mode_ == MODE_REC)
	{// 動作保存モードの処理

		RecData[recFrame_].InputKey = key_;
		RecData[recFrame_].x = RecData->x;
		RecData[recFrame_].y = RecData->y;
		RecData[recFrame_].spdX = RecData->spdX;
		RecData[recFrame_].spdY = RecData->spdY;

		recFrame_++;

		if (recFrame_ == 60 * RECTIME)
		{
			Finalize();
		}
	}
	else
	{// 動作再生時の処理

		static int cnt = 0;

		if (key_.IsKeyDown(InputManager::Instance()->keyArray_.at(KEY_SHOT)))
		{
			if (++cnt % 4 == 0)
			{
				player->ShotBullet();
			}
		};

		player->setPos(Vector2(RecData[rePlayFrame_].x, RecData[rePlayFrame_].y));
		player->update();
		TaskManager::Instance()->Update();
		Control::Instance()->Update();

		rePlayFrame_++;

		if (rePlayFrame_ == recFrame_)
		{
			Finalize();
		}
	}
}

void RePlay::Draw()
{
	const Vector2 pos = Vector2();
	if (isUpdate_)
	{
		if (mode_ == MODE_REC)
		{
			const wstring str = L"動作を保存中です";
			GraphManager::Instance()->DrawTextWrite(str, pos);
		}
		else
		{
			TaskManager::Instance()->Draw();
			player->draw();

			const wstring str = L"動作を再生中です";
			GraphManager::Instance()->DrawTextWrite(str, pos);
		}
	}
	else 
	{
		if (mode_ == MODE_REC)
		{
			const wstring str = L"動作を終了しました";
			GraphManager::Instance()->DrawTextWrite(str, pos);
		}
		else
		{
			const wstring str = L"動作の再生が終了しました";
			GraphManager::Instance()->DrawTextWrite(str, pos);
		}
	}

}

void RePlay::Finalize()
{
	isUpdate_ = false;

	if (mode_ == MODE_REC)
	{// 動作保存モードの処理

		FILE *fp;
		errno_t err;
		err = fopen_s(&fp, "MoveRec.dat", "wb");
		
		assert(fp);

		// 最初に保存したフレーム数を書き出す
		fwrite(&recFrame_, sizeof(int), 1, fp);
		// 次に保存した動作データを書き出す
		fwrite(RecData, sizeof(RECDATA), recFrame_, fp);

		fclose(fp);
	}
	else
	{// 動作再生時の処理

		TaskManager::Instance()->Clear();
		Control::Instance()->Finalize();
	}

	free(RecData);
}
