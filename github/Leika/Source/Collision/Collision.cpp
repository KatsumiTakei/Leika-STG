#include "Collision.h"
#include "Shape.h"

//--------------------------------------
// 円と円の当たり判定
//--------------------------------------
bool CircleToCircle(const CircleShape& s1, const CircleShape& s2)
{
	bool rezult = false;

	float x1 = s1.GetX() + s1.GetRadius();
	float x2 = s2.GetX() + s2.GetRadius();
	float y1 = s1.GetY() + s1.GetRadius();
	float y2 = s2.GetY() + s2.GetRadius();
	float r1 = s1.GetRadius();
	float r2 = s2.GetRadius();
	if ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)
		<= (r1 + r2) * (r1 + r2))
	{
		rezult = true;
	}
	return rezult;
};

//--------------------------------------
// 円と長方形の当たり判定
//--------------------------------------

bool CircleToRect(const CircleShape& s1, const RectShape& s2)
{
	bool rezult = false;

	// 点の中心点と矩形の当たり判定
	bool a = (s1.GetX() > s2.GetLeft()) && (s1.GetX() < s2.GetRight()) &&
		(s1.GetY() < s2.GetTop()) && (s1.GetY() > s2.GetBottom());
	bool b = (s1.GetX() > s2.GetLeft()) && (s1.GetX() < s2.GetRight()) &&
		(s1.GetY() < s2.GetTop()) && (s1.GetY() > s2.GetBottom());

	// 矩形の角と円の当たり判定
	bool c = (s2.GetLeft() - s1.GetX()) * (s2.GetLeft() - s1.GetX()) +
		(s2.GetTop() - s1.GetY()) * (s2.GetTop() - s1.GetY()) < s1.GetRadius() * s1.GetRadius();
	bool d = (s2.GetRight() - s1.GetX()) * (s2.GetRight() - s1.GetX()) +
		(s2.GetTop() - s1.GetY()) * (s2.GetTop() - s1.GetY()) < s1.GetRadius() * s1.GetRadius();
	bool e = (s2.GetRight() - s1.GetX()) * (s2.GetRight() - s1.GetX()) +
		(s2.GetBottom() - s1.GetY()) * (s2.GetBottom() - s1.GetY()) < s1.GetRadius() * s1.GetRadius();
	bool f = (s2.GetLeft() - s1.GetX()) * (s2.GetLeft() - s1.GetX()) +
		(s2.GetBottom() - s1.GetY()) * (s2.GetBottom() - s1.GetY()) < s1.GetRadius() * s1.GetRadius();

	rezult = a || b || c || d || e || f;

	return rezult;
};

//--------------------------------------
// 長方形と長方形の当たり判定
//--------------------------------------
bool RectToRect(const RectShape& s1, const RectShape& s2)
{
	bool rezult = false;

	float ax1 = s1.GetLeft();
	float ax2 = s1.GetRight();
	float ay1 = s1.GetTop();
	float ay2 = s1.GetBottom();
	float bx1 = s2.GetLeft();
	float bx2 = s2.GetRight();
	float by1 = s2.GetTop();
	float by2 = s2.GetBottom();
	if ((ax1 <= bx2) && (bx1 <= ax2) && (ay1 >= by2) && (by1 >= ay2))
	{
		rezult = true;
	}
	return rezult;
};