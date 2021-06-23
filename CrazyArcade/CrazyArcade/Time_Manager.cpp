#include "framework.h"
#include "Time_Manager.h"

IMPLEMENT_SINGLETON(CTime_Manager)

CTime_Manager::CTime_Manager()
	:m_deltaTime(0.f)
	,m_beginTime({})
	,m_endTime({})
	,m_cpuTick({})
{

}

CTime_Manager::~CTime_Manager()
{
}

void CTime_Manager::Ready_Time_Manager()
{
	QueryPerformanceFrequency(&m_cpuTick);
	QueryPerformanceCounter(&m_beginTime);
	QueryPerformanceCounter(&m_endTime);
}

void CTime_Manager::Update_Time_Manager()
{
	QueryPerformanceCounter(&m_endTime);
	QueryPerformanceFrequency(&m_cpuTick);

	m_deltaTime = float(m_endTime.QuadPart - m_beginTime.QuadPart) / m_cpuTick.QuadPart;
	m_beginTime = m_endTime;
}
