#include "Game.h"
#include "../Window/Window.h"
#include "../DirectX/Direct3D.h"

#include "../Scene/SceneManager.h"
#include "../Utility/TaskManager.h"
#include "../Utility/GraphManager.h"
#include "../InputManager/InputManager.h"
#include "../Sound/SoundManager.h"
#include "../Utility/TextManager.h"
#include "../Control/Control.h"

#if defined(_DEBUG)
#include <crtdbg.h>
#endif

#pragma comment(lib, "DirectXTK.lib")

using namespace DirectX;

namespace
{
	const DWORD WINDOW_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
}

Game::Game()
{
	window = new Window();
	// 日本語設定しないとワイド文字列の変換で不定値が入って落ちる(おそらく)
	setlocale(LC_ALL, "japanese");

	direct3d = Direct3D::Instance();

	// DirectXデバイスの初期化
	if (FAILED(direct3d->InitDevice()))
	{
		direct3d->Finalize();
		exit(EXIT_FAILURE);
	}

	graphManager = GraphManager::Instance();
	inputManager = InputManager::Instance();
	soundManager = SoundManager::Instance();
	control = Control::Instance();

	graphManager->Start();
	soundManager->Start();
	inputManager->Start();
	control->Start();

	sceneManager = new SceneManager(SCENE_TITLE);

}

void Game::Run()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{// メッセージが来ているか調べる
			// メッセージが来ていればウインドウプロシージャへ送る
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (GetKeyState(0x1B) & 0x800)
			{
				PostMessage(window->windowHundle(), WM_CLOSE, 0, 0);
			}

			inputManager->Update();
			sceneManager->update();

			// バックバッファのクリア
			direct3d->context()->ClearRenderTargetView(direct3d->renderTargetView(), DirectX::Colors::MidnightBlue);

			// 深度バッファのクリア
			direct3d->context()->ClearDepthStencilView(direct3d->depthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			TaskManager::Instance()->SetMode(DRAW_ALPHA);
			graphManager->renderTarget()->BeginDraw();
			graphManager->spriteBatch()->Begin(SpriteSortMode_Deferred, graphManager->state()->NonPremultiplied());
			
			sceneManager->draw();

			graphManager->spriteBatch()->End();
			graphManager->renderTarget()->EndDraw();

			TaskManager::Instance()->SetMode(DRAW_ADD);
			graphManager->spriteBatch()->Begin(SpriteSortMode_Deferred, graphManager->state()->Additive());

			TaskManager::Instance()->Draw();

			graphManager->spriteBatch()->End();

			// バックバッファとフロントバッファを入れ替える
			direct3d->swapChain()->Present(0, 0);
		}
	}

	delete sceneManager;
	direct3d->Finalize();

#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif
}