#pragma once
class CTime_Manager
{
	DECLARE_SINGLETON(CTime_Manager)

private:
	CTime_Manager();
	~CTime_Manager();

public:
	float Get_DeltaTime() { return m_deltaTime; }
public:
	void Ready_Time_Manager();
	void Update_Time_Manager();

private:
	LARGE_INTEGER m_beginTime;
	LARGE_INTEGER m_endTime;
	LARGE_INTEGER m_cpuTick;
	float m_deltaTime;
};

