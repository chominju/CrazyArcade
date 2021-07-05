#include "framework.h"
#include "Wall.h"

HRESULT CWall::Ready_GameObject()
{
	return E_NOTIMPL;
}

int CWall::Update_GameObject()
{
	return 0;
}

void CWall::Late_Update_GameObject()
{
	Set_Rect();
}

void CWall::Render_GameObject()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	TCHAR szBuf[32]{};
	wstring objectKey;
	wstring stateKey;

	if (m_info.objectKey == 3)
		objectKey = L"Wall";
	if (m_info.stateKey == 3)
		stateKey = L"WallObj";

	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_info.drawID);
	if (nullptr == textureInfo)
		return;
	//float centerX = float(textureInfo->imageInfo.Width >> 1);
	//float centerY = float(textureInfo->imageInfo.Height >> 1);

	float centerY = float(textureInfo->imageInfo.Height) - float(TILECY);

	D3DXMatrixScaling(&matScale, m_info.size.x, m_info.size.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_info.pos.x, m_info.pos.y, 0.f);
	matWorld = matScale * matTrans;

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}


void CWall::Release_GameObject()
{
}
