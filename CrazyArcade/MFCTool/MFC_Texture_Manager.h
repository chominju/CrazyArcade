#pragma once
class CTexture;
class CTexture_Manager
{
	DECLARE_SINGLETON(CTexture_Manager)
public:
	explicit CTexture_Manager();
	~CTexture_Manager();

public:
	HRESULT Insert_Texture_Manager(TEXTURE_ID id,
		const wstring& filePath,
		const wstring& objectKey,
		const wstring& stateKey = L"",
		const DWORD& count = 0);

	const Texture_Info* Get_TextureInfo_Manager(const wstring& objectKey,
		const wstring& stateKey = L"",
		const DWORD& index = 0);

	void Release_Texture_Manager();

private:
	map<wstring, CTexture*> m_mapTexture;
};

