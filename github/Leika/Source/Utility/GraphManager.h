#pragma once

/**
* @file   GraphManager.h
* @brief  Graphic ÇÃìoò^ÅAéÊìæ
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "../Stdafx/stdafx.h"
#include "../DirectX/DWriteBase.h"
#include "../DirectX/Direct2DBase.h"

class Texture;

class GraphManager
{
private:

	std::unique_ptr <DirectX::EffectFactory> factory_;
	std::unique_ptr <DirectX::CommonStates> state_;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;
	std::unique_ptr<DirectX::SpriteFont> spriteFont_;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader_;

	std::unique_ptr<Direct2DBase> direct2D;
	std::unique_ptr<DWriteBase> directWrite;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brushWhite;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;

	std::map<const wchar_t*, Texture*> image_;
	std::map < const wchar_t*, std::unique_ptr<DirectX::Model>> model_;
	
protected:
	GraphManager() {};
	~GraphManager() {};

public:

	static GraphManager *Instance() {
		static GraphManager instance;
		return &instance;
	}

	void Start();

#pragma region setter_and_getter
	void AddAndLoadImage(const wchar_t *filename);
	void AddAndLoadModel(const wchar_t *filename);
	void DrawTextWrite(std::wstring str, DirectX::SimpleMath::Vector2 pos)const;
	void ConfTextColor(D2D1::ColorF color);

	inline Texture* FindIndexTexture(const wchar_t *key)const { return image_.at(key); }
	inline DirectX::Model *FindIndexModel(const wchar_t *key)const { return  model_.at(key).get(); }

	inline ID2D1RenderTarget *renderTarget()const { return direct2D->GetRenderTarget(); }
	

	inline DirectX::EffectFactory * factory()const { return factory_.get(); }
	inline DirectX::CommonStates *state()const { return state_.get(); }
	inline DirectX::SpriteBatch *spriteBatch()const { return spriteBatch_.get(); }
	inline DirectX::SpriteFont *spriteFont()const { return spriteFont_.get(); }
	inline ID3D11PixelShader *pixelShader()const { return pixelShader_.Get(); }
#pragma endregion
};

void DrawGraph(float argPosX, float argPosY, Texture *argTexture);
void DrawGraph(float argPosX, float argPosY, float argWight, float argHeight, float argDepth, Texture *argTexture);

void DrawRectGraph(float argPosX, float argPosY, float argCutX, float argCutY, float argWight, float argHeight, Texture *argTexture);
void DrawRectGraph(float argPosX, float argPosY, float argCutX, float argCutY, float argWight, float argHeight, Texture *argTexture, DirectX::SimpleMath::Color argColor);

void DrawRotaGraph(float argPosX, float argPosY, float argWight, float argHeight, float argExRate, float argAngle, Texture *argTexture);
void DrawRotaGraph(float argPosX, float argPosY, float argWight, float argHeight, float argCenterX, float argCenterY, float argExRate, float argAngle, Texture *argTexture);
void DrawRotaGraph(float argPosX, float argPosY, float argWight, float argHeight, float argCenterX, float argCenterY, DirectX::SimpleMath::Vector2 argExRate, float argAngle, Texture *argTexture);
void DrawRotaGraph(float argPosX, float argPosY, float argWight, float argHeight, float argCenterX, float argCenterY, float argExRate, float argAngle, Texture *argTexture, DirectX::SimpleMath::Color argColor);

void DrawRectRotaGraph(float argPosX, float argPosY, float argCutX, float argCutY, float argWight, float argHeight, float argExRate, float argAngle, Texture *argTexture);
void DrawRectRotaGraph(float argPosX, float argPosY, float argCutX, float argCutY, float argWight, float argHeight, float argExRate, float argAngle, Texture *argTexture, DirectX::SimpleMath::Color argColor);
