#pragma once
//--------------------------------------
// 形状の種類
//--------------------------------------
enum SHAPE_TYPE
{
	SHAPE_CIRCLE,
	SHAPE_RECT,
	SHAPE_TYPE_NUM,
};
//--------------------------------------
// 形状の基底クラス
//--------------------------------------
class Shape
{
public:
	virtual ~Shape() {};
	virtual SHAPE_TYPE GetType() const = 0;
};
//--------------------------------------
// 円形状クラス
//--------------------------------------
class CircleShape : public Shape
{
private:
	float posX_, posY_;
	float radius_;
public:
	CircleShape(float argPosX, float argPosY, float argRadius) :
		posX_(argPosX), posY_(argPosY), radius_(argRadius){}

	virtual ~CircleShape(){}
	SHAPE_TYPE GetType()const { return SHAPE_CIRCLE; }
	float GetX()const { return posX_; }
	float GetY()const { return posY_; }
	float GetRadius()const { return radius_; }
};
//--------------------------------------
// 矩形形状クラス
//--------------------------------------
class RectShape : public Shape
{
private:
	float left_, right_, bottom_, top_;
public:
	RectShape(float argLeft, float argRight, float argBottom,float argTop) :
		left_(argLeft),right_(argRight), bottom_(argBottom), top_(argTop) {}

	virtual ~RectShape() {}
	SHAPE_TYPE GetType()const { return SHAPE_RECT; }
	float GetLeft()const { return left_; }
	float GetRight()const { return right_; }
	float GetBottom()const { return bottom_; }
	float GetTop()const { return top_; }
};

