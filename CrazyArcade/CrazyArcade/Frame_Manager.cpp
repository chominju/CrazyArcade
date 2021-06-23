#include "framework.h"
#include "Frame_Manager.h"
#include "Time_Manager.h"


IMPLEMENT_SINGLETON(CFrame_Manager)
CFrame_Manager::CFrame_Manager()
	: m_szfps(L"")
	, m_fpsTime(0.f)
	, m_fps(0)
{
}

CFrame_Manager::~CFrame_Manager()
{
}

void CFrame_Manager::Ready_Frame_Manager(float fps)
{
	m_spf = 1.f;
	m_deltaTime = 0.f;
	QueryPerformanceCounter(&m_beginTime);
	QueryPerformanceCounter(&m_endTime);
	QueryPerformanceFrequency(&m_cpuTick);
}

void CFrame_Manager::Render_Frame_Manager()
{
	m_fpsTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (1.f <= m_fpsTime)
	{
		swprintf_s(m_szfps, L"FPS : %d", m_fps);
		m_fps = 0;
		m_fpsTime = 0.f;
	}
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 100.f, 100.f, 0.f);

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szfps, lstrlen(m_szfps), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
}

bool CFrame_Manager::Frame_Lock()
{
	QueryPerformanceCounter(&m_endTime);

	m_deltaTime += float(m_endTime.QuadPart - m_beginTime.QuadPart) / m_cpuTick.QuadPart;
	m_beginTime = m_endTime;
	if (m_spf <= m_deltaTime)
	{
		QueryPerformanceFrequency(&m_cpuTick);
		m_deltaTime = 0.f;
		++m_fps;
		return true;
	}
	return false;
}
