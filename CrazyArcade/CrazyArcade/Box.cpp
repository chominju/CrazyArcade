#include "framework.h"
#include "Box.h"
#include "GameObject_Manager.h"

HRESULT CBox::Ready_GameObject()
{
	return E_NOTIMPL;
}

int CBox::Update_GameObject()
{
	if (m_isPushed && m_info.drawID==9)
	{
		switch (m_curState)
		{
		case WALK_LEFT:
			m_info.pos.x -=1;
			break;		   
		case WALK_RIGHT:   
			m_info.pos.x +=1;
			break;		   
		case WALK_UP:	   
			m_info.pos.y -=1;
			break;		   
		case WALK_DOWN:	   
			m_info.pos.y +=1;
			break;
		}

		if (m_curState == WALK_LEFT || m_curState == WALK_RIGHT)
		{
			if (m_finishX == m_info.pos.x)
			{
				m_isPushed = false;
				m_info.centerX = m_info.pos.x + TILECX * expansionSize / 2;
				m_info.index = m_finishIndex;
				m_finishIndex = -1;
			}
		}
		if (m_curState == WALK_UP || m_curState == WALK_DOWN)
		{
			if (m_finishY == m_info.pos.y)
			{
				m_isPushed = false;
				m_info.centerY = m_info.pos.y + TILECY * expansionSize / 2;
				m_info.index = m_finishIndex;
				m_finishIndex = -1;
			}
		}
	}
	return 0;
}

void CBox::Late_Update_GameObject()
{
}

void CBox::Render_GameObject()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	TCHAR szBuf[32]{};
	wstring objectKey;
	wstring stateKey;

	if (m_info.objectKey == 2)
		objectKey = L"Box";
	if (m_info.stateKey == 2)
		stateKey = L"BoxObj";

	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_info.drawID);
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

void CBox::Release_GameObject()
{
}
