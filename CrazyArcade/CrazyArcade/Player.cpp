#include "framework.h"
#include "Player.h"
#include "Collision_Manager.h"
#include "GameObject_Manager.h"

CPlayer::CPlayer()
	: m_frame({})
	, m_speed(0.f)
	, m_startIndex(0.f)
	, m_objectKey(L"Cappi")
	, m_stateKey(L"Stand")
	, m_preState(CHARACTER_STATE::STAND)
	, m_curState(CHARACTER_STATE::STAND)
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
	if (m_curState == CHARACTER_STATE::WALK_LEFT)
	{
		int result = CCollision_Manager::Collision_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);
		
		if(result == OBJ_NONE)
			m_info.pos.x -= m_speed;
		/*if (m_info.pos.x  >= 0)
			m_info.pos.x -= m_speed;*/
		m_stateKey = L"WalkLeft";
	}
	if (m_curState == CHARACTER_STATE::WALK_RIGHT)
	{
		int result = CCollision_Manager::Collision_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);

		if (result == OBJ_NONE)
			m_info.pos.x += m_speed;
		/*if (m_info.pos.x <= TILECX * 1.5f * TILEX  - 75)
			m_info.pos.x += m_speed;*/
		m_stateKey = L"WalkRight";
	}
	if (m_curState == CHARACTER_STATE::WALK_UP)
	{
		int result = CCollision_Manager::Collision_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);

		if (result == OBJ_NONE)
			m_info.pos.y -= m_speed;
	/*	if (m_info.pos.y >=  m_speed)
			m_info.pos.y -= m_speed;*/
		m_stateKey = L"WalkUp";
	}
	if (m_curState == CHARACTER_STATE::WALK_DOWN)
	{
		int result = CCollision_Manager::Collision_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT), m_curState);

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

HRESULT CPlayer::Ready_GameObject()
{
	m_info.pos = { 100.f,100.f,0.f };
	m_info.dir = { 1.f,1.f,0.f };
	m_info.size = { 1.5f,1.5f,0.f };
	m_frame = { 0.f,4.f };
	m_speed = 4.f;
	m_playerSize[0] = 56 * 1.5;
	m_playerSize[1] = 60 * 1.5;
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

	if (!CKey_Manager::Get_Instance()->Key_Pressing(KEY_LEFT) && !CKey_Manager::Get_Instance()->Key_Pressing(KEY_RIGHT) && !CKey_Manager::Get_Instance()->Key_Pressing(KEY_UP) && !CKey_Manager::Get_Instance()->Key_Pressing(KEY_DOWN))
	{
		m_preState = m_curState;
		m_curState = CHARACTER_STATE::STAND;
	}

	return 0;
}

void CPlayer::Late_Update_GameObject()
{
	PlayerActrion();
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
