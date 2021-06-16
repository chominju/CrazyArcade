#pragma once
class CTexture;
class CMFC_Texture_Manager
{
	DECLARE_SINGLETON(CMFC_Texture_Manager)
public:
	CMFC_Texture_Manager();
	~CMFC_Texture_Manager();


private:
	map<wstring, CTexture*> m_mapTexture;
};

