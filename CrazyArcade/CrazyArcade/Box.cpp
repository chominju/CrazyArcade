#include "framework.h"
#include "Box.h"
#include "GameObject_Manager.h"
#include "Item.h"

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
	m_LocationIndex = m_tileInfo.index;
	if (m_dead)
	{
		int randTemp = rand() % 100;
		int randIndex = 0;
		if (randTemp > 90)
		{
			randIndex = 9;// rand() % 8 + 3;
			if (randIndex == 4 || randIndex == 5)
				randIndex = 6;
		}
		else if(randTemp > 50)
		{
			randIndex = rand() % 3;
		}
		else
			return OBJ_DEAD;

		CItem * item = new CItem(randIndex);
		item->Set_ItemData();
		item->Set_Info(m_tileInfo);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);


		return OBJ_DEAD;
	}

	if (m_isPushed && m_tileInfo.drawID==9)
	{
		switch (m_curState)
		{
		case WALK_LEFT:
			m_tileInfo.pos.x -=2;
			break;		   
		case WALK_RIGHT:   
			m_tileInfo.pos.x +=2;
			break;		   
		case WALK_UP:	   
			m_tileInfo.pos.y -=2;
			break;		   
		case WALK_DOWN:	   
			m_tileInfo.pos.y +=2;
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

	//CGraphic_Device::Get_Instance()->Get_Sprite()->End();

	//D3DXVECTOR2 vLine[5]{ {(float)m_rect.left, (float)m_rect.top}, {(float)m_rect.right, (float)m_rect.top},{(float)m_rect.right, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.top }, };
	//CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

	//CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
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
