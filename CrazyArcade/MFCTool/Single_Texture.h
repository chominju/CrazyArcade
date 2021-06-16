#pragma once
#include "Texture.h"
class CSingle_Texture :
	public CTexture
{
public:
	explicit CSingle_Texture();
	virtual ~CSingle_Texture();

	// CTexture을(를) 통해 상속됨
	virtual HRESULT Insert_Texture(const wstring & filePath, const wstring & stateKey = L"", const DWORD & count = 0) override;
	virtual const Texture_Info* Get_TextureInfo(const wstring & stateKey = L"", const DWORD & index = 0) override;
	virtual void Release_Texture() override;

private:
	Texture_Info m_textureInfo;
};

