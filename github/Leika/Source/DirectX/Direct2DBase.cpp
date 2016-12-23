#include "Direct2DBase.h"
#include "../Stdafx/stdafx.h"

using namespace std;
using namespace Microsoft::WRL;

#pragma comment(lib,"D2d1.lib")

class Direct2DBase::Impl
{
public:
	ComPtr<ID2D1Factory> factory;
	ComPtr<ID2D1RenderTarget> renderTarget;

	Impl()
	{
	}
	~Impl()
	{
	}

	// オブジェクトの作成
	void Create(IDXGISwapChain* swapChain)
	{
		// ファクトリの作成
		// https://msdn.microsoft.com/ja-jp/library/windows/desktop/dd535473(v=vs.85).aspx

#if defined(DEBUG) || defined(_DEBUG)
		D2D1_FACTORY_OPTIONS options;
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			options,
			factory.GetAddressOf());
#else
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			factory.GetAddressOf());
#endif
		CreateRanderTarget(swapChain);
	}

	void ReleaseRenderTarget()
	{
		renderTarget.Reset();
	}

	bool CreateRanderTarget(IDXGISwapChain* swapChain)
	{

		// レンダーターゲットの作成
		// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=JA-JP&k=k(d2d1%2FID2D1Factory%3A%3ACreateDxgiSurfaceRenderTarget);k(ID2D1Factory%3A%3ACreateDxgiSurfaceRenderTarget);k(CreateDxgiSurfaceRenderTarget);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true

		ComPtr<IDXGISurface> surface;

		HRESULT hr;

		hr = swapChain->GetBuffer(
			0,
			IID_PPV_ARGS(surface.GetAddressOf()));

		if (FAILED(hr)) return false;

		FLOAT dpiX;
		FLOAT dpiY;

		factory->GetDesktopDpi(&dpiX, &dpiY);

		D2D1_RENDER_TARGET_PROPERTIES rtp;
		rtp = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY);

		hr = factory->CreateDxgiSurfaceRenderTarget(
			surface.Get(),
			&rtp,
			renderTarget.GetAddressOf());

		if (FAILED(hr)) return false;

		return true;
	}

	void Draw()
	{
		renderTarget->BeginDraw();

		//renderTarget->DrawRectangle(
		//	D2D1::RectF(0, 0, 100, 100),
		//	brush.Get());

		renderTarget->EndDraw();
	}
};

Direct2DBase::Direct2DBase() : pImpl(new Impl())
{
}

Direct2DBase::~Direct2DBase()
{
}

void Direct2DBase::Create(IDXGISwapChain * swapChain)
{
	pImpl->Create(swapChain);
}

void Direct2DBase::ReleaseRenderTarget()
{
	pImpl->ReleaseRenderTarget();
}

bool Direct2DBase::CreateRanderTarget(IDXGISwapChain * swapChain)
{
	return pImpl->CreateRanderTarget(swapChain);
}

ID2D1RenderTarget * Direct2DBase::GetRenderTarget()
{
	return pImpl->renderTarget.Get();
}

ID2D1Factory * Direct2DBase::GetFactory()
{
	return pImpl->factory.Get();
}

