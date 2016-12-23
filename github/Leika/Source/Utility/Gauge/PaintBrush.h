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

	// ���E���h���a �ݒ�
	void SetRound(float width, float height);

	// ���E���h���a�擾
	void SetRound(const D2D1_SIZE_F& size);

	// �J���[�ݒ�
	void SetColor(const D2D1_COLOR_F& color);

	// �T�u�J���[�ݒ�
	void SetColorB(const D2D1_COLOR_F& color);

	// �e�J���[�ݒ�
	void SetShadowColor(const D2D1_COLOR_F color);

	// ���̑����ݒ�
	void SetLineWidth(float width);

	// ���E���h���a�擾
	const D2D1_SIZE_F& GetRound() const;

	// �J���[�擾
	const D2D1_COLOR_F& GetColor() const;

	// �T�u�J���[�擾
	const D2D1_COLOR_F& GetColorB() const;

	// �����擾
	float GetLineWidth() const;

	// �h���b�v�V���h�E�̗L���ݒ�
	void SetShadowEnable(bool isShadow);

	// �h���b�v�V���h�E�̗L���ݒ�擾
	bool GetShadowEnable() const;

	// �h���b�v�V���h�E�̃I�t�Z�b�g�ݒ�
	void SetShadowOffset(float x, float y);

	// �h���b�v�V���h�E�̃I�t�Z�b�g�擾
	const D2D_SIZE_F& GetShadowOffset() const;

	// �~�ʂ̊J�n�p�x�ݒ�
	void SetArcStartAngle(float angle);

	// �~�ʂ̊J�n�p�x�擾
	float GetArcStartAngle() const;

	FAPtr GetFactory();

	RTPtr GetRenderTarget();
	
	// �x�����ʓx�ϊ�
	float ToRadian(float degree);

	// �ʓx���x���ϊ�
	float ToDegree(float radian);

	// ��]�ړ��s��
	D2D1_MATRIX_3X2_F ToTransform(float angle, float cx, float cy, float x, float y);

	// �~�ʕ`��i�Z���^�[�w��j
	void DrawArc(float cx, float cy, float radius, float startAngle, float endAngle);

	// �~�ʕ`��i�T�C�Y�w��j
	void DrawArc(float x, float y, float width, float height, float startAngle, float endAngle);

	//  �~�ʓh��Ԃ��i�Z���^�[�w��j
	void FillArc(float cx, float cy, float radius, float startAngle, float endAngle);

	// �~�ʓh��Ԃ��i�T�C�Y�w��j
	void FillArc(float x, float y, float width, float height, float startAngle, float endAngle);

	// �`�F�b�J�[�`��
	void DrawChecker(float left, float top, int num_x, int num_y, float width, float height);

	// �p�ۋ�`�`��
	void DrawRoundRect(float top, float left, float width, float height);

	// �p�ۋ�`�h��Ԃ�
	void FillRoundRect(float top, float left, float width, float height);

	// ��`�`��
	//  �K���F�e�A�����A���C���J���[
	void DrawRect(float left, float top, float width, float height);

	// ��`�h��Ԃ�
	//  �K���F�e�A�����A���C���J���[
	void FillRect(float left, float top, float width, float height);

	// �~�`��
	void DrawCircle(float cx, float cy, float radius);

	// �ȉ~�`��
	void DrawEllipse(float left, float top, float width, float height);

	// �~�h��Ԃ�
	void FillCircle(float cx, float cy, float radius);

	// �ȉ~�h��Ԃ�
	void FillEllipse(float left, float top, float width, float height);

	// �����`��
	void DrawLine(float sx, float sy, float ex, float ey);

	/////////////////////////////////////////
	/// @brief �J�����A�����C��
	/////////////////////////////////////////
	void DrawLinesOpen(D2D1_POINT_2F *points, int num);

	/////////////////////////////////////////
	/// @brief �����A�����C��
	/////////////////////////////////////////
	void DrawLinesClosed(D2D1_POINT_2F *points, int num);

	/////////////////////////////////////////
	/// @brief �����A�����C���h��Ԃ�
	/////////////////////////////////////////
	void FillLinesClosed(D2D1_POINT_2F *points, int num);

	/////////////////////////////////////////
	/// @brief �J�[�u���C��
	/////////////////////////////////////////
	void DrawCurve(D2D1_POINT_2F *points, int num);

	/////////////////////////////////////////
	/// @brief �J�[�u�h��Ԃ�
	/////////////////////////////////////////
	void FillCurve(D2D1_POINT_2F *points, int num);

private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};