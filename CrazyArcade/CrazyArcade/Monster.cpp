#include "framework.h"
#include "Monster.h"
#include "Collision_Manager.h"
#include "GameObject_Manager.h"
#include "Terrain.h"
CMonster::CMonster()
	:m_monsterIndex(1)
{

}

CMonster::~CMonster()
{

}

void CMonster::Set_MonsterIndex(int index)
{
	m_monsterIndex = index;
}

void CMonster::Set_RespawnPos(int index)
{
	auto respawnTile = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::RESPAWN_TILE);
	list<CGameObject*>::iterator iter = respawnTile.begin();

	while (index!=0)
	{
		index--;
		iter++;
	}
	m_info.pos.x = dynamic_cast<CTerrain*>(*iter)->Get_Terrain_Info().pos.x;
	m_info.pos.y = dynamic_cast<CTerrain*>(*iter)->Get_Terrain_Info().pos.y;


}

HRESULT CMonster::Ready_GameObject()
{
	m_info.pos.x = 100.0f;
	m_info.pos.y = 100.0f;
	m_info.dir = { 1.f,1.f,0.f };
	m_info.size = { expansionSize,expansionSize,0.f };

	m_curState = CHARACTER_STATE::WALK_RIGHT;
	m_frame = { 0.f,2.f };

	if (m_monsterIndex == 1)
	{
		m_monsterSizeX = 34 * expansionSize;
		m_monsterSizeY = 38 * expansionSize;
		m_playerableInfo.speed = 0.3;
		m_objectKey = L"Monster1";
		m_stateKey = L"Monster1_Right";
	
	}
	else if (m_monsterIndex == 2)
	{
		m_monsterSizeX = 41 * expansionSize;
		m_monsterSizeY = 39 * expansionSize;
		m_playerableInfo.speed = 0.7f;
		m_objectKey = L"Monster2";
		m_stateKey = L"Monster2_Right";
	}
	


	return S_OK;
}

int CMonster::Update_GameObject()
{
	//m_preState = m_curState;
	//return 0;
	if (m_isDeadAnimFin)
		return OBJ_DEAD;
	if (m_dead && !m_isDeadAnim)
	{
		if (m_monsterIndex == 1)
			m_stateKey = L"Monster1_Die";
		else
			m_stateKey = L"Monster2_Die";

		m_frame.frameStart = 0;
		m_isDeadAnim = true;
	}
	else if (!m_dead && !m_isDeadAnim)
	{
		Set_Rect();

		int CollisionObject = CCollision_Manager::Collision_Monster_Object(this, &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL), m_curState);
		//if(m_preState == m_curState)
			CCollision_Manager::Collision_Monster_Sensor(m_monsterSensorRect, &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), m_curState);




		if (CollisionObject != OBJ_WALL/* && CollisionWater != OBJ_WALL*/)
		{
			switch (m_curState)
			{
			case WALK_LEFT:
				m_info.pos.x -= m_playerableInfo.speed;
				if (m_monsterIndex == 1)
					m_stateKey = L"Monster1_Left";
				else
					m_stateKey = L"Monster2_Left";
				break;

			case WALK_RIGHT:
				m_info.pos.x += m_playerableInfo.speed;
				if (m_monsterIndex == 1)
					m_stateKey = L"Monster1_Right";
				else
					m_stateKey = L"Monster2_Right";
				break;

			case WALK_UP:
				m_info.pos.y -= m_playerableInfo.speed;
				if (m_monsterIndex == 1)
					m_stateKey = L"Monster1_Up";
				else
					m_stateKey = L"Monster2_Up";
				break;

			case WALK_DOWN:
				m_info.pos.y += m_playerableInfo.speed;
				if (m_monsterIndex == 1)
					m_stateKey = L"Monster1_Down";
				else
					m_stateKey = L"Monster2_Down";
				break;
			default:
				break;
			}
		}
	}
	//else
	//{
	//	Change_CurrentState();
	//}
}

void CMonster::Late_Update_GameObject()
{
	Set_Rect();
	Set_SeneorRect();
	FrameMove(0.7f * m_playerableInfo.speed * 2);
}

void CMonster::Render_GameObject()
{
	const Texture_Info* pTexInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(m_objectKey,m_stateKey, m_frame.frameStart);
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




	D3DXVECTOR2 vLineSensorLeft[5]{ {(float)m_monsterSensorRect[0].left, (float)m_monsterSensorRect[0].top}, {(float)m_monsterSensorRect[0].right, (float)m_monsterSensorRect[0].top},{(float)m_monsterSensorRect[0].right, (float)m_monsterSensorRect[0].bottom },{ (float)m_monsterSensorRect[0].left, (float)m_monsterSensorRect[0].bottom },{ (float)m_monsterSensorRect[0].left, (float)m_monsterSensorRect[0].top }, };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLineSensorLeft, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

	D3DXVECTOR2 vLineSensorRight[5]{ {(float)m_monsterSensorRect[1].left, (float)m_monsterSensorRect[1].top}, {(float)m_monsterSensorRect[1].right, (float)m_monsterSensorRect[1].top},{(float)m_monsterSensorRect[1].right, (float)m_monsterSensorRect[1].bottom },{ (float)m_monsterSensorRect[1].left, (float)m_monsterSensorRect[1].bottom },{ (float)m_monsterSensorRect[1].left, (float)m_monsterSensorRect[1].top }, };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLineSensorRight, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

	D3DXVECTOR2 vLineSensorUp[5]{ {(float)m_monsterSensorRect[2].left, (float)m_monsterSensorRect[2].top}, {(float)m_monsterSensorRect[2].right, (float)m_monsterSensorRect[2].top},{(float)m_monsterSensorRect[2].right, (float)m_monsterSensorRect[2].bottom },{ (float)m_monsterSensorRect[2].left, (float)m_monsterSensorRect[2].bottom },{ (float)m_monsterSensorRect[2].left, (float)m_monsterSensorRect[2].top }, };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLineSensorUp, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

	D3DXVECTOR2 vLineSensorDown[5]{ {(float)m_monsterSensorRect[3].left, (float)m_monsterSensorRect[3].top}, {(float)m_monsterSensorRect[3].right, (float)m_monsterSensorRect[3].top},{(float)m_monsterSensorRect[3].right, (float)m_monsterSensorRect[3].bottom },{ (float)m_monsterSensorRect[3].left, (float)m_monsterSensorRect[3].bottom },{ (float)m_monsterSensorRect[3].left, (float)m_monsterSensorRect[3].top }, };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLineSensorDown, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);*/

	//const Texture_Info* pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Monster2", L"Monster2_Down",m_frame.frameStart);
	//if (nullptr == pTexInfo2)
	//	return;
	//D3DXMATRIX matScale2, matTrans2, matWorld2;
	//D3DXMatrixScaling(&matScale2, m_info.size.x, m_info.size.y, 0.f);
	//D3DXMatrixTranslation(&matTrans2, m_info.pos.x, m_info.pos.y + 400, 0.f);
	//matWorld2 = matScale2 * matTrans2;
	////float fCenterX = pTexInfo->imageInfo.Width >> 1;
	////float fCenterY = pTexInfo->imageInfo.Height >> 1;

	//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld2);
	//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->texture, nullptr, &D3DXVECTOR3(0/*fCenterX, fCenterY*/, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMonster::Release_GameObject()
{
}

void CMonster::Set_Rect()
{
	m_player_centerX = m_info.pos.x + 60;// m_monsterSizeX/2;
	m_player_centerY = m_info.pos.y + 60;// m_monsterSizeY/2;

	m_rect.left = m_info.pos.x;// -TILECX * expansionSize / 2;
	m_rect.top = m_info.pos.y;// (m_info.pos.y + m_monsterSizeX) - (TILECY * expansionSize);
	m_rect.right = m_info.pos.x + TILECX * expansionSize; //m_player_centerX + TILECX * expansionSize / 2;
	m_rect.bottom = m_info.pos.y + TILECY*expansionSize;// m_info.pos.y + m_monsterSizeY;


	/*m_rect.left = m_player_centerX - TILECX * expansionSize / 2;
	m_rect.top = (m_info.pos.y + m_monsterSizeX) - (TILECY * expansionSize);
	m_rect.right = m_player_centerX + TILECX * expansionSize / 2;
	m_rect.bottom = m_info.pos.y + m_monsterSizeY;*/



	indexX = ((m_rect.left + m_rect.right - STARTX) / 2) / (TILECX * expansionSize);
	indexY = (m_rect.bottom - STARTY - 38) / (TILECY * expansionSize);

	m_LocationIndex = indexX + indexY * TILEX;


	/*m_monsterSensorRect.left = m_player_centerX - TILECX * expansionSize / 2 - TILECX * expansionSize;
	m_monsterSensorRect.top = (m_info.pos.y + m_monsterSizeX) - (TILECY * expansionSize) + -TILECX * expansionSize;
	m_monsterSensorRect.right = m_player_centerX + TILECX * expansionSize / 2 + TILECX * expansionSize;
	m_monsterSensorRect.bottom = m_info.pos.y + m_monsterSizeY + expansionSize * TILECX;*/

}

void CMonster::Set_SeneorRect()
{
	// 왼쪽
	m_monsterSensorRect[0].left = m_info.pos.x - TILECX * expansionSize;
	m_monsterSensorRect[0].top = m_info.pos.y;
	m_monsterSensorRect[0].right = m_info.pos.x;
	m_monsterSensorRect[0].bottom = m_info.pos.y + TILECY * expansionSize;

	// 오른쪽
	m_monsterSensorRect[1].left = m_info.pos.x + TILECX * expansionSize;
	m_monsterSensorRect[1].top = m_info.pos.y;
	m_monsterSensorRect[1].right = m_info.pos.x + TILECX * expansionSize * 2;
	m_monsterSensorRect[1].bottom = m_info.pos.y + TILECY * expansionSize;


	// 위쪽
	m_monsterSensorRect[2].left = m_info.pos.x;
	m_monsterSensorRect[2].top = m_info.pos.y - TILECY * expansionSize;
	m_monsterSensorRect[2].right = m_info.pos.x + TILECX * expansionSize;
	m_monsterSensorRect[2].bottom = m_info.pos.y;


	// 아래쪽
	m_monsterSensorRect[3].left = m_info.pos.x;
	m_monsterSensorRect[3].top = m_info.pos.y + TILECY * expansionSize;
	m_monsterSensorRect[3].right = m_info.pos.x + TILECX * expansionSize;
	m_monsterSensorRect[3].bottom = m_info.pos.y + TILECY * expansionSize * 2;


}

void CMonster::Change_CurrentState()
{
	switch (m_curState)
	{
	case WALK_LEFT:
		m_curState = WALK_RIGHT;
		break;

	case WALK_RIGHT:
		m_curState = WALK_LEFT;
		break;

	case WALK_UP:
		m_curState = WALK_DOWN;
		break;

	case WALK_DOWN:
		m_curState = WALK_UP;
		break;
	default:
		break;
	}
}
