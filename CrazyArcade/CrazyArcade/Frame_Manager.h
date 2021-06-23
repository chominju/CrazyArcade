#pragma once
class CFrame_Manager
{
	DECLARE_SINGLETON(CFrame_Manager)
public:
	
	CFrame_Manager();
	~CFrame_Manager();

public:
	void Ready_Frame_Manager(float fps);
	void Render_Frame_Manager();
	bool Frame_Lock();

private:
	LARGE_INTEGER m_beginTime;
	LARGE_INTEGER m_endTime;
	LARGE_INTEGER m_cpuTick;
	float m_deltaTime;
	float m_spf;

	TCHAR m_szfps[32];
	float m_fpsTime;

	int m_fps;
};

	