#include "framework.h"
#include "GameObject.h"

CGameObject::CGameObject()
	: m_info({})
	, m_curState(CHARACTER_STATE::STAND)
	, m_preState(CHARACTER_STATE::STAND)
	, m_playerableInfo({})
	, m_frame({})
	, m_player_centerX(0)
	, m_player_centerY(0)
	, m_moveLock(false)
	, m_dead(false)
{
}


CGameObject::~CGameObject()
{
}

int CGameObject::Get_PlayerIndex()
{
	return -1;
}

void CGameObject::Set_PlayerIndex()
{
}

void CGameObject::FrameMove(float speed)
{
	m_frame.frameStart += m_frame.frameEnd * CTime_Manager::Get_Instance()->Get_DeltaTime() * speed;
	if (m_frame.frameEnd < m_frame.frameStart)
		m_frame.frameStart = 0;
}

void CGameObject::Set_Rect()
{
	m_rect.left = m_info.pos.x;
	m_rect.top = m_info.pos.y;
	m_rect.right = m_info.pos.x + TILECX * expansionSize;
	m_rect.bottom = m_info.pos.y + TILECY * expansionSize;
}
