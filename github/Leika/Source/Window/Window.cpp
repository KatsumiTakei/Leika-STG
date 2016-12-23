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
	// �E�C���h�E�̍쐬
	if (FAILED(InitWindow(hInstance_, nCmdShow)))
	{
		exit(EXIT_FAILURE);
	}

}
/**
* @brief �E�C���h�E�̍쐬
* @prame HINSTANCE hInstance, intcCmdShow
* @return �E�B���h�E
*/
HRESULT Window::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	/**
	*	@brief	�E�C���h�E�N���X��o�^����
	*	cbSize			!<	�\���̂̑傫��
	*	style			!<	�T�C�Y���ς������ĕ`�悷��ݒ�
	*	lpfnWndProc		!<	���b�Z�[�W�v���V�[�W���[
	*	cbClsExtra		!<	�w�肵���������\���̂̌��Ƀ��������m�ۂ���
	*	cbWndExtra		!<	�E�B���h�E�������Ɋm�ۂ���,���ʗ̈�̃������̗�
	*	hInstance		!<	�A�v���̃C���X�^���X�n���h��
	*	hIcon			!<	�傫���T�C�Y�̃A�C�R���n���h���̎w��
	*	hCursor			!<	�J�[�\���̃n���h��
	*	hbrBackground	!<	��ʂ̔w�i�F�Ɏg���u���V�̐F
	*	lpszMenuName	!<	���j���[�̃��\�[�X��
	*	lpszClassName	!<	�E�B���h�E�N���X�̖��O
	*	hIconSm			!<	�������T�C�Y�̃A�C�R���n���h���̎w��
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
	// �E�B���h�E�N���X�̓o�^
	if (!RegisterClassEx(&wcex))
	{
		return E_FAIL;
	}
	// �E�C���h�E���쐬����
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
	// �E�C���h�E�̕\��
	ShowWindow(hWnd_, nCmdShow);

	return S_OK;
}

/**
* @brief �E�C���h�E�v���V�[�W���[
* @prame HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
* @return ���b�Z�[�W
* @detail �ÓI�ɂ��Ȃ��ƃN���X�����o�ɂł��Ȃ�
*/
LRESULT CALLBACK Window::LoadWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// �E�C���h�E���j�����ꂽ���b�Z�[�W
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// �E�C���h�E���A�N�e�B�u���A��A�N�e�B�u�����鎞�ɗ��郁�b�Z�[�W
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

		// �L�[���͊֌W�̃��b�Z�[�W
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	default:
		// �����ŏ������Ȃ����b�Z�[�W��DefWindowProc�֐��ɔC����
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