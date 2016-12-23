#pragma once

class DWriteBase
{
public:
	DWriteBase();
	~DWriteBase();

	void Create();

	IDWriteFactory* GetFactory();

private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};