#include "framework.h"
#include "Water.h"

CWater::CWater(int indexX , int indexY)
	:m_indexX(indexX)
	,m_indexY(indexY)
	, m_isBoxDestroy(false)
{
}

CWater::~CWater()
{
}

HRESULT CWater::Ready_GameObject()
{
	m_objectKey = L"Water";
	m_stateKey = L"Center";

	m_info.pos = { m_indexX * TILECX * expansionSize + STARTX ,m_indexY * TILECY * expansionSize + STARTY,0.f };
	m_info.dir = { 1.f,1.f,0.f };
	m_info.size = { expansionSize,expansionSize,0.f };
	m_LocationIndex = m_indexX + m_indexY * TILEX;
	m_frame = { 0.f,4.f };

	Set_Rect();

	return E_NOTIMPL;
}

int CWater::Update_GameObject()
{
	if (m_dead)
		return OBJ_DEAD;
	return 0;
}

void CWater::Late_Update_GameObject()
{
	FrameMove(3.f);
}

void CWater::Render_GameObject()
{
	if (!m_isBoxDestroy)
	{
		const Texture_Info* pTexInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(m_objectKey, m_stateKey, (DWORD)m_frame.frameStart);
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
	
	


		/*CGraphic_Device::Get_Instance()->Get_Sprite()->End();

		D3DXVECTOR2 vLine[5]{ {(float)m_rect.left, (float)m_rect.top}, {(float)m_rect.right, (float)m_rect.top},{(float)m_rect.right, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.top }, };
		CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

		CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);*/
	}
}

void CWater::FrameMove(float speed)
{
	m_frame.frameStart += m_frame.frameEnd * CTime_Manager::Get_Instance()->Get_DeltaTime() * speed;
	if (m_frame.frameEnd < m_frame.frameStart)
		m_dead = true;
}

void CWater::Release_GameObject()
{
}
