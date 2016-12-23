#include "Texture.h"
#include "../DirectX/Direct3D.h"

using namespace DirectX;

Texture::Texture(wchar_t* argFilename)
{
	CreateWICTextureFromFile(Direct3D::Instance()->pd3dDevice().Get(), argFilename, nullptr, &texture_);
}

Texture::~Texture()
{
	texture_->Release();
}