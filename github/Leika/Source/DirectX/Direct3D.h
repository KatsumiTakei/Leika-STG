#pragma once

class Direct3D
{
private:

	// ドライバータイプ（ハードウェアなど）
	D3D_DRIVER_TYPE driverType_;
	// 機能レベル
	D3D_FEATURE_LEVEL featureLevel_;

	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView_;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView_;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext_;	
	Microsoft::WRL::ComPtr<ID3D11Device> pd3dDevice_;
	
	HRESULT InitBackBuffer();

public:

	static Direct3D *Instance()
	{
		static Direct3D instance;
		return &instance;
	}
	HRESULT InitDevice();
	void Finalize();

#pragma region getter
	inline ID3D11RenderTargetView *renderTargetView()const { return pRenderTargetView_.Get(); }
	inline ID3D11DepthStencilView *depthStencilView()const { return pDepthStencilView_.Get(); }
	inline Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain()const { return pSwapChain_; }
	inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> context()const { return pContext_; }
	inline Microsoft::WRL::ComPtr<ID3D11Device> pd3dDevice()const { return pd3dDevice_; }
#pragma endregion
};

