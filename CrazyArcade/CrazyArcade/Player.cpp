#include "framework.h"
#include "Player.h"
#include "Collision_Manager.h"
#include "GameObject_Manager.h"
#include "WaterBall.h"
#include "Terrain.h"

CPlayer::CPlayer()
	: m_speed(0.f)
	, m_startIndex(0.f)
	, m_objectKey(L"Cappi")
	, m_stateKey(L"Stand")
{

}

CPlayer::~CPlayer()
{
	Release_GameObject();
}

void CPlayer::FrameMove(float speed)
{
	m_frame.frameStart += m_frame.frameEnd * CTime_Manager::Get_Instance()->Get_DeltaTime() * speed;
	if (m_frame.frameEnd < m_frame.frameStart)
		m_frame.frameStart = m_startIndex;
}

void CPlayer::PlayerActrion()
{
	// »óÇÏÁÂ¿ì
	int result = CCollision_Manager::Collision_Player_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);
	
	if (m_curState == CHARACTER_STATE::WALK_LEFT)
	{
		
		if (!m_moveLock)
		if(result == OBJ_NONE)
			m_info.pos.x -= m_speed;
		/*if (m_info.pos.x  >= 0)
			m_info.pos.x -= m_speed;*/
		m_stateKey = L"WalkLeft";
	}
	if (m_curState == CHARACTER_STATE::WALK_RIGHT)
	{
		//int result = CCollision_Manager::Collision_Player_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);

		if (!m_moveLock)
		if (result == OBJ_NONE)
			m_info.pos.x += m_speed;
		/*if (m_info.pos.x <= TILECX * 1.5f * TILEX  - 75)
			m_info.pos.x += m_speed;*/
		m_stateKey = L"WalkRight";
	}
	if (m_curState == CHARACTER_STATE::WALK_UP)
	{
		//int result = CCollision_Manager::Collision_Player_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);

		if (!m_moveLock)
		if (result == OBJ_NONE)
			m_info.pos.y -= m_speed;
	/*	if (m_info.pos.y >=  m_speed)
			m_info.pos.y -= m_speed;*/
		m_stateKey = L"WalkUp";
	}
	if (m_curState == CHARACTER_STATE::WALK_DOWN)
	{
		//int result = CCollision_Manager::Collision_Player_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);

		if (!m_moveLock)
		if (result == OBJ_NONE)
			m_info.pos.y += m_speed;
		/*if (m_info.pos.y <= TILECY * 1.5f * TILEY - 85)
			m_info.pos.y += m_speed;*/
		m_stateKey = L"WalkDown";
	}

	// ½ºÅÄµå
	if (m_curState == CHARACTER_STATE::STAND)
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
	else
	{
		m_frame.frameEnd = 4.f;
		m_startIndex = 0.f;
	}
}

void CPlayer::Use_WaterBall()
{
	auto & temp = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);
	bool isWaterBallExist = false;
	for (auto waterBall : temp)
	{
		if (waterBall->Get_LocationIndex() == m_LocationIndex)
			isWaterBallExist = true;
	}
	if (!isWaterBallExist &&(m_playerableInfo.WaterBallCurrent < m_playerableInfo.WaterBallCurrentMax))
	{
		m_playerableInfo.WaterBallCurrent++;
		CWaterBall * waterBall = new CWaterBall(m_LocationIndex);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATERBALL, waterBall);
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

	m_rect.left = m_player_centerX - TILECX * expansionSize / 2 ;
	m_rect.top = (m_info.pos.y + m_playerSize[1]) - (TILECY * expansionSize) ;
	m_rect.right = m_player_centerX + TILECX * expansionSize / 2 ;
	m_rect.bottom = m_info.pos.y + m_playerSize[1] ;

	indexX = ((m_rect.left + m_rect.right - STARTX)/2) / (TILECX * expansionSize);
	indexY = (m_rect.bottom - STARTY - 38) / (TILECY * expansionSize);

	m_LocationIndex = indexX + indexY * TILEX;
}

HRESULT CPlayer::Ready_GameObject()
{
	auto respawnTile = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::RESPAWN_TILE);
	int respawnSize = respawnTile.size();
	list<CGameObject*>::iterator iter = respawnTile.begin();
	iter++;
	iter++;
	iter++;
	m_info.pos.x = dynamic_cast<CTerrain*>(*iter)->Get_Terrain_Info().pos.x - 10;
	m_info.pos.y = dynamic_cast<CTerrain*>(*iter)->Get_Terrain_Info().pos.y - 30;
	m_info.pos.z = 0.0f;
	m_info.dir = { 1.f,1.f,0.f };
	m_info.size = { expansionSize,expansionSize,0.f };
	m_frame = { 0.f,4.f };
	m_speed = 2.f;
	m_playerSize[0] = 56 * 1.5;
	m_playerSize[1] = 60 * 1.5;

	m_playerableInfo.isKick = false;
	m_playerableInfo.isRevival = false;
	m_playerableInfo.isRide = false;
	m_playerableInfo.isShield = false;
	m_playerableInfo.speed = 2;
	m_playerableInfo.WaterBallCurrent = 0;
	m_playerableInfo.WaterBallCurrentMax = 5;
	m_playerableInfo.WaterBallMax = 8;
	m_playerableInfo.WaterLength = 2;
	m_playerableInfo.WaterLengthMax = 4;

	return S_OK;
}

int CPlayer::Update_GameObject()
{
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
	FrameMove(0.7f * m_speed);
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
}

void CPlayer::Release_GameObject()
{
}
