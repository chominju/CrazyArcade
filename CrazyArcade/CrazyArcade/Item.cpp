#include "framework.h"
#include "Item.h"
#include "GameObject_Manager.h"

CItem::CItem(int index)
{
	m_itemIndex = index;
}

CItem::~CItem()
{
}

void CItem::Set_ItemData()
{
	m_itemData = CGameObject_Manager::Get_Instance()->Get_ItemData(m_itemIndex);
}

void CItem::Set_Info(Tile_Info info)
{
	m_info.pos = info.pos;
	m_info.size = info.size;
	Set_Rect();
}

HRESULT CItem::Ready_GameObject()
{
	return E_NOTIMPL;
}

int CItem::Update_GameObject()
{
	if (m_dead)
		return OBJ_DEAD;
	return 0;
}

void CItem::Late_Update_GameObject()
{
}

void CItem::Render_GameObject()
{
	const Texture_Info* pTexInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Item", L"ItemObj",m_itemIndex);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_info.size.x, m_info.size.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_info.pos.x, m_info.pos.y, 0.f);
	matWorld = matScale * matTrans;
	//float fCenterX = pTexInfo->imageInfo.Width >> 1;
	//float fCenterY = pTexInfo->imageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->texture, nullptr, &D3DXVECTOR3(0/*fCenterX, fCenterY*/, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CItem::Release_GameObject()
{
}
