#include "framework.h"
#include "GameObject.h"

CGameObject::CGameObject()
	: m_info({})
	, m_curState(CHARACTER_STATE::STAND)
	, m_preState(CHARACTER_STATE::STAND)
	, m_playerableInfo({})
	, m_frame({})
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
