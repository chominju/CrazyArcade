#include "framework.h"
#include "Box.h"
#include "GameObject_Manager.h"

HRESULT CBox::Ready_GameObject()
{
	return E_NOTIMPL;
}

CBox::CBox()
{
}

CBox::~CBox()
{
}

int CBox::Update_GameObject()
{
	if (m_dead)
		return OBJ_DEAD;

	if (m_isPushed && m_tileInfo.drawID==9)
	{
		switch (m_curState)
		{
		case WALK_LEFT:
			m_tileInfo.pos.x -=1;
			break;		   
		case WALK_RIGHT:   
			m_tileInfo.pos.x +=1;
			break;		   
		case WALK_UP:	   
			m_tileInfo.pos.y -=1;
			break;		   
		case WALK_DOWN:	   
			m_tileInfo.pos.y +=1;
			break;
		}

		if (m_curState == WALK_LEFT || m_curState == WALK_RIGHT)
		{
			if (m_finishX == m_tileInfo.pos.x)
			{
				m_isPushed = false;
				m_tileInfo.centerX = m_tileInfo.pos.x + TILECX * expansionSize / 2;
				m_tileInfo.index = m_finishIndex;
				m_finishIndex = -1;
			}
		}
		if (m_curState == WALK_UP || m_curState == WALK_DOWN)
		{
			if (m_finishY == m_tileInfo.pos.y)
			{
				m_isPushed = false;
				m_tileInfo.centerY = m_tileInfo.pos.y + TILECY * expansionSize / 2;
				m_tileInfo.index = m_finishIndex;
				m_finishIndex = -1;
			}
		}
	}
	return 0;
}

void CBox::Late_Update_GameObject()
{
	Set_Rect();
}

void CBox::Render_GameObject()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	TCHAR szBuf[32]{};
	wstring objectKey;
	wstring stateKey;

	if (m_tileInfo.objectKey == 2)
		objectKey = L"Box";
	if (m_tileInfo.stateKey == 2)
		stateKey = L"BoxObj";

	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_tileInfo.drawID);
	if (nullptr == textureInfo)
		return;

	m_textureSizeX = float(textureInfo->imageInfo.Width * expansionSize);
	m_textureSizeY = float(textureInfo->imageInfo.Height* expansionSize);


	//float centerX = float(textureInfo->imageInfo.Width >> 1);
	//float centerY = float(textureInfo->imageInfo.Height >> 1);
	float centerY = float(textureInfo->imageInfo.Height) - float(TILECY);

	D3DXMatrixScaling(&matScale, m_tileInfo.size.x, m_tileInfo.size.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tileInfo.pos.x, m_tileInfo.pos.y, 0.f);
	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CBox::Release_GameObject()
{
}

void CBox::Set_Rect()
{
	m_rect.left = m_tileInfo.pos.x;
	m_rect.top = m_tileInfo.pos.y;
	m_rect.right = m_tileInfo.pos.x + TILECX * expansionSize;
	m_rect.bottom = m_tileInfo.pos.y + TILECY * expansionSize;
}
