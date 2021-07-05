#include "framework.h"
#include "WaterBall.h"
#include "GameObject_Manager.h"
CWaterBall::CWaterBall(int index)
	: m_animIndex(0)
	, m_time(3.f)
{
	m_LocationIndex = index;
	int tempY = index / TILEX;
	int tempX = index % TILEX;

	m_info.pos.x = STARTX + tempX * TILECX * expansionSize;
	m_info.pos.y = STARTY + tempY * TILECY * expansionSize;
	m_info.size = { expansionSize,expansionSize ,expansionSize };

	m_frame.frameStart = 0;
	m_frame.frameEnd = 3;
}

CWaterBall::~CWaterBall()
{
}

HRESULT CWaterBall::Ready_GameObject()
{
	return E_NOTIMPL;
}

int CWaterBall::Update_GameObject()
{
	m_time -= CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_time <= 0)
	{
		CGameObject_Manager::Get_Instance()->Get_Player()->Decrese_WaterBall();
		return OBJ_DEAD;
	}
	
	return 0;
}

void CWaterBall::Late_Update_GameObject()
{
	FrameMove(1);
}

void CWaterBall::Render_GameObject()
{
		D3DXMATRIX matScale, matTrans, matWorld;
		TCHAR szBuf[32]{};
		wstring objectKey;
		wstring stateKey;

		const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"WaterBall", L"Idle", m_frame.frameStart);
		if (nullptr == textureInfo)
			return;
		//float centerX = float(textureInfo->imageInfo.Width >> 1);
		//float centerY = float(textureInfo->imageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_info.size.x, m_info.size.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_info.pos.x, m_info.pos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CWaterBall::Release_GameObject()
{
}
