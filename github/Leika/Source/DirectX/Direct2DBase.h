#pragma once

class Direct2DBase
{
public:
	Direct2DBase();
	~Direct2DBase();
	void Create(IDXGISwapChain* swapChain);
	void ReleaseRenderTarget();
	bool CreateRanderTarget(IDXGISwapChain* swapChain);

	ID2D1RenderTarget* GetRenderTarget();
	ID2D1Factory* GetFactory();

private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};