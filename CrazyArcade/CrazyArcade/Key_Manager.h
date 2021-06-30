#pragma once

#define KEY_LBUTTON			0x0001
#define KEY_RBUTTON			0x0002
#define KEY_LEFT			0x0004
#define KEY_RIGHT			0x0008
#define KEY_UP				0x0010
#define KEY_DOWN			0x0020
#define KEY_SPACE			0x0040
#define KEY_ENTER			0x0080



class CKey_Manager
{
	DECLARE_SINGLETON(CKey_Manager)
private:
	explicit CKey_Manager();
	~CKey_Manager();

public:
	void Update_Key_Manager();
	bool Key_Up(DWORD key);
	bool Key_Down(DWORD key);
	bool Key_Pressing(DWORD key);

private:
	DWORD m_key;
	DWORD m_keyDown;
	DWORD m_keyUP;
};

