#include "framework.h"
#include "Tile.h"

HRESULT CTile::Ready_GameObject()
{
	return E_NOTIMPL;
}

int CTile::Update_GameObject()
{
	return 0;
}

void CTile::Late_Update_GameObject()
{
}

void CTile::Render_GameObject()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	TCHAR szBuf[32]{};
	wstring objectKey;
	wstring stateKey;
	if (m_tileInfo.objectKey == 1)
		objectKey = L"Terrain";
	if (m_tileInfo.stateKey == 1)
		stateKey = L"Tile";

	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_tileInfo.drawID);
	if (nullptr == textureInfo)
		return;
	//float centerX = float(textureInfo->imageInfo.Width >> 1);
	//float centerY = float(textureInfo->imageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tileInfo.size.x, m_tileInfo.size.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tileInfo.pos.x, m_tileInfo.pos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



	/*CGraphic_Device::Get_Instance()->Get_Sprite()->End();

	D3DXVECTOR2 vLine[5]{ {(float)m_rect.left, (float)m_rect.top}, {(float)m_rect.right, (float)m_rect.top},{(float)m_rect.right, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.top }, };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);*/


	//D3DXMATRIX matScale, matTrans, matWorld;
	//DWORD size = m_vecTile.size();
	//TCHAR szBuf[32]{};
	////m_vecTile[31]->drawID = 0;
	//for (size_t i = 0; i < size; ++i)
	//{
	//	if (m_vecTile[i] == NULL)
	//		continue;

	//	wstring objectKey;
	//	wstring stateKey;
	//	if (m_vecTile[i]->objectKey == 1)
	//		objectKey = L"Terrain";
	//	if (m_vecTile[i]->stateKey == 1)
	//		stateKey = L"Tile";

	//	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_vecTile[i]->drawID);
	//	if (nullptr == textureInfo)
	//		continue;
	//	//float centerX = float(textureInfo->imageInfo.Width >> 1);
	//	//float centerY = float(textureInfo->imageInfo.Height >> 1);

	//	D3DXMatrixScaling(&matScale, m_vecTile[i]->size.x, m_vecTile[i]->size.y, 0.f);
	//	D3DXMatrixTranslation(&matTrans, m_vecTile[i]->pos.x, m_vecTile[i]->pos.y, 0.f);
	//	matWorld = matScale * matTrans;

	//	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	//	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//	swprintf_s(szBuf, L"%d", i);
	//	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

	//}

	//size = m_vecObj.size();
	//for (size_t i = 0; i < size; ++i)
	//{
	//	if (m_vecObj[i] == NULL)
	//		continue;

	//	wstring objectKey;
	//	wstring stateKey;
	//	if (m_vecObj[i]->objectKey == 2)
	//		objectKey = L"Box";
	//	if (m_vecObj[i]->stateKey == 2)
	//		stateKey = L"BoxObj";

	//	if (m_vecObj[i]->objectKey == 3)
	//		objectKey = L"Wall";
	//	if (m_vecObj[i]->stateKey == 3)
	//		stateKey = L"WallObj";

	//	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_vecObj[i]->drawID);
	//	if (nullptr == textureInfo)
	//		continue;
	//	float centerX = float(textureInfo->imageInfo.Width >> 1);
	//	float centerY = float(textureInfo->imageInfo.Height) - float(TILECY);

	//	D3DXMatrixScaling(&matScale, m_vecObj[i]->size.x, m_vecObj[i]->size.y, 0.f);
	//	D3DXMatrixTranslation(&matTrans, m_vecObj[i]->pos.x, m_vecObj[i]->pos.y, 0.f);
	//	matWorld = matScale * matTrans;

	//	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	//	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//}
}

void CTile::Release_GameObject()
{

}
