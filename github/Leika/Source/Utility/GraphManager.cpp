#include "GraphManager.h"
#include "../Texture/Texture.h"
#include "../DirectX/Direct3D.h"
#include "../Constant.h"

#pragma warning(disable : 4996)

using namespace std;
using namespace Constant;
using namespace DirectX;
using namespace DirectX::Colors;
using namespace DirectX::SimpleMath;

void GraphManager::Start()
{
	Direct3D *direct3D = Direct3D::Instance();
	{// 描画ステートの設定
		factory_.reset(new EffectFactory(direct3D->pd3dDevice().Get()));
		factory_->SetDirectory(L"Resources/cModels");
		state_.reset(new CommonStates(direct3D->pd3dDevice().Get()));
		spriteBatch_.reset(new SpriteBatch(direct3D->context().Get()));
		spriteFont_.reset(new SpriteFont(direct3D->pd3dDevice().Get(), L"Resources/Fonts/HGgyousyo.spritefont"));
	}
	{// DirectWriteの設定
		direct2D.reset(new Direct2DBase);
		direct2D->Create(direct3D->swapChain().Get());
		directWrite.reset(new DWriteBase);
		directWrite->Create();

		ID2D1RenderTarget *renderTarget = direct2D->GetRenderTarget();
		IDWriteFactory *directWriteFactory = directWrite->GetFactory();
		/**
		 *	@brief	文字色のためのブラシ作成
		 *	@param	!<	色（1，1，1）
		 *	@param	!<	設定したいブラシ
		 */
		renderTarget->CreateSolidColorBrush(
			D2D1::ColorF(0, 0, 0),
			brushWhite.GetAddressOf());
		/**
		 *	@brief	フォントの作成
		 *	@param	!<	フォント名   ""の場合は、デフォルトのフォントが選ばれる
		 *	@param	!<	フォントファミリー  nullptrの場合は、システムフォントファミリーが使われる
		 *  @param	!<	文字の太さ
		 *	@param	!<	文字のスタイル
		 *	@param	!<	文字の伸縮
		 *	@param	!<	文字のサイズ
		 *　@param	!<	ロケール名
		 *　@param	!<	テキストフォーマット
		 */
		directWriteFactory->CreateTextFormat(
			L"",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.f,
			L"japanese",
			textFormat.GetAddressOf());
	}
	{// Shaderの設定
		vector<char>buffer;
		if (auto fs = fstream(L"Resources/Shader/stage01Shader.cso", ios::in | ios::binary))
		{
			fs.seekg(0, ios::end);
			size_t size = (size_t)fs.tellg();
			fs.seekg(0, ios::beg);

			buffer.resize(size);
			fs.read(buffer.data(), size);
		}
		direct3D->pd3dDevice()->CreatePixelShader(buffer.data(), buffer.size(), nullptr, pixelShader_.GetAddressOf());
	}
}

void GraphManager::AddAndLoadImage(const wchar_t *filename)
{
	wchar_t name[50] = L"Resources/Images/";
	wcscat(name, filename);

	Texture *texture = new Texture(name);
	image_.insert(map<const wchar_t*, Texture*>::value_type(filename, texture));
}

void GraphManager::AddAndLoadModel(const wchar_t *filename)
{
	wchar_t name[50] = L"Resources/cModels/";
	wcscat(name, filename);

	unique_ptr<Model> model = Model::CreateFromCMO(Direct3D::Instance()->pd3dDevice().Get(), name, *factory_);
	model_.insert(map<const wchar_t*, unique_ptr<Model>>::value_type(filename, move(model)));
}

void GraphManager::DrawTextWrite(wstring str, SimpleMath::Vector2 pos)const
{
	direct2D->GetRenderTarget()->DrawTextW(
		str.c_str(),
		str.length(),
		textFormat.Get(),
		D2D1::RectF(pos.x, pos.y, (float)SCREEN_RIGHT, (float)SCREEN_BOTTOM),
		brushWhite.Get());
}

void GraphManager::ConfTextColor(D2D1::ColorF color)
{
	direct2D->GetRenderTarget()->CreateSolidColorBrush(
		color,
		brushWhite.GetAddressOf());
}

void DrawGraph(float argPosX, float argPosY, Texture *argTexture)
{
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY));
}

void DrawGraph(float argPosX, float argPosY, float argWight, float argHeight, float argDepth, Texture *argTexture)
{
	RECT rect = { (LONG)0, (LONG)0, (LONG)argWight, (LONG)argHeight };
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY)
		, &rect, White, 0.f, Vector2(0, 0), 1.f, DirectX::SpriteEffects::SpriteEffects_None, argDepth);
}

void DrawRectGraph(float argPosX, float argPosY, float argCutX, float argCutY,
	float argWight, float argHeight, Texture *argTexture)
{
	RECT rect = { (LONG)argCutX, (LONG)argCutY , (LONG)(argCutX + argWight), (LONG)(argCutY + argHeight) };
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY), &rect
		, White, 0.f, Vector2(0, 0), 1.f);
}

// 色、アルファ指定アリ
void DrawRectGraph(float argPosX, float argPosY, float argCutX, float argCutY,
	float argWight, float argHeight, Texture *argTexture, Color argColor)
{
	RECT rect = { (LONG)argCutX, (LONG)argCutY , (LONG)(argCutX + argWight), (LONG)(argCutY + argHeight) };
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY)
		, &rect, argColor, 0.f, Vector2(0, 0), 1.f);
}

// 回転中心ナシ
void DrawRotaGraph(float argPosX, float argPosY, float argWight, float argHeight,
	float argExRate, float argAngle, Texture *argTexture)
{
	RECT rect = { (LONG)0, (LONG)0, (LONG)argWight, (LONG)argHeight };
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY)
		, &rect, White, argAngle, Vector2(0.f, 0.f), argExRate);
}

// 回転中心アリ
void DrawRotaGraph(float argPosX, float argPosY, float argWight, float argHeight,
	float argCenterX, float argCenterY, float argExRate, float argAngle, Texture *argTexture)
{
	RECT rect = { (LONG)0, (LONG)0, (LONG)argWight, (LONG)argHeight };
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY)
		, &rect, White, argAngle, Vector2(argCenterX, argCenterY), argExRate);
}

// 回転中心、縦横別の拡縮アリ
void DrawRotaGraph(float argPosX, float argPosY, float argWight, float argHeight,
	float argCenterX, float argCenterY, Vector2 argExRate, float argAngle, Texture *argTexture)
{
	RECT rect = { (LONG)0, (LONG)0, (LONG)argWight, (LONG)argHeight };
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY)
		, &rect, White, argAngle, Vector2(argCenterX, argCenterY), argExRate);
}

// 回転中心、色、アルファ指定アリ
void DrawRotaGraph(float argPosX, float argPosY, float argWight, float argHeight, float argCenterX,
	float argCenterY, float argExRate, float argAngle, Texture *argTexture, Color argColor)
{
	RECT rect = { (LONG)0, (LONG)0, (LONG)argWight, (LONG)argHeight };
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY)
		, &rect, argColor, argAngle, Vector2(argCenterX, argCenterY), argExRate);
}

// 
void DrawRectRotaGraph(float argPosX, float argPosY, float argCutX, float argCutY,
	float argWight, float argHeight, float argExRate, float argAngle, Texture *argTexture)
{
	RECT rect = { (LONG)argCutX, (LONG)argCutY, (LONG)(argCutX + argWight), (LONG)(argCutY + argHeight) };
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY)
		, &rect, White, argAngle, Vector2(argWight / 2.f, argHeight / 2.f), argExRate);
}

// 
void DrawRectRotaGraph(float argPosX, float argPosY, float argCutX, float argCutY,
	float argWight, float argHeight, float argExRate, float argAngle, Texture *argTexture, Color argColor)
{
	RECT rect = { (LONG)argCutX, (LONG)argCutY, (LONG)(argCutX + argWight), (LONG)(argCutY + argHeight) };
	GraphManager::Instance()->spriteBatch()->Draw(argTexture->texture_, Vector2(argPosX, argPosY)
		, &rect, argColor, argAngle, Vector2(argWight / 2.f, argHeight / 2.f), argExRate);
}