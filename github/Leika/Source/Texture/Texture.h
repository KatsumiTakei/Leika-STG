#pragma once

#include "../Stdafx/stdafx.h"

class Texture
{
public:
	// テクスチャハンドル
	ID3D11ShaderResourceView* texture_;

	// コンストラクタ
	Texture(wchar_t* argFilename);
	// デストラクタ
	~Texture();
};