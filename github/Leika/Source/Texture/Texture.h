#pragma once

#include "../Stdafx/stdafx.h"

class Texture
{
public:
	// �e�N�X�`���n���h��
	ID3D11ShaderResourceView* texture_;

	// �R���X�g���N�^
	Texture(wchar_t* argFilename);
	// �f�X�g���N�^
	~Texture();
};