#pragma once
/**
* @file   Task.h
* @brief  Task のインターフェース
* @author Katsumi Takei
* @date   2016 / 06 / 29
*/

#include "../Stdafx/stdafx.h"

class Texture;

enum DrawMode
{
	DRAW_ADD,
	DRAW_ALPHA,
};

class Task
{
protected:
	enum
	{
		PRIORITY_EFFECT,
		PRIORITY_PLAYSCREEN,
		PRIORITY_BACKGROUND,
		PRIORITY_UI,
	};

	bool is_living_;
	int life_, maxlife_;
	int count_;
	Texture *graph_;
	DirectX::SimpleMath::Vector2 spd;
	DirectX::SimpleMath::Vector2 pos;
	float angle_;
	float radius_;
	int priority_;
	DrawMode mode_;
public:
#pragma region setter and getter

	inline void setIsLiving(bool state) { is_living_ = state; }
	inline void setCount(int value) { count_ = value; }
	inline void setLife(int value) { life_ = value; }
	inline void setGraph(Texture *graph) { graph_ = graph; }
	inline void setAngle(float angle) { angle_ = angle; }
	inline void setRadius(float radius) { radius_ = radius; }
	inline void setPos(DirectX::SimpleMath::Vector2 argPos) { pos = argPos; }
	inline void setSpd(DirectX::SimpleMath::Vector2 argSpd){ spd = argSpd; }
	
	inline DirectX::SimpleMath::Vector2 getPos()const { return pos; }
	inline DirectX::SimpleMath::Vector2 getSpd()const { return spd; }
	inline bool getIsLiving()const { return is_living_; }
	inline int getCount()const { return count_; }
	inline int getMaxLife()const { return maxlife_; }
	inline int getLife()const { return life_; }
	inline Texture *getGraph()const { return graph_; }
	inline float getRadius()const { return radius_; }
	inline float getAngle()const { return angle_; }
	inline int priority()const { return priority_; }

	inline std::string ToString() { return typeid(this).name(); }
	inline DrawMode drawMode()const { return mode_; }
#pragma endregion
public:

	Task() : life_(0),maxlife_(0),count_(0),
		angle_(0.f),radius_(0.f),graph_(0),
		pos(DirectX::SimpleMath::Vector2(0.f,0.f)),spd(DirectX::SimpleMath::Vector2(0.f,0.f)){}
	~Task(){}

	virtual void initialize() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void finalize() = 0;

public:
#pragma region operator
	inline bool operator<(const Task &task) const
	{
		return (priority_ < task.priority_);
	}
	inline bool operator>(const Task &task) const
	{
		return (priority_ > task.priority_);
	}
	inline bool operator>=(const Task &task) const 
	{
		return (priority_ >= task.priority_);
	}
	inline bool operator<=(const Task &task) const 
	{
		return (priority_ <= task.priority_);
	}
#pragma endregion
};