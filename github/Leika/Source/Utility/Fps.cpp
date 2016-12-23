#include "Fps.h"

#include "../Constant.h"
#include "../Function.h"

#include "../Utility/GraphManager.h"

using namespace Constant;
using namespace DirectX::SimpleMath;

namespace
{
	//平均を取るサンプル数
	const int AVERAGE = 60;
	//設定したFPS
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
	{ //1フレーム目なら時刻を記憶
		startTime_ = clock();
	}
	if (count_ == AVERAGE) 
	{ //60フレーム目なら平均を計算する
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
 *  @brief 指定された時間分だけ待機
 *  @param[in] waitTime - 待機時間[ミリ秒]
 */
void Fps::Wait()
{
	//かかった時間
	int tookTime = clock() - startTime_;
	//待つべき時間
	int waitTime = count_ * 1000 / FPS_FLEAM - tookTime;
	if (waitTime > 0)
	{// 待機時間をタイマー刻みの単位に変換
		clock_t waitClocks = waitTime * CLOCKS_PER_SEC / 1000;

		// 計測開始時間の取得
		clock_t start = clock();

		// 指定された時間まで待機させる
		while (clock() - start < waitClocks);
	}
}