#include "DWriteBase.h"
#include "../Stdafx/stdafx.h"

using namespace Microsoft::WRL;

#pragma comment(lib,"DWrite.lib")

class DWriteBase::Impl
{
public:
	ComPtr<IDWriteFactory> factory;

	Impl()
	{
	}
	~Impl()
	{
	}

	void Create()
	{
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(factory.GetAddressOf()));
	}
};

DWriteBase::DWriteBase() : pImpl(new Impl())
{
}

DWriteBase::~DWriteBase()
{
}

void DWriteBase::Create()
{
	pImpl->Create();
}

IDWriteFactory * DWriteBase::GetFactory()
{
	return pImpl->factory.Get();
}

