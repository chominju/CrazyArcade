#include "framework.h"
#include "Single_Texture.h"

CSingle_Texture::CSingle_Texture()
	:m_textureInfo({})
{
}

CSingle_Texture::~CSingle_Texture()
{
	Release_Texture();
}

HRESULT CSingle_Texture::Insert_Texture(const wstring & filePath, const wstring & stateKey, const DWORD & count)
{
	if (FAILED(D3DXGetImageInfoFromFile(filePath.c_str(), &m_textureInfo.imageInfo)))
		goto ERR;

	if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
		filePath.c_str(),
		m_textureInfo.imageInfo.Width,
		m_textureInfo.imageInfo.Height,
		m_textureInfo.imageInfo.MipLevels,
		0,
		m_textureInfo.imageInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr,
		&m_textureInfo.texture)))
		goto ERR;

	return S_OK;


ERR:
	ERR_MSG(L"Single Texture Error");
	return E_FAIL;
}

const Texture_Info* CSingle_Texture::Get_TextureInfo(const wstring & stateKey, const DWORD & index)
{
	return &m_textureInfo;
}

void CSingle_Texture::Release_Texture()
{
	if (m_textureInfo.texture)
		m_textureInfo.texture->Release();
}
