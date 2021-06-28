#pragma once
class CTexture abstract
{
public:
	explicit CTexture();
	virtual ~CTexture();

	virtual HRESULT Insert_Texture(const wstring& filePath, const wstring& stateKey = L"", const DWORD& count = 0)PURE;
	virtual const Texture_Info* Get_TextureInfo(const wstring& stateKey = L"", const DWORD& index = 0)PURE;
	virtual void Release_Texture()PURE;
};

