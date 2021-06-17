#include "pch.h"
#include "framework.h"
#include "MFC_Texture_Manager.h"
#include "Single_Texture.h"
#include "Multi_Texture.h"

IMPLEMENT_SINGLETON(CTexture_Manager)
CTexture_Manager::CTexture_Manager()
{
}


CTexture_Manager::~CTexture_Manager()
{
	Release_Texture_Manager();
}

HRESULT CTexture_Manager::Insert_Texture_Manager(TEXTURE_ID id, const wstring & filePath, const wstring & objectKey, const wstring & stateKey, const DWORD & count)
{
	auto& iter_find = m_mapTexture.find(objectKey);
	CTexture* newTexture = nullptr;
	if (m_mapTexture.end == iter_find)
	{
		switch (id)
		{
		case TEXTURE_SINGLE:
			newTexture = new CSingle_Texture;
			break;
		case TEXTURE_MULTI:
			newTexture = new CMulti_Texture;
			break;
		default:
			break;
		}
		if (FAILED(newTexture->Insert_Texture(filePath, stateKey, count)))
			goto ERR;

		m_mapTexture.emplace(objectKey, newTexture);
	}
	else if (TEXTURE_MULTI == id)
	{
		if (FAILED(m_mapTexture[objectKey]->Insert_Texture(filePath, stateKey, count)))
			goto ERR;
	}
	return S_OK;

ERR:
	ERR_MSG(L"Texture Insert Failed");
	return E_FAIL;


}

const Texture_Info * CTexture_Manager::Get_TextureInfo_Manager(const wstring & objectKey, const wstring & stateKey, const DWORD & index)
{
	auto& iter_find = m_mapTexture.find(objectKey);
	if (m_mapTexture.end() == iter_find)
		return nullptr;
	return iter_find->second->Get_TextureInfo(stateKey, index);
}

void CTexture_Manager::Release_Texture_Manager()
{
	for (auto& pair : m_mapTexture)
		Safe_Delete(pair.second);
	m_mapTexture.clear();
}
