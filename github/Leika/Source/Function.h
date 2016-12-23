#pragma once

#include "Stdafx/stdafx.h"

class Texture;
class Task;

double rang(double ang);
bool isInArea(DirectX::SimpleMath::Vector2 vec);
float DegreeToRadian(float degree);
int XorRand();
void DrawNum(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 size, const float n, Texture* graph, int max, DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1.f,1.f,1.f,1.f));
bool compareByA(const Task *left, const Task *right);

template <typename T>
inline void SafeDelete(T*& p)
{
	if (p != NULL) 
	{
		delete (p);
		(p) = NULL;
	}
}

template <typename T>
inline void SafeDeleteArray(T*& p) 
{
	if (p != NULL) 
	{
		delete[](p);
		(p) = NULL;
	}
}

template <typename T>
inline void SafeRelease(T*& p) 
{
	if (p != NULL) 
	{
		(p)->Release();
		(p) = NULL;
	}
}