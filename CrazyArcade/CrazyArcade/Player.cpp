#include "framework.h"
#include "Player.h"
#include "Collision_Manager.h"
#include "GameObject_Manager.h"
#include "WaterBall.h"
#include "Terrain.h"
#include "TimerWaterBall.h"
#include "Shield.h"

CPlayer::CPlayer()
	: m_startIndex(0.f)
	, m_isTrapped(false)
	, m_animCheck(false)
	, m_itemUseable(false)
	, m_waterBallNumber(0)
	, m_isUseShield(false)
	, m_TimerwaterBallNum(3)
{

}

CPlayer::~CPlayer()
{
	Release_GameObject();
}

void CPlayer::FrameMove(float speed)
{
	if (m_isTrapped || m_isDeadAnim)
		speed = 0.3f;
	m_frame.frameStart += m_frame.frameEnd * CTime_Manager::Get_Instance()->Get_DeltaTime() * speed;
	if (m_frame.frameEnd < m_frame.frameStart)
	{
		m_frame.frameStart = m_startIndex;
		if (m_isDeadAnim)
			m_dead = true;
		if (m_isTrapped)
		{
			m_isDeadAnim = true;
		}
	}
}

void CPlayer::PlayerActrion()
{
		// »óÇÏÁÂ¿ì
		int CollisionObject = CCollision_Manager::Collision_Player_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);
		int CollisionWater = CCollision_Manager::Collision_Player_WaterBall(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL), m_curState);
		int CollisionMonster = CCollision_Manager::Collision_Player_Monster(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::MONSTER));
		if (m_isDeadAnim &&!m_animCheck)
		{
			m_stateKey = L"Die";
			m_moveLock = true;
			m_frame.frameStart = 0.f;
			m_frame.frameEnd = 4.f;
			m_animCheck = true;
			CSoundMgr::Get_Instance()->PlaySound(L"PlayerDie.wav", CSoundMgr::PLAYER);
		}
		if (m_isTrapped && !m_isDeadAnim)
		{
			m_stateKey = L"Trapped";
			//m_playerableInfo.speed = m_saveSpeed;
			m_playerableInfo.speed = 0.1f;
			m_frame.frameEnd = 4.f;
		}
		//if(m_isTrapped && m_playerableInfo.isRevival)
		//{
		//	m_playerableInfo.speed = m_saveSpeed;
		//	m_isTrapped = false;
		//	m_playerableInfo.isRevival = false;
		//}

		if (!m_isDeadAnim)
		{
			if (CKey_Manager::Get_Instance()->Key_Down(KEY_Q))
				Use_Item();

			if (m_curState == CHARACTER_STATE::WALK_LEFT)
			{
				if (!m_moveLock)
					if (CollisionObject == OBJ_NONE && CollisionWater != OBJ_WALL)
						m_info.pos.x -= m_playerableInfo.speed;
				/*if (m_info.pos.x  >= 0)
					m_info.pos.x -= m_speed;*/
				if (!m_isTrapped)
				{
					if (!m_playerableInfo.isRide)
						m_stateKey = L"WalkLeft";
					else
						m_stateKey = L"RideLeft";
				}
			}
			if (m_curState == CHARACTER_STATE::WALK_RIGHT)
			{
				//int result = CCollision_Manager::Collision_Player_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);

				if (!m_moveLock)
					if (CollisionObject == OBJ_NONE && CollisionWater != OBJ_WALL)
						m_info.pos.x += m_playerableInfo.speed;
				/*if (m_info.pos.x <= TILECX * 1.5f * TILEX  - 75)
					m_info.pos.x += m_speed;*/
				if (!m_isTrapped)
				{
					if (!m_playerableInfo.isRide)
						m_stateKey = L"WalkRight";
					else
						m_stateKey = L"RideRight";
				}
			}
			if (m_curState == CHARACTER_STATE::WALK_UP)
			{
				//int result = CCollision_Manager::Collision_Player_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);

				if (!m_moveLock)
					if (CollisionObject == OBJ_NONE && CollisionWater != OBJ_WALL)
						m_info.pos.y -= m_playerableInfo.speed;
				/*	if (m_info.pos.y >=  m_speed)
						m_info.pos.y -= m_speed;*/
				if (!m_isTrapped)
				{
					if (!m_playerableInfo.isRide)
						m_stateKey = L"WalkUp";
					else
						m_stateKey = L"RideUp";
				}
			}
			if (m_curState == CHARACTER_STATE::WALK_DOWN)
			{
				//int result = CCollision_Manager::Collision_Player_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);

				if (!m_moveLock)
					if (CollisionObject == OBJ_NONE && CollisionWater != OBJ_WALL)
						m_info.pos.y += m_playerableInfo.speed;
				/*if (m_info.pos.y <= TILECY * 1.5f * TILEY - 85)
					m_info.pos.y += m_speed;*/
				if (!m_isTrapped)
				{
					if (!m_playerableInfo.isRide)
						m_stateKey = L"WalkDown";
					else
						m_stateKey = L"RideDown";
				}
			}

			// ½ºÅÄµå
			if (m_curState == CHARACTER_STATE::STAND)
			{
				if (!m_isTrapped && !m_isDeadAnim)
				{
					if (!m_playerableInfo.isRide)
					{
						m_stateKey = L"Stand";
						switch (m_preState)
						{
						case STAND:
							break;
						case WALK_LEFT:
							m_startIndex = 1.f;
							break;
						case WALK_RIGHT:
							m_startIndex = 2.f;
							break;
						case WALK_UP:
							m_startIndex = 3.f;
							break;
						case WALK_DOWN:
						case TRAPPED:
						case REVIVAL:
							m_startIndex = 0.f;
							break;
						default:
							break;
						}
						m_frame.frameEnd = 0.f;
					}
				}
			}
			else
			{
				m_frame.frameEnd = 4.f;
				m_startIndex = 0.f;
			}
		}

}

void CPlayer::Use_WaterBall()
{
	bool isTimerWaterBallExist = false;
	if (!m_isTrapped && !m_isDeadAnim)
	{
		auto & temp = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);
		bool isWaterBallExist = false;
		for (auto waterBall : temp)
		{
			auto waterBallTemp = dynamic_cast<CWaterBall*>(waterBall);
			if (waterBallTemp->Get_isTimerWaterBall())
			{
				waterBallTemp->Set_Dead(true);
				isTimerWaterBallExist = true;
				continue;
			}

			if (waterBall->Get_LocationIndex() == m_LocationIndex)
				isWaterBallExist = true;
		}
		if (!isTimerWaterBallExist && !isWaterBallExist && (m_playerableInfo.WaterBallCurrent < m_playerableInfo.WaterBallCurrentMax))
		{
			CSoundMgr::Get_Instance()->PlaySound(L"WaterBall.mp3", CSoundMgr::PLAYER);

			m_playerableInfo.WaterBallCurrent++;
			CWaterBall * waterBall = new CWaterBall(m_LocationIndex);
			waterBall->Set_UniqueNumber(m_waterBallNumber);
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATERBALL, waterBall);
			m_waterBallNumber++;
		}
	}
}

//
//void CPlayer::Set_PlayerIndex()
//{
//	float player_centerX = m_info.pos.x + m_playerSize[0] / 2;
//	float player_centerY = m_info.pos.y + m_playerSize[1] / 2 + 15;
//
//	int indexX = (player_centerX - STARTX) / (TILECX * 1.5);
//	int indexY = (player_centerY - STARTY) / (TILECY * 1.5);
//
//	m_LocationIndex = indexX + indexY * TILEX;
//}

void CPlayer::Set_Rect()
{
		m_player_centerX = m_info.pos.x + m_playerSize[0] / 2;
		m_player_centerY = m_info.pos.y + m_playerSize[1] / 2;

		m_rect.left = m_player_centerX - TILECX * expansionSize / 2;
		m_rect.top = (m_info.pos.y + m_playerSize[1]) - (TILECY * expansionSize);
		m_rect.right = m_player_centerX + TILECX * expansionSize / 2;
		m_rect.bottom = m_info.pos.y + m_playerSize[1];

		indexX = ((m_rect.left + m_rect.right - STARTX) / 2) / (TILECX * expansionSize);
		indexY = (m_rect.bottom - STARTY - 38) / (TILECY * expansionSize);

		m_LocationIndex = indexX + indexY * TILEX;
}

void CPlayer::Auto_Item(Item_Info eatItem , Object_Info posInfo)
{
	if (eatItem.isKick)
		m_playerableInfo.isKick = true;
	if (eatItem.isRide)
	{
		if(!m_playerableInfo.isRide)
			m_info.pos.y -= 17 * expansionSize;

		m_playerableInfo.isRide = true;


		if (m_curState == CHARACTER_STATE::WALK_LEFT)
			m_stateKey = L"RideLeft";

		if (m_curState == CHARACTER_STATE::WALK_RIGHT)
			m_stateKey = L"RideRight";

		if (m_curState == CHARACTER_STATE::WALK_UP)
			m_stateKey = L"RideUp";

		if (m_curState == CHARACTER_STATE::WALK_DOWN)
			m_stateKey = L"RideDown";

	}

	if (eatItem.isSpeedUp)
	{
		m_playerableInfo.speed+=1.f;
		m_saveSpeed += 1.f;
		if (m_saveSpeed > m_playerableInfo.speedMax)
		{
			m_playerableInfo.speed = m_playerableInfo.speedMax;
			m_saveSpeed = m_playerableInfo.speedMax;
		}
	}

	if (eatItem.isWaterBall)
	{
		m_playerableInfo.WaterBallCurrentMax++;
		if (m_playerableInfo.WaterBallCurrentMax > m_playerableInfo.WaterBallMax)
			m_playerableInfo.WaterBallCurrentMax = m_playerableInfo.WaterBallMax;
	}
	if (eatItem.isWaterLengthUp)
	{
		m_playerableInfo.WaterLength++;
		if (m_playerableInfo.WaterLength > m_playerableInfo.WaterLengthMax)
			m_playerableInfo.WaterLength = m_playerableInfo.WaterLengthMax;
	}
}

void CPlayer::Manual_Item(Item_Info eatItem)
{
	m_haveItem = eatItem;
	m_itemUseable = true;
	//if (eatItem.isRevival)
	//{
	//	m_playerableInfo.isRevival = true;
	//	m_playerableInfo.isShield = false;
	//}


	//if (eatItem.isShield)
	//{
	//	m_playerableInfo.isKick = true;
	//	m_playerableInfo.isRevival = false;
	//}
}

void CPlayer::IsRide()
{
	if (m_playerableInfo.isRide)
	{
		m_playerSize[0] = 56 * expansionSize;
		m_playerSize[1] = 77 * expansionSize;
		m_playerableInfo.speed = 2;
	}
	else if(!m_playerableInfo.isRide && m_isInvincibility)
	{
		m_playerSize[0] = 56 * expansionSize;
		m_playerSize[1] = 60 * expansionSize;
		m_playerableInfo.speed = m_saveSpeed;
	}
	else if (!m_playerableInfo.isRide && !m_isTrapped)
	{
		m_playerSize[0] = 56 * expansionSize;
		m_playerSize[1] = 60 * expansionSize;
		m_saveSpeed = m_playerableInfo.speed;
	}
}


void CPlayer::Use_Item()
{
	if(m_itemUseable)
	{
		if (m_haveItem.isRevival && m_isTrapped)
		{
			m_stateKey = L"WalkDown";
			m_playerableInfo.speed = m_saveSpeed;
			m_isTrapped = false;
			m_itemUseable = false;
			CSoundMgr::Get_Instance()->PlaySound(L"NeedleUse.wav", CSoundMgr::PLAYER);
		}


		if (m_haveItem.drawID == 10 && !m_isTrapped && !m_isDeadAnim)
		{
			auto & temp = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);

			bool isWaterBallExist = false;
			for (auto waterBall : temp)
			{
				if (waterBall->Get_LocationIndex() == m_LocationIndex)
					isWaterBallExist = true;
			}

			if (!isWaterBallExist && (m_playerableInfo.WaterBallCurrent < m_playerableInfo.WaterBallCurrentMax))
			{
				CTimerWaterBall * timerWaterBall = new CTimerWaterBall(m_LocationIndex);

				timerWaterBall->Set_UniqueNumber(m_waterBallNumber);
				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATERBALL, timerWaterBall);
				m_waterBallNumber++;
				m_TimerwaterBallNum--;

				if (m_TimerwaterBallNum == 0)
				{
					m_itemUseable = false;
					m_TimerwaterBallNum = 5;
				}
			}

		}

		// ½¯µå
		if (m_haveItem.drawID == 8 && !m_isTrapped && !m_isDeadAnim)
		{
			CShield * timerWaterBall = new CShield(m_info);

			//timerWaterBall->Set_UniqueNumber(m_waterBallNumber);
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::USING_ITEM, timerWaterBall);
			//m_waterBallNumber++;
			m_itemUseable = false;
			m_isUseShield = true;
		}

	}
}

HRESULT CPlayer::Ready_GameObject()
{
	auto respawnTile = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::RESPAWN_TILE);
	int respawnSize = respawnTile.size();
	list<CGameObject*>::iterator iter = respawnTile.begin();
	iter++;
	iter++;
	iter++;
	//m_info.pos.x = dynamic_cast<CTerrain*>(*iter)->Get_Terrain_Info().pos.x - 10;
	//m_info.pos.y = dynamic_cast<CTerrain*>(*iter)->Get_Terrain_Info().pos.y - 30;
	m_info.pos.x = 270;//dynamic_cast<CTerrain*>(*iter)->Get_Terrain_Info().pos.x;
	m_info.pos.y = 390;//dynamic_cast<CTerrain*>(*iter)->Get_Terrain_Info().pos.y -90;
	m_info.pos.z = 0.0f;
	m_info.dir = { 1.f,1.f,0.f };
	m_info.size = { expansionSize,expansionSize,0.f };
	m_frame = { 0.f,4.f };
	m_playerSize[0] = 56 * expansionSize;
	m_playerSize[1] = 60 * expansionSize;

	m_objectKey = L"Cappi";// (L"Cappi")
	m_stateKey = L"Stand";// (L"Stand")

	m_playerableInfo.isKick = false;
	m_playerableInfo.isRevival = false;
	m_playerableInfo.isRide = false;
	m_playerableInfo.isShield = false;
	m_playerableInfo.speed = 1.5;
	m_playerableInfo.speedMax = 4;
	m_saveSpeed = m_playerableInfo.speed;
	m_playerableInfo.WaterBallCurrent = 0;
	m_playerableInfo.WaterBallCurrentMax = 2;
	m_playerableInfo.WaterBallMax = 8;
	m_playerableInfo.WaterLength = 2;
	m_playerableInfo.WaterLengthMax = 6;

	m_haveItem.drawID = -1;

	return S_OK;
}

int CPlayer::Update_GameObject()
{
	if (m_dead)
	{
		m_playerableInfo.speed = m_saveSpeed;
		CGameObject_Manager::Get_Instance()->SavePlayerData();
		return OBJ_DEAD;
	}

	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_LEFT))
		m_curState = CHARACTER_STATE::WALK_LEFT;
	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_RIGHT))
		m_curState = CHARACTER_STATE::WALK_RIGHT;

	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_UP))
		m_curState = CHARACTER_STATE::WALK_UP;
	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_DOWN))
		m_curState = CHARACTER_STATE::WALK_DOWN;

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_SPACE))
		Use_WaterBall();

	if (!CKey_Manager::Get_Instance()->Key_Pressing(KEY_LEFT) && !CKey_Manager::Get_Instance()->Key_Pressing(KEY_RIGHT) && !CKey_Manager::Get_Instance()->Key_Pressing(KEY_UP) && !CKey_Manager::Get_Instance()->Key_Pressing(KEY_DOWN))
	{
		m_preState = m_curState;
		m_curState = CHARACTER_STATE::STAND;
	}

	return 0;
}

void CPlayer::Late_Update_GameObject()
{
	Set_PlayerIndex();
	PlayerActrion();
	Set_Rect();
	IsRide();
	Set_InvincibilityCurrentTime();
	FrameMove(0.7f * m_playerableInfo.speed);
}

void CPlayer::Render_GameObject()
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
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->texture, nullptr, &D3DXVECTOR3(0/*fCenterX, fCenterY*/,0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//CGraphic_Device::Get_Instance()->Get_Sprite()->End();

	//D3DXVECTOR2 vLine[5]{ {(float)m_rect.left, (float)m_rect.top}, {(float)m_rect.right, (float)m_rect.top},{(float)m_rect.right, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.top }, };
	//CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

	//CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

	if (m_itemUseable)
	{
		const Texture_Info* itemTexInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Item", L"ItemObj", m_haveItem.drawID);
		if (nullptr == itemTexInfo)
			return;
		D3DXMATRIX matScale2, matTrans2, matWorld2;
		D3DXMatrixScaling(&matScale2, m_info.size.x, m_info.size.y, 0.f);
		D3DXMatrixTranslation(&matTrans2, 1010, 730, 0.f);
		matWorld2 = matScale2 * matTrans2;
		//float fCenterX = pTexInfo->imageInfo.Width >> 1;
		//float fCenterY = pTexInfo->imageInfo.Height >> 1;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld2);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(itemTexInfo->texture, nullptr, &D3DXVECTOR3(0/*fCenterX, fCenterY*/, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


}

void CPlayer::Release_GameObject()
{
}
