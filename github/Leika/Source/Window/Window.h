#pragma once
#include "../Stdafx/stdafx.h"

class Window
{
private:
	HINSTANCE hInstance_;
	HWND hWnd_;
	int nCmdShow;

	static Window *window_;
public:

	Window();
	~Window() {};

	static Window *window()
	{
		return window_;
	}

	inline HWND windowHundle()
	{
		return hWnd_;
	}

	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	static LRESULT CALLBACK LoadWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void DestoryWindow();
};