#include "framework.h"
#include "Shield.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "Player.h"
CShield::CShield(Object_Info info)
	: m_disappear(3.f)
{
	m_info = info;


	m_frame.frameStart = 0;
	m_frame.frameEnd = 4;
}

CShield::~CShield()
{
}

HRESULT CShield::Ready_GameObject()
{
	return E_NOTIMPL;
}

int CShield::Update_GameObject()
{
	m_disappear -= CTime_Manager::Get_Instance()->Get_DeltaTime();

	m_info = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Object_Info();
	
	if (m_disappear <= 0)
	{
		dynamic_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_IsUseShield(false);
		return OBJ_DEAD;
	}
	return 0;
}

void CShield::Late_Update_GameObject()
{
	FrameMove(2);
}

void CShield::Render_GameObject()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	wstring objectKey;
	wstring stateKey;

	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Item", L"Shield", m_frame.frameStart);
	if (nullptr == textureInfo)
		return;
	//float centerX = float(textureInfo->imageInfo.Width >> 1);
	//float centerY = float(textureInfo->imageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_info.size.x, m_info.size.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_info.pos.x, m_info.pos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



	/*CGraphic_Device::Get_Instance()->Get_Sprite()->End();

	D3DXVECTOR2 vLine[5]{ {(float)m_rect.left, (float)m_rect.top}, {(float)m_rect.right, (float)m_rect.top},{(float)m_rect.right, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.top }, };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);*/
}

void CShield::Release_GameObject()
{
}
