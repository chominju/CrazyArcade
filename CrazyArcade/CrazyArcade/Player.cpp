#include "framework.h"
#include "Player.h"

CPlayer::CPlayer()
	: m_frame({})
	, m_speed(0.f)
{
	ZeroMemory(&m_info, sizeof(Object_Info));
}

CPlayer::~CPlayer()
{
	Release_GameObject();
}

void CPlayer::FrameMove(float speed)
{
	m_frame.frameStart += m_frame.frameEnd + CTime_Manager::Get_Instance()->Get_DeltaTime() + speed;
	if (m_frame.frameEnd < m_frame.frameStart)
		m_frame.frameStart = 0.f;
}

void CPlayer::MovePlayer()
{

}

HRESULT CPlayer::Ready_GameObject()
{
	m_info.pos = { 400.f,300.f,0.f };
	m_info.dir = { 1.f,1.f,0.f };
	m_info.size = { 1.5f,1.5f,0.f };
	m_frame = { 0.f,6.f };
	m_speed = 300.f;
	return S_OK;
}

int CPlayer::Update_GameObject()
{
	return 0;
}

void CPlayer::Late_Update_GameObject()
{
	FrameMove(0.5f);
}

void CPlayer::Render_GameObject()
{
}

void CPlayer::Release_GameObject()
{
}
