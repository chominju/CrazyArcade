#include "pch.h"
#include "framework.h"
#include "Multi_Texture.h"

CMulti_Texture::CMulti_Texture()
{
}

CMulti_Texture::~CMulti_Texture()
{
	Release_Texture();
}


HRESULT CMulti_Texture::Insert_Texture(const wstring & filePath, const wstring & stateKey, const DWORD & count)
{
	auto iter_find = m_mapMultiTexture.find(stateKey);
	if (iter_find == m_mapMultiTexture.end())
	{
		TCHAR szFilePath[MAX_PATH] = L"";
		Texture_Info* textureInfo = nullptr;
		for (int i = 0; i < count; i++)
		{
			swprintf_s(szFilePath, filePath.c_str(), i);
			textureInfo = new Texture_Info;
			if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &textureInfo->imageInfo)))
				goto ERR;
			if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
				szFilePath,
				textureInfo->imageInfo.Width,
				textureInfo->imageInfo.Height,
				textureInfo->imageInfo.MipLevels,
				0,
				textureInfo->imageInfo.Format,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,
				nullptr,
				nullptr,
				&textureInfo->texture)))
				goto ERR;

			m_mapMultiTexture[stateKey].emplace_back(textureInfo);
		}
	}

	return S_OK;
ERR:
	wstring wstrCombine = filePath+ L" Insert MultiTex Failed";
	ERR_MSG(wstrCombine.c_str());
	return E_FAIL;
}

const Texture_Info * CMulti_Texture::Get_TextureInfo(const wstring & stateKey, const DWORD & index)
{
	auto iter_find = m_mapMultiTexture.find(stateKey);
	if (iter_find == m_mapMultiTexture.end())
		return nullptr;

	if (iter_find->second.size() <= index)
		return nullptr;

	return iter_find->second[index];
}

void CMulti_Texture::Release_Texture()
{
	for (auto & pair : m_mapMultiTexture)
	{
		for (auto & textureInfo : pair.second)
			Safe_Delete(textureInfo);

		pair.second.clear();
		pair.second.shrink_to_fit();
	}
	m_mapMultiTexture.clear();
}
