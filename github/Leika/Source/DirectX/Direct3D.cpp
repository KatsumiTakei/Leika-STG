#include "Direct3D.h"

#include "../Window/Window.h"

using namespace Microsoft::WRL;
#pragma comment(lib, "d3d11.lib")

HRESULT Direct3D::InitDevice()
{
	HWND hWnd = Window::window()->windowHundle();
	bool isFullScreen;
	{
#if defined (_DEBUG)
		isFullScreen = false;
#else
		UINT messege = MessageBox(nullptr, L"フルスクリーンにしますか？", L"スクリーンサイズ設定", MB_YESNO | MB_ICONQUESTION);
		if (messege == IDNO)
		{
			isFullScreen = false;
		}
		else
		{
			isFullScreen = true;
		}
#endif

		driverType_ = D3D_DRIVER_TYPE_NULL;
		featureLevel_ = D3D_FEATURE_LEVEL_11_0;
	}

	HRESULT hr = S_OK;

	// クライアント領域の取得
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;	// デバックレイヤーをサポートするフラグ
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;	// <--リフレッシュレートはDXGIの自動設定に任せる
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = !isFullScreen;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // <--フルスクリーンへの切り替えの時に表示モードを変更する

	//////////////////////////////////////
	// デバイスとスワップチェーンの作成	//
	//////////////////////////////////////
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType_ = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType_, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &pSwapChain_, &pd3dDevice_, &featureLevel_, &pContext_);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	//////////////////////////////////////////////////
	// Atl+Enterで画面モードを変更しないようにする	//
	//////////////////////////////////////////////////
	ComPtr<IDXGIDevice1> pDXGIDevice;
	ComPtr<IDXGIAdapter> pDXGIAdapter;
	ComPtr<IDXGIFactory> pDXGIFactory;

	// インターフェイス取得
	hr = pd3dDevice_.As(&pDXGIDevice);
	if (FAILED(hr)) {
		return hr;
	}

	// アダプター取得
	hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
	if (FAILED(hr)) {
		return hr;
	}
	// ファクトリー取得
	hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (LPVOID*)&pDXGIFactory);
	if (FAILED(hr)) {
		return hr;
	}
	// 画面モードの切り替え機能の設定
	hr = pDXGIFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
	if (FAILED(hr)) {
		return hr;
	}

	//////////////////////////
	// バックバッファの作成	//
	//////////////////////////
	hr = InitBackBuffer();
	if (FAILED(hr))
		return hr;

	//////////////////////
	// ビューポート作成	//
	//////////////////////
	CD3D11_VIEWPORT vp(0.0f, 0.0f, (FLOAT)width, (FLOAT)height);
	pContext_->RSSetViewports(1, &vp);

	return S_OK;
}

HRESULT Direct3D::InitBackBuffer()
{
	HRESULT hr = S_OK;

	//////////////////////////
	// バックバッファの作成	//
	//////////////////////////

	// スワップチェーンからバックバッファを取得
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	// バックバッファに指定されているテクスチャ情報を取得
	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);

	// レンダーターゲットビューを作成
	hr = pd3dDevice_->CreateRenderTargetView(pBackBuffer, nullptr, pRenderTargetView_.GetAddressOf());
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	//////////////////////////
	// 深度バッファの作成	//
	//////////////////////////

	// 深度ステンシルテクスチャの作成
	CD3D11_TEXTURE2D_DESC descDepth(descBackBuffer);	// <--バックバッファに指定されているテクスチャ情報を元に設定する
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pd3dDevice_->CreateTexture2D(&descDepth, nullptr, &pDepthStencil_);
	if (FAILED(hr))
		return hr;

	// 深度ステンシルビューの作成
	CD3D11_DEPTH_STENCIL_VIEW_DESC descDSV(D3D11_DSV_DIMENSION_TEXTURE2D, descDepth.Format);
	hr = pd3dDevice_->CreateDepthStencilView(pDepthStencil_.Get(), &descDSV, &pDepthStencilView_);
	if (FAILED(hr))
		return hr;

	//////////////////////////////
	// レンダーターゲット設定	//
	//////////////////////////////
	pContext_->OMSetRenderTargets(1, pRenderTargetView_.GetAddressOf(), pDepthStencilView_.Get());

	return S_OK;
}

void Direct3D::Finalize()
{
	if (pContext_) pContext_->ClearState();
}

