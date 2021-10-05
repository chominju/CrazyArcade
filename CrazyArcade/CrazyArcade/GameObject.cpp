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
	, m_pushTime(0.f)
	, m_isPushed(false)
	, m_finishIndex(-1)
	, m_renderID(RENDER_ID::RENDER_OBJECT)
	, m_saveSpeed(0.f)
	, m_isDeadAnim(false)
	, m_isDeadAnimFin(false)
	, m_isBossMonster(false)
	, m_invincibilityCurrnetTime(0.f)
	, m_invincibilityTime(1.f)
	, m_isInvincibility(false)
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
	{
		m_frame.frameStart = 0;

		if (m_isDeadAnim)
			m_isDeadAnimFin = true;
	}
}

void CGameObject::Reset_PlayerInfo()
{
	m_playerableInfo.isKick = false;
	m_playerableInfo.isRevival = false;
	m_playerableInfo.isRide = false;
	m_playerableInfo.isShield = false;
	m_playerableInfo.speed = 1;
	m_saveSpeed = m_playerableInfo.speed;
	m_playerableInfo.WaterBallCurrent = 0;
	m_playerableInfo.WaterBallCurrentMax = 2;
	m_playerableInfo.WaterLength = 2;
}

void CGameObject::Set_InvincibilityCurrentTime()
{
	if (m_isInvincibility)
	{
		m_invincibilityCurrnetTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_invincibilityCurrnetTime > m_invincibilityTime)
		{
			m_isInvincibility = false;
			m_invincibilityCurrnetTime = 0;
		}
	}
}

void CGameObject::Set_Rect()
{
	m_rect.left = m_info.pos.x;
	m_rect.top = m_info.pos.y;
	m_rect.right = m_info.pos.x + TILECX * expansionSize;
	m_rect.bottom = m_info.pos.y + TILECY * expansionSize;
}
