#pragma once
class CLoad_Manager
{
private:
	CLoad_Manager();
	~CLoad_Manager();
public:

	static HRESULT LoadTerrainData(const wstring& filePath);
	static HRESULT LoadTextureData(const wstring& filePath);

};
