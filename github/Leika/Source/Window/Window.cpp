#include "Window.h"
#include "../Constant.h"
#include "../Utility/Debug.h"
#include "../InputManager/InputManager.h"
#include "../../Resources/Icon/resource.h"

Window *Window::window_ = nullptr;

namespace
{
	const DWORD WINDOW_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	LPCWSTR WindowName = L"WindowClass";
}

using namespace Constant;
using namespace DirectX;

Window::Window()
{
	STARTUPINFO si;
	si.dwFlags = 0;
	GetStartupInfo(&si);
	nCmdShow = si.dwFlags & STARTF_USESHOWWINDOW ? si.wShowWindow : SW_SHOWDEFAULT;
	hInstance_ = GetModuleHandle(NULL);
	window_ = this;
#if defined(_DEBUG)
	debug->CreateConsoleWindow();
#endif
	// ウインドウの作成
	if (FAILED(InitWindow(hInstance_, nCmdShow)))
	{
		exit(EXIT_FAILURE);
	}

}
/**
* @brief ウインドウの作成
* @prame HINSTANCE hInstance, intcCmdShow
* @return ウィンドウ
*/
HRESULT Window::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	/**
	*	@brief	ウインドウクラスを登録する
	*	cbSize			!<	構造体の大きさ
	*	style			!<	サイズが変わったら再描画する設定
	*	lpfnWndProc		!<	メッセージプロシージャー
	*	cbClsExtra		!<	指定した数だけ構造体の後ろにメモリを確保する
	*	cbWndExtra		!<	ウィンドウ生成時に確保する,特別領域のメモリの量
	*	hInstance		!<	アプリのインスタンスハンドル
	*	hIcon			!<	大きいサイズのアイコンハンドルの指定
	*	hCursor			!<	カーソルのハンドル
	*	hbrBackground	!<	画面の背景色に使うブラシの色
	*	lpszMenuName	!<	メニューのリソース名
	*	lpszClassName	!<	ウィンドウクラスの名前
	*	hIconSm			!<	小さいサイズのアイコンハンドルの指定
	*/

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = LoadWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = WindowName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	// ウィンドウクラスの登録
	if (!RegisterClassEx(&wcex))
	{
		return E_FAIL;
	}
	// ウインドウを作成する
	hInstance_ = hInstance;
	RECT rc = { 0, 0, SCREEN_RIGHT, SCREEN_BOTTOM };
	AdjustWindowRect(&rc, WINDOW_STYLE, FALSE);
	hWnd_ = CreateWindow(wcex.lpszClassName, TITLE_NAME, WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, hInstance, nullptr);
	if (!hWnd_)
	{
		return E_FAIL;
	}
	// ウインドウの表示
	ShowWindow(hWnd_, nCmdShow);

	return S_OK;
}

/**
* @brief ウインドウプロシージャー
* @prame HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
* @return メッセージ
* @detail 静的にしないとクラスメンバにできない
*/
LRESULT CALLBACK Window::LoadWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// ウインドウが破棄されたメッセージ
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// ウインドウがアクティブ化、非アクティブ化する時に来るメッセージ
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

		// キー入力関係のメッセージ
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	default:
		// 自分で処理しないメッセージはDefWindowProc関数に任せる
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Window::DestoryWindow()
{
	BOOL result;
	result = DestroyWindow(hWnd_);
	result = UnregisterClass(WindowName, hInstance_);
}