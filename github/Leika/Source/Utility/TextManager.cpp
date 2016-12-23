#include "TextManager.h"

#include "GraphManager.h"
#include "../InputManager/InputManager.h"
#include "../Utility/Debug.h"

using namespace std;
using namespace DirectX::SimpleMath;

void TextManager::textUpdate(const char str)
{
	// １文字分一時記憶配列
	char OneBuf[3];
	switch (str)
	{// 文字の設定

	case '@':
	{// 改行文字
		charP_++;
		break;
	}
	case 'B':
	{// ボタン押し待ち文字
		is_waiting_ = true;
		charP_++;
		break;
	}
	case 'E':
	{// 終了文字
		is_finished_ = true;
		charP_++;
		break;
	}
	case 'C':
	{// クリア文字
		drawtext_.clear();
		charP_++;
		break;
	}
	default:
	{// その他の文字
		if (count_ > 600)
		{
			is_waiting_ = false;
		}
		if (!is_waiting_)
		{// １文字分抜き出す
			OneBuf[0] = text[stringP_][charP_];
			OneBuf[1] = text[stringP_][charP_ + 1];
			OneBuf[2] = '\0';

			// 参照文字位置を２バイト勧める
			charP_ += 2;

			drawtext_ += OneBuf;
			count_ = 0;
		}
		break;
	}

	if (text[stringP_][charP_] == '\0')
	{// 参照文字列の終端まで行っていたら参照文字列を進める
		stringP_++;
		charP_ = 0;
	}

	}// ----switch終わり----
}

void TextManager::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		text[0][i] = 0;
	}
	drawtext_.clear();
	is_waiting_ = false;
	count_ = 0;
	stringP_ = 0;
	charP_ = 0; 
	drawPointX_ = 50.f; 
	drawPointY_ = 300.f;
	is_finished_ = true;
	ifstream fs("stage01text.txt");

	int begin = static_cast<int>(fs.tellg());
	fs.seekg(0, fs.end);
	// 一応範囲チェックすべきだけど…… 
	int end = static_cast<int>(fs.tellg());
	int size = end - begin;
	fs.clear();  // ここでclearしてEOFフラグを消す 
	fs.seekg(0, fs.beg);
	text[256][256] = '\0';  // 念のため末尾をNULL文字に 
	fs.read(*text, size);

	fs.close();
	graph_ = GraphManager::Instance()->FindIndexTexture(L"textscreen.png");
}

bool TextManager::Update()
{
	if (is_finished_) return false;
	const char OneStr = text[stringP_][charP_];

	if (InputManager::Instance()->IsTouched(KEY_SHOT))
	{
		is_waiting_ = false;
	}
	textUpdate(OneStr);

	count_++;

	return true;
}

void TextManager::Draw()
{
	if (!is_finished_)
	{
		GraphManager::Instance()->ConfTextColor(D2D1::ColorF(1, 1, 1));
		DrawGraph(drawPointX_, drawPointY_, graph_);

		size_t wLen = 0;
		wchar_t str[150];
		mbstowcs_s(&wLen, str, 150, drawtext_.c_str(), _TRUNCATE);

		GraphManager::Instance()->DrawTextWrite(str, Vector2(drawPointX_, drawPointY_));
		GraphManager::Instance()->ConfTextColor(D2D1::ColorF(0, 0, 0));

	}
}