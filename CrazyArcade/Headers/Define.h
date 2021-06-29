#pragma once

#define SWORD	0x01
#define BOW		0x02
#define POLE	0x04
#define OBJ_DEAD	1 
#define OBJ_NOEVENT 0

#define DECLARE_SINGLETON(ClassName)				\
public:												\
	static ClassName* Get_Instance()				\
	{												\
		if (nullptr == m_instance)					\
			m_instance = new ClassName;				\
		return m_instance;							\
	}												\
													\
	static void Destroy_Instance()					\
	{												\
		if (m_instance)								\
		{											\
			delete m_instance;						\
			m_instance = nullptr;					\
		}											\
	}												\
													\
	private:										\
		static ClassName* m_instance;				


#define IMPLEMENT_SINGLETON(ClassName)				\
		ClassName*ClassName::m_instance = nullptr;


#ifdef _AFX
#define ERR_MSG(msg) AfxMessageBox(msg)
#else
#define ERR_MSG(msg) MessageBox(nullptr, msg, L"System Error",MB_OK);
#endif // _AFX