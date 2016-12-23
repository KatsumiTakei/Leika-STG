#pragma once

#include "../../Stdafx/stdafx.h"

class PaintBrush
{
public:
	PaintBrush();
	virtual ~PaintBrush();

	using FAPtr = ID2D1Factory*;
	using RTPtr = ID2D1RenderTarget*;

	void Draw(RTPtr rt);

protected:
	virtual void Render() = 0;

	// ラウンド半径 設定
	void SetRound(float width, float height);

	// ラウンド半径取得
	void SetRound(const D2D1_SIZE_F& size);

	// カラー設定
	void SetColor(const D2D1_COLOR_F& color);

	// サブカラー設定
	void SetColorB(const D2D1_COLOR_F& color);

	// 影カラー設定
	void SetShadowColor(const D2D1_COLOR_F color);

	// 線の太さ設定
	void SetLineWidth(float width);

	// ラウンド半径取得
	const D2D1_SIZE_F& GetRound() const;

	// カラー取得
	const D2D1_COLOR_F& GetColor() const;

	// サブカラー取得
	const D2D1_COLOR_F& GetColorB() const;

	// 線幅取得
	float GetLineWidth() const;

	// ドロップシャドウの有効設定
	void SetShadowEnable(bool isShadow);

	// ドロップシャドウの有効設定取得
	bool GetShadowEnable() const;

	// ドロップシャドウのオフセット設定
	void SetShadowOffset(float x, float y);

	// ドロップシャドウのオフセット取得
	const D2D_SIZE_F& GetShadowOffset() const;

	// 円弧の開始角度設定
	void SetArcStartAngle(float angle);

	// 円弧の開始角度取得
	float GetArcStartAngle() const;

	FAPtr GetFactory();

	RTPtr GetRenderTarget();
	
	// 度数→弧度変換
	float ToRadian(float degree);

	// 弧度→度数変換
	float ToDegree(float radian);

	// 回転移動行列
	D2D1_MATRIX_3X2_F ToTransform(float angle, float cx, float cy, float x, float y);

	// 円弧描画（センター指定）
	void DrawArc(float cx, float cy, float radius, float startAngle, float endAngle);

	// 円弧描画（サイズ指定）
	void DrawArc(float x, float y, float width, float height, float startAngle, float endAngle);

	//  円弧塗りつぶし（センター指定）
	void FillArc(float cx, float cy, float radius, float startAngle, float endAngle);

	// 円弧塗りつぶし（サイズ指定）
	void FillArc(float x, float y, float width, float height, float startAngle, float endAngle);

	// チェッカー描画
	void DrawChecker(float left, float top, int num_x, int num_y, float width, float height);

	// 角丸矩形描画
	void DrawRoundRect(float top, float left, float width, float height);

	// 角丸矩形塗りつぶし
	void FillRoundRect(float top, float left, float width, float height);

	// 矩形描画
	//  適応：影、太さ、メインカラー
	void DrawRect(float left, float top, float width, float height);

	// 矩形塗りつぶし
	//  適応：影、太さ、メインカラー
	void FillRect(float left, float top, float width, float height);

	// 円描画
	void DrawCircle(float cx, float cy, float radius);

	// 楕円描画
	void DrawEllipse(float left, float top, float width, float height);

	// 円塗りつぶし
	void FillCircle(float cx, float cy, float radius);

	// 楕円塗りつぶし
	void FillEllipse(float left, float top, float width, float height);

	// 直線描画
	void DrawLine(float sx, float sy, float ex, float ey);

	/////////////////////////////////////////
	/// @brief 開いた連続ライン
	/////////////////////////////////////////
	void DrawLinesOpen(D2D1_POINT_2F *points, int num);

	/////////////////////////////////////////
	/// @brief 閉じた連続ライン
	/////////////////////////////////////////
	void DrawLinesClosed(D2D1_POINT_2F *points, int num);

	/////////////////////////////////////////
	/// @brief 閉じた連続ライン塗りつぶし
	/////////////////////////////////////////
	void FillLinesClosed(D2D1_POINT_2F *points, int num);

	/////////////////////////////////////////
	/// @brief カーブライン
	/////////////////////////////////////////
	void DrawCurve(D2D1_POINT_2F *points, int num);

	/////////////////////////////////////////
	/// @brief カーブ塗りつぶし
	/////////////////////////////////////////
	void FillCurve(D2D1_POINT_2F *points, int num);

private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};