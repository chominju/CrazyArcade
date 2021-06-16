#pragma once
#include "Texture.h"
class CMulti_Texture :
	public CTexture
{
public:
	explicit CMulti_Texture();
	virtual ~CMulti_Texture();
public:
	// CTexture을(를) 통해 상속됨
	virtual HRESULT Insert_Texture(const wstring& filePath, const wstring& stateKey = L"", const DWORD& count = 0)override;
	virtual const Texture_Info* Get_TextureInfo(const wstring& stateKey = L"", const DWORD& index = 0)override;
	virtual void Release_Texture()PURE;
private:
	map<wstring, vector<Texture_Info*>> m_mapMultiTexture;

};

