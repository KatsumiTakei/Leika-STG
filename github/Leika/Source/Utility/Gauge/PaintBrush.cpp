#include "PaintBrush.h"

using namespace Microsoft::WRL;
using namespace D2D1;

class PaintBrush::Impl
{
public:
	Impl()
	{
		color_a = ColorF(ColorF::White);
		color_b = ColorF(ColorF::White);
		color_shadow = ColorF(ColorF::Gray, 0.5f);
		// RenderTarget�ˑ��̃u���V�́A�����ł͍쐬�ł��Ȃ�

		SetRound(0, 0);

		SetShadowEnable(false);
		SetShadowOffset(5, 5);

		SetLineWidth(1.0f);

		SetArcStartAngle(ToRadian(0.0f));
	}

	~Impl()
	{

	}

	void Begin(RTPtr rt)
	{
		this->rt = rt;
		// �t�@�N�g���̎擾
		ComPtr<ID2D1Factory> factory;
		rt->GetFactory(factory.GetAddressOf());
		this->fa = factory.Get();

		// �u���V�̍쐬
		SetColor(color_a);
		SetColorB(color_b);
		SetShadowColor(color_shadow);
	}

	void End()
	{
		rt->SetTransform(D2D1::IdentityMatrix());
	}


	// ���E���h���a �ݒ�
	void SetRound(float width, float height)
	{
		round = D2D1::SizeF(width, height);
	}

	// ���E���h���a�擾
	void SetRound(const D2D1_SIZE_F& size)
	{
		round = size;
	}

	// �J���[�ݒ�
	void SetColor(const D2D1_COLOR_F& color)
	{
		color_a = color;
		rt->CreateSolidColorBrush(color, brush_a.ReleaseAndGetAddressOf());
	}

	// �T�u�J���[�ݒ�
	void SetColorB(const D2D1_COLOR_F& color)
	{
		color_b = color;
		rt->CreateSolidColorBrush(color, brush_b.ReleaseAndGetAddressOf());
	}

	// �e�J���[�ݒ�
	void SetShadowColor(const D2D1_COLOR_F color)
	{
		color_shadow = color;
		rt->CreateSolidColorBrush(color, brush_shadow.ReleaseAndGetAddressOf());
	}

	// ���̑����ݒ�
	void SetLineWidth(float width)
	{
		line_width = width;
	}

	// ���E���h���a�擾
	const D2D1_SIZE_F& GetRound() const
	{
		return round;
	}

	// �J���[�擾
	const D2D1_COLOR_F& GetColor() const
	{
		return color_a;
	}

	// �T�u�J���[�擾
	const D2D1_COLOR_F& GetColorB() const
	{
		return color_b;
	}

	// �����擾
	float GetLineWidth() const
	{
		return 1.0f;
	}

	// �h���b�v�V���h�E�̗L���ݒ�
	void SetShadowEnable(bool isShadow)
	{
		this->is_shadow = isShadow;
	}

	// �h���b�v�V���h�E�̗L���ݒ�擾
	bool GetShadowEnable() const
	{
		return is_shadow;
	}

	// �h���b�v�V���h�E�̃I�t�Z�b�g�ݒ�
	void SetShadowOffset(float x, float y)
	{
		shadow_offset = D2D1::SizeF(x, y);
		shadow_matrix = D2D1::Matrix3x2F::Translation(shadow_offset);
	}

	// �h���b�v�V���h�E�̃I�t�Z�b�g�擾
	const D2D_SIZE_F& GetShadowOffset() const
	{
		return shadow_offset;
	}

	// �~�ʂ̊J�n�p�x�ݒ�
	void SetArcStartAngle(float angle)
	{
		arc_start_angle = angle;
	}

	// �~�ʂ̊J�n�p�x�擾
	float GetArcStartAngle() const
	{
		return arc_start_angle;
	}

	FAPtr GetFactory()
	{
		return fa;
	}

	RTPtr GetRenderTarget()
	{
		return rt;
	}

	FAPtr fa; // D2DFactory
	RTPtr rt; // D2DRenderTarget

	float line_width; // ���̑���

	D2D1_SIZE_F round; // ���E���h���a

	D2D1_COLOR_F color_a; // ���C���J���[
	D2D1_COLOR_F color_b; // �T�u�J���[
	D2D1_COLOR_F color_shadow; // �h���b�v�V���h�E�J���[

	ComPtr<ID2D1SolidColorBrush> brush_a; // ���C���u���V
	ComPtr<ID2D1SolidColorBrush> brush_b; // �T�u�u���V
	ComPtr<ID2D1SolidColorBrush> brush_shadow; // �h���b�v�V���h�E�u���V

	bool is_shadow; // �h���b�v�V���h�E�̏��

	D2D1_SIZE_F shadow_offset;       // �h���b�v�V���h�E�̃I�t�Z�b�g
	D2D1_MATRIX_3X2_F shadow_matrix; // �h���b�v�V���h�E�̕ϊ��s��

	float arc_start_angle;  // �~�ʂ̊J�n�p�x

	// �x�����ʓx�ϊ�
	float ToRadian(float degree)
	{
		return degree / 180.0f * float(M_PI);
	}

	// �ʓx���x���ϊ�
	float ToDegree(float radian)
	{
		return radian / float(M_PI) * 180.0f;
	}

	// ��]�ړ��s��
	D2D1_MATRIX_3X2_F ToTransform(float angle, float cx, float cy, float x, float y)
	{
		return D2D1::Matrix3x2F::Translation(-cx, -cy) * D2D1::Matrix3x2F::Rotation(ToDegree(angle)) * D2D1::Matrix3x2F::Translation(x, y);
	}

	// �`��
	void Render(RTPtr rt)
	{
		Begin(rt);


		// �`��J�n

		SetLineWidth(10);

		SetShadowOffset(5, 5);

		SetColor(ColorF(ColorF::LightGreen));
		SetColorB(ColorF(ColorF::LightBlue));
		DrawChecker(0, 0, 40, 23, 32, 32);

		SetShadowEnable(true);
		SetColor(ColorF(ColorF::Pink));
		FillArc(0.f, 0.f, 300.f, 300.f, 1.0f, -(float)M_PI);
		SetColor(ColorF(ColorF::LightPink));
		DrawArc(0.f, 0.f, 300.f, 300.f, 1.0f, -(float)M_PI);

		SetRound(20, 20);
		SetColor(ColorF(ColorF::Lime));
		FillRoundRect(100, 300, 200, 300);
		SetColor(ColorF(ColorF::LimeGreen));
		DrawRoundRect(100, 300, 200, 300);

		SetColor(ColorF(ColorF::LightCyan));
		FillRect(200, 200, 500, 200);
		SetColor(ColorF(ColorF::Cyan));
		DrawRect(200, 200, 500, 200);

		SetColor(ColorF(ColorF::Lavender));
		FillEllipse(500, 200, 100, 100);
		SetColor(ColorF(ColorF::LavenderBlush));
		DrawEllipse(500, 200, 100, 100);

		SetColor(ColorF(ColorF::Red));
		DrawLine(30, 60, 90, 120);

		{
			D2D1_POINT_2F points[] =
			{
				Point2F(400,300),
				Point2F(300,400),
				Point2F(200,100),
				Point2F(100,200),
				Point2F(10,50),
			};

			SetColor(ColorF(ColorF::DarkBlue));
			DrawLinesOpen(points, 5);
		}

		{

			D2D1_POINT_2F points[] =
			{
				Point2F(600,300),
				Point2F(600,400),
				Point2F(400,100),
				Point2F(300,200),
				Point2F(210, 50),
			};

			SetColor(ColorF(ColorF::DarkCyan));
			DrawLinesClosed(points, 5);
		}

		{
			D2D1_POINT_2F points[] =
			{
				Point2F(400,400),
				Point2F(300,600),
				Point2F(500,450),
				Point2F(300,450),
				Point2F(500,600),
			};
			SetColor(ColorF(ColorF::LightYellow));
			FillLinesClosed(points, 5);
			SetColor(ColorF(ColorF::Yellow));
			DrawLinesClosed(points, 5);
		}

		{
			D2D1_POINT_2F points[] =
			{
				Point2F(700,200),
				Point2F(1100,200),
				Point2F(800,600),
				Point2F(1000,600),
			};
			SetColor(ColorF(ColorF::GreenYellow));
			FillCurve(points, 4);
			SetColor(ColorF(ColorF::YellowGreen));
			DrawCurve(points, 4);
		}

		{
			D2D_POINT_2F points[] =
			{
				Point2F(320,100),
				Point2F(300,120),
				Point2F(100,120),
				Point2F(120,100),
			};
			D2D_POINT_2F points2[] =
			{
				Point2F(320,100),
				Point2F(300,120),
				Point2F(100,120),
				Point2F(120,100),
			};
			points2[0].x = 200;
			points2[1].x = 180;

			SetShadowOffset(3, 3);
			SetShadowEnable(true);
			SetColor(ColorF(ColorF::LightGray));
			FillLinesClosed(points, 4);
			SetShadowEnable(false);
			SetColor(ColorF(ColorF::LightGreen));
			FillLinesClosed(points2, 4);
			SetShadowEnable(true);
			SetLineWidth(5);
			SetColor(ColorF(ColorF::White));
			DrawLinesClosed(points, 4);
		}


		{
			SetShadowEnable(true);
			SetColor(ColorF(ColorF::LightGray));
			FillCircle(300, 300, 50);
			SetShadowEnable(false);
			SetColor(ColorF(ColorF::Red));
			FillArc(300, 300, 50, 0, ToRadian(270));
			SetColor(ColorF(ColorF::White));
			SetShadowEnable(true);
			FillCircle(300, 300, 30);
			SetShadowEnable(true);
			DrawCircle(300, 300, 50);
		}

		End();
	}


	// �e�`��ėp�֐�
	template<class T>
	void DrawShadow(T func)
	{
		if (!is_shadow) return;

		D2D1_MATRIX_3X2_F old_world;
		rt->GetTransform(&old_world);
		D2D1_MATRIX_3X2_F world = old_world * shadow_matrix;
		rt->SetTransform(world);
		func();
		rt->SetTransform(old_world);
	}

	// �~�ʕ`��i�Z���^�[�w��j
	void DrawArc(float cx, float cy, float radius, float startAngle, float endAngle)
	{
		DrawArc(cx - radius, cy - radius, radius * 2, radius * 2, startAngle, endAngle);
	}

	// �~�ʕ`��i�T�C�Y�w��j
	void DrawArc(float x, float y, float width, float height, float startAngle, float endAngle)
	{
		// ���a�����߂�
		float radius_x = width / 2.0f;
		float radius_y = height / 2.0f;

		startAngle += arc_start_angle;
		endAngle += arc_start_angle;

		// �p�x���
		float angle = endAngle - startAngle;
		float angle_abs = fabsf(angle);

		if (angle == 0.0f) return;

		if (angle_abs < M_PI * 2)
		{
			// 360�x�ȉ�

			// �J�n���W
			float sx = cosf(startAngle) * radius_x + x + radius_x;
			float sy = sinf(startAngle) * radius_y + y + radius_y;

			// �I�����W
			float ex = cosf(endAngle) * radius_x + x + radius_x;
			float ey = sinf(endAngle) * radius_y + y + radius_y;

			// �`�����
			auto dir = D2D1_SWEEP_DIRECTION_CLOCKWISE;
			if (angle < 0.0f) dir = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

			// 180�x���傫����
			auto arc_size = D2D1_ARC_SIZE_SMALL;
			if (angle_abs > float(M_PI)) arc_size = D2D1_ARC_SIZE_LARGE;


			// �p�X�W�I���g���쐬
			ComPtr<ID2D1PathGeometry> g;
			fa->CreatePathGeometry(g.GetAddressOf());

			// �V���N�쐬
			ComPtr<ID2D1GeometrySink> s;
			g->Open(s.GetAddressOf());

			// �`�󐶐��J�n
			s->BeginFigure(Point2F(sx, sy), D2D1_FIGURE_BEGIN_HOLLOW);

			//D2D1::ArcSegment(�I�_,���a,�ȉ~�̊p�x,���v���肩�A180���傫����)
			s->AddArc(
				D2D1::ArcSegment(
					Point2F(ex, ey),                 // �I�_
					SizeF(radius_x, radius_y),       // ���a
					0,                               // ��]�p�x
					dir,                             // ��]����
					arc_size));                      // 180���傫����

													 // �`�󐶐��I��
			s->EndFigure(D2D1_FIGURE_END_OPEN);

			// �V���N�N���[�Y
			s->Close();

			// �e�̕`��
			DrawShadow([this, g]() { rt->DrawGeometry(g.Get(), brush_shadow.Get(), line_width); });

			// �`��
			rt->DrawGeometry(g.Get(), brush_a.Get(), line_width);

		}
		else
		{
			// �ȉ~��`��
			rt->DrawEllipse(D2D1::Ellipse(Point2F(x + radius_x, y + radius_y), radius_x, radius_y), brush_a.Get(), 10);
		}
	}

	//  �~�ʓh��Ԃ��i�Z���^�[�w��j
	void FillArc(float cx, float cy, float radius, float startAngle, float endAngle)
	{
		FillArc(cx - radius, cy - radius, radius * 2, radius * 2, startAngle, endAngle);
	}

	// �~�ʓh��Ԃ��i�T�C�Y�w��j
	void FillArc(float x, float y, float width, float height, float startAngle, float endAngle)
	{
		// ���a�����߂�
		float radius_x = width / 2.0f;
		float radius_y = height / 2.0f;

		startAngle += arc_start_angle;
		endAngle += arc_start_angle;

		// �p�x���
		float angle = endAngle - startAngle;
		float angle_abs = fabsf(angle);

		if (angle == 0) return;

		if (angle_abs < M_PI * 2)
		{
			// ���S
			float cx = radius_x + x;
			float cy = radius_y + y;

			// �J�n���W
			float sx = cosf(startAngle) * radius_x + x + radius_x;
			float sy = sinf(startAngle) * radius_y + y + radius_y;

			// �I�����W
			float ex = cosf(endAngle) * radius_x + x + radius_x;
			float ey = sinf(endAngle) * radius_y + y + radius_y;

			// �p�X�W�I���g������
			ComPtr<ID2D1PathGeometry> g;
			fa->CreatePathGeometry(g.GetAddressOf());

			// �V���N�쐬
			ComPtr<ID2D1GeometrySink> s;
			g->Open(s.GetAddressOf());


			// �`�����
			auto dir = D2D1_SWEEP_DIRECTION_CLOCKWISE;
			if (angle < 0.0f) dir = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

			// ����
			auto arc_size = D2D1_ARC_SIZE_SMALL;
			if (fabsf(angle) > float(M_PI)) arc_size = D2D1_ARC_SIZE_LARGE;

			// �`�󐶐��J�n
			s->BeginFigure(Point2F(cx, cy), D2D1_FIGURE_BEGIN_FILLED);

			// ���S����A�O�a�ւ̒���
			s->AddLine(Point2F(sx, sy));

			// �~��
			//D2D1::ArcSegment(�I�_,���a,�ȉ~�̊p�x,���v���肩�A180���傫����)
			s->AddArc(
				D2D1::ArcSegment(
					Point2F(ex, ey),                 // �I�_
					SizeF(radius_x, radius_y),       // ���a
					0,                               // ��]�p�x
					dir,                             // ��]����
					arc_size));                      // 180���傫����

													 // �`�󐮌`�I��
			s->EndFigure(D2D1_FIGURE_END_CLOSED);

			// �V���N�N���[�Y
			s->Close();

			// �e�̕`��
			DrawShadow([this, &g]() {rt->FillGeometry(g.Get(), brush_shadow.Get()); });

			// �`��
			rt->FillGeometry(g.Get(), brush_a.Get());
		}
		else
		{
			// �`��
			rt->FillEllipse(D2D1::Ellipse(Point2F(x + radius_x, y + radius_y), radius_x, radius_y), brush_a.Get());
		}
	}


	// �`�F�b�J�[�`��
	void DrawChecker(float left, float top, int num_x, int num_y, float width, float height)
	{
		int num = num_x * num_y;
		for (int i = 0; i < num; i++)
		{
			float x = float(i % num_x)*width + left;
			float y = float(i / num_x)*height + top;

			auto rect = RectF(x, y, x + width, y + height);

			ID2D1SolidColorBrush *brush = nullptr;
			if ((i + ((i / num_x) % 2)) % 2 == 0) brush = brush_a.Get();
			else brush = brush_b.Get();

			// �e���Ή�

			rt->FillRectangle(rect, brush);
		}
	}

	// �p�ۋ�`�`��
	void DrawRoundRect(float top, float left, float width, float height)
	{
		auto rect = D2D1::RoundedRect(RectF(top, left, top + width, left + height), round.width, round.height);

		DrawShadow([this, &rect]() {rt->DrawRoundedRectangle(rect, brush_shadow.Get(), line_width); });
		rt->DrawRoundedRectangle(rect, brush_a.Get(), line_width);
	}

	// �p�ۋ�`�h��Ԃ�
	void FillRoundRect(float top, float left, float width, float height)
	{
		auto rect = D2D1::RoundedRect(RectF(top, left, top + width, left + height), round.width, round.height);

		DrawShadow([this, &rect]() {rt->FillRoundedRectangle(rect, brush_shadow.Get()); });
		rt->FillRoundedRectangle(rect, brush_a.Get());
	}

	// ��`�`��
	//  �K���F�e�A�����A���C���J���[
	void DrawRect(float left, float top, float width, float height)
	{
		auto rect = RectF(left, top, left + width, top + height);

		DrawShadow([this, &rect] {rt->DrawRectangle(rect, brush_shadow.Get(), line_width); });
		rt->DrawRectangle(rect, brush_a.Get(), line_width);
	}

	// ��`�h��Ԃ�
	//  �K���F�e�A�����A���C���J���[
	void FillRect(float left, float top, float width, float height)
	{
		auto rect = RectF(left, top, left + width, top + height);

		DrawShadow([this, &rect] {rt->FillRectangle(rect, brush_shadow.Get()); });
		rt->FillRectangle(rect, brush_a.Get());
	}

	// �~�`��
	void DrawCircle(float cx, float cy, float radius)
	{
		auto ellipse = D2D1::Ellipse(Point2F(cx, cy), radius, radius);

		DrawShadow([this, &ellipse] {rt->DrawEllipse(ellipse, brush_shadow.Get(), line_width); });
		rt->DrawEllipse(ellipse, brush_a.Get(), line_width);
	}

	// �ȉ~�`��
	void DrawEllipse(float left, float top, float width, float height)
	{
		float rx = width / 2;
		float ry = height / 2;

		float cx = left + rx;
		float cy = top + ry;

		auto ellipse = D2D1::Ellipse(Point2F(cx, cy), rx, ry);

		DrawShadow([this, &ellipse] {rt->DrawEllipse(ellipse, brush_shadow.Get(), line_width); });
		rt->DrawEllipse(ellipse, brush_a.Get(), line_width);
	}

	// �~�h��Ԃ�
	void FillCircle(float cx, float cy, float radius)
	{
		auto ellipse = D2D1::Ellipse(Point2F(cx, cy), radius, radius);

		DrawShadow([this, &ellipse] {rt->FillEllipse(ellipse, brush_shadow.Get()); });
		rt->FillEllipse(ellipse, brush_a.Get());
	}

	// �ȉ~�h��Ԃ�
	void FillEllipse(float left, float top, float width, float height)
	{
		float rx = width / 2;
		float ry = height / 2;

		float cx = left + rx;
		float cy = top + ry;

		auto ellipse = D2D1::Ellipse(Point2F(cx, cy), rx, ry);
		DrawShadow([this, &ellipse] {rt->FillEllipse(ellipse, brush_shadow.Get()); });
		rt->FillEllipse(ellipse, brush_a.Get());
	}

	// �����`��
	void DrawLine(float sx, float sy, float ex, float ey)
	{
		DrawShadow([&] {rt->DrawLine(Point2F(sx, sy), Point2F(ex, ey), brush_shadow.Get(), line_width); });
		rt->DrawLine(Point2F(sx, sy), Point2F(ex, ey), brush_a.Get(), line_width);
	}

	/////////////////////////////////////////
	/// @brief �J�����A�����C��
	/////////////////////////////////////////
	void DrawLinesOpen(D2D1_POINT_2F *points, int num)
	{
		// �p�X�W�I���g������
		ComPtr<ID2D1PathGeometry> g;
		fa->CreatePathGeometry(g.GetAddressOf());

		// �V���N�쐬
		ComPtr<ID2D1GeometrySink> s;
		g->Open(s.GetAddressOf());

		s->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);

		s->AddLines(&points[1], num - 1);

		s->EndFigure(D2D1_FIGURE_END_OPEN);

		s->Close();

		DrawShadow([this, &g] {rt->DrawGeometry(g.Get(), brush_shadow.Get(), line_width); });

		rt->DrawGeometry(g.Get(), brush_a.Get(), line_width);
	}

	/////////////////////////////////////////
	/// @brief �����A�����C��
	/////////////////////////////////////////
	void DrawLinesClosed(D2D1_POINT_2F *points, int num)
	{
		// �p�X�W�I���g������
		ComPtr<ID2D1PathGeometry> g;
		fa->CreatePathGeometry(g.GetAddressOf());

		// �V���N�쐬
		ComPtr<ID2D1GeometrySink> s;
		g->Open(s.GetAddressOf());

		s->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);

		s->AddLines(&points[1], num - 1);

		s->EndFigure(D2D1_FIGURE_END_CLOSED);

		s->Close();

		DrawShadow([this, &g] {rt->DrawGeometry(g.Get(), brush_shadow.Get(), line_width); });

		rt->DrawGeometry(g.Get(), brush_a.Get(), line_width);
	}


	/////////////////////////////////////////
	/// @brief �����A�����C���h��Ԃ�
	/////////////////////////////////////////
	void FillLinesClosed(D2D1_POINT_2F *points, int num)
	{
		//ComPtr<ID2D1SolidColorBrush> brush;
		//rt->CreateSolidColorBrush(ColorF(ColorF::Green), brush.GetAddressOf());

		// �p�X�W�I���g������
		ComPtr<ID2D1PathGeometry> g;
		fa->CreatePathGeometry(g.GetAddressOf());

		// �V���N�쐬
		ComPtr<ID2D1GeometrySink> s;
		g->Open(s.GetAddressOf());

		s->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);

		s->AddLines(&points[1], num - 1);

		s->EndFigure(D2D1_FIGURE_END_CLOSED);

		s->Close();

		DrawShadow([this, &g] {rt->FillGeometry(g.Get(), brush_shadow.Get()); });

		rt->FillGeometry(g.Get(), brush_a.Get());
	}

	/////////////////////////////////////////
	/// @brief �J�[�u���C��
	/////////////////////////////////////////
	void DrawCurve(D2D1_POINT_2F *points, int num)
	{
		D2D1_QUADRATIC_BEZIER_SEGMENT b[100];

		for (int i = 0; i < num + 1; i++)
		{
			int index = i%num;
			int next = (i + 1) % num;

			b[i].point1 = points[index];
			b[i].point2 = Point2F((points[index].x + points[next].x) / 2, (points[index].y + points[next].y) / 2);
		}

		// �p�X�W�I���g������
		ComPtr<ID2D1PathGeometry> g;
		fa->CreatePathGeometry(g.GetAddressOf());

		// �V���N�쐬
		ComPtr<ID2D1GeometrySink> s;
		g->Open(s.GetAddressOf());

		s->BeginFigure(b[0].point2, D2D1_FIGURE_BEGIN_HOLLOW);

		s->AddQuadraticBeziers(&b[1], num);

		s->EndFigure(D2D1_FIGURE_END_OPEN);

		s->Close();

		DrawShadow([this, &g] {rt->DrawGeometry(g.Get(), brush_shadow.Get(), line_width); });

		rt->DrawGeometry(g.Get(), brush_a.Get(), line_width);
	}

	/////////////////////////////////////////
	/// @brief �J�[�u�h��Ԃ�
	/////////////////////////////////////////
	void FillCurve(D2D1_POINT_2F *points, int num)
	{
		D2D1_QUADRATIC_BEZIER_SEGMENT b[100];

		for (int i = 0; i < num + 1; i++)
		{
			int index = i%num;
			int next = (i + 1) % num;

			b[i].point1 = points[index];
			b[i].point2 = Point2F((points[index].x + points[next].x) / 2, (points[index].y + points[next].y) / 2);
		}

		// �p�X�W�I���g������
		ComPtr<ID2D1PathGeometry> g;
		fa->CreatePathGeometry(g.GetAddressOf());

		// �V���N�쐬
		ComPtr<ID2D1GeometrySink> s;
		g->Open(s.GetAddressOf());

		s->BeginFigure(b[0].point2, D2D1_FIGURE_BEGIN_FILLED);

		s->AddQuadraticBeziers(&b[1], num);

		s->EndFigure(D2D1_FIGURE_END_OPEN);

		s->Close();

		DrawShadow([this, &g] {rt->FillGeometry(g.Get(), brush_shadow.Get()); });
		rt->FillGeometry(g.Get(), brush_a.Get());
	}
};



PaintBrush::PaintBrush() : pImpl(std::make_unique<Impl>())
{
}

PaintBrush::~PaintBrush()
{
}

void PaintBrush::Draw(RTPtr rt)
{
	pImpl->Begin(rt);
	Render();
	pImpl->End();
}

void PaintBrush::SetRound(float width, float height)
{
	pImpl->SetRound(width, height);
}

void PaintBrush::SetRound(const D2D1_SIZE_F & size)
{
	pImpl->SetRound(size);
}

void PaintBrush::SetColor(const D2D1_COLOR_F & color)
{
	pImpl->SetColor(color);
}

void PaintBrush::SetColorB(const D2D1_COLOR_F & color)
{
	pImpl->SetColorB(color);
}

void PaintBrush::SetShadowColor(const D2D1_COLOR_F color)
{
	pImpl->SetShadowColor(color);
}

void PaintBrush::SetLineWidth(float width)
{
	pImpl->SetLineWidth(width);
}

const D2D1_SIZE_F & PaintBrush::GetRound() const
{
	return pImpl->GetRound();
}

const D2D1_COLOR_F & PaintBrush::GetColor() const
{
	return pImpl->GetColor();
}

const D2D1_COLOR_F & PaintBrush::GetColorB() const
{
	return pImpl->GetColorB();
}

float PaintBrush::GetLineWidth() const
{
	return pImpl->GetLineWidth();
}

void PaintBrush::SetShadowEnable(bool isShadow)
{
	pImpl->SetShadowEnable(isShadow);
}

bool PaintBrush::GetShadowEnable() const
{
	return pImpl->GetShadowEnable();
}

void PaintBrush::SetShadowOffset(float x, float y)
{
	pImpl->SetShadowOffset(x, y);
}

const D2D_SIZE_F & PaintBrush::GetShadowOffset() const
{
	return pImpl->GetShadowOffset();
}

void PaintBrush::SetArcStartAngle(float angle)
{
	pImpl->SetArcStartAngle(angle);
}

float PaintBrush::GetArcStartAngle() const
{
	return pImpl->GetArcStartAngle();
}

PaintBrush::FAPtr PaintBrush::GetFactory()
{
	return pImpl->GetFactory();
}

PaintBrush::RTPtr PaintBrush::GetRenderTarget()
{
	return pImpl->GetRenderTarget();
}

float PaintBrush::ToRadian(float degree)
{
	return pImpl->ToRadian(degree);
}

float PaintBrush::ToDegree(float radian)
{
	return pImpl->ToDegree(radian);
}

D2D1_MATRIX_3X2_F PaintBrush::ToTransform(float angle, float cx, float cy, float x, float y)
{
	return pImpl->ToTransform(angle,cx,cy,x,y);
}

void PaintBrush::DrawArc(float cx, float cy, float radius, float startAngle, float endAngle)
{
	pImpl->DrawArc(cx, cy, radius, startAngle, endAngle);
}

void PaintBrush::DrawArc(float x, float y, float width, float height, float startAngle, float endAngle)
{
	pImpl->DrawArc(x, y, width, height, startAngle, endAngle);
}

void PaintBrush::FillArc(float cx, float cy, float radius, float startAngle, float endAngle)
{
	pImpl->FillArc(cx, cy, radius, startAngle, endAngle);
}

void PaintBrush::FillArc(float x, float y, float width, float height, float startAngle, float endAngle)
{
	pImpl->FillArc(x, y, width, height, startAngle, endAngle);
}

void PaintBrush::DrawChecker(float left, float top, int num_x, int num_y, float width, float height)
{
	pImpl->DrawChecker(left, top, num_x, num_y, width, height);
}

void PaintBrush::DrawRoundRect(float top, float left, float width, float height)
{
	pImpl->DrawRoundRect(top, left, width, height);
}

void PaintBrush::FillRoundRect(float top, float left, float width, float height)
{
	pImpl->FillRoundRect(top, left, width, height);
}

void PaintBrush::DrawRect(float left, float top, float width, float height)
{
	pImpl->DrawRect(left, top, width, height);
}

void PaintBrush::FillRect(float left, float top, float width, float height)
{
	pImpl->FillRect(left, top, width, height);
}

void PaintBrush::DrawCircle(float cx, float cy, float radius)
{
	pImpl->DrawCircle(cx, cy, radius);
}

void PaintBrush::DrawEllipse(float left, float top, float width, float height)
{
	pImpl->DrawEllipse(left, top, width, height);
}

void PaintBrush::FillCircle(float cx, float cy, float radius)
{
	pImpl->FillCircle(cx, cy, radius);
}

void PaintBrush::FillEllipse(float left, float top, float width, float height)
{
	pImpl->FillEllipse(left, top, width, height);
}

void PaintBrush::DrawLine(float sx, float sy, float ex, float ey)
{
	pImpl->DrawLine(sx, sy, ex, ey);
}

void PaintBrush::DrawLinesOpen(D2D1_POINT_2F * points, int num)
{
	pImpl->DrawLinesOpen(points, num);
}

void PaintBrush::DrawLinesClosed(D2D1_POINT_2F * points, int num)
{
	pImpl->DrawLinesClosed(points, num);
}

void PaintBrush::FillLinesClosed(D2D1_POINT_2F * points, int num)
{
	pImpl->FillLinesClosed(points, num);
}

void PaintBrush::DrawCurve(D2D1_POINT_2F * points, int num)
{
	pImpl->DrawCurve(points, num);
}

void PaintBrush::FillCurve(D2D1_POINT_2F * points, int num)
{
	pImpl->FillCurve(points, num);
}
