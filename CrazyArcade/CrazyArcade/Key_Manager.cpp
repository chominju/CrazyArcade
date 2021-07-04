#include "framework.h"
#include "Key_Manager.h"

CKey_Manager::CKey_Manager()
	:m_key(0)
	, m_keyDown(0)
	, m_keyUP(0)
{
}


CKey_Manager::~CKey_Manager()
{
}

void CKey_Manager::Update_Key_Manager()
{
	m_key = 0;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_key |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_key |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_key |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_key |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_key |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_key |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_key |= KEY_SPACE;
	if (GetAsyncKeyState('C') & 0x8000)
		m_key |= KEY_C;
	/*if (GetAsyncKeyState('A') & 0x8000)
		m_key |= KEY_A;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_key |= KEY_Q;
	if (GetAsyncKeyState('W') & 0x8000)
		m_key |= KEY_W;
	if (GetAsyncKeyState('E') & 0x8000)
		m_key |= KEY_E;
	if (GetAsyncKeyState('R') & 0x8000)
		m_key |= KEY_R;
	if (GetAsyncKeyState('P') & 0x8000)
		m_key |= KEY_P;
	if (GetAsyncKeyState('I') & 0x8000)
		m_key |= KEY_I;
	if (GetAsyncKeyState('Z') & 0x8000)
		m_key |= KEY_Z;
	if (GetAsyncKeyState(VK_F5) & 0x8000)
		m_key |= KEY_F5;
	if (GetAsyncKeyState(VK_F6) & 0x8000)
		m_key |= KEY_F6;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_key |= KEY_RETURN;
	if (GetAsyncKeyState('1') & 0x8000)
		m_key |= KEY_1;
	if (GetAsyncKeyState('2') & 0x8000)
		m_key |= KEY_2;
	if (GetAsyncKeyState('3') & 0x8000)
		m_key |= KEY_3;
	if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
		m_key |= KEY_NUMPAD0;*/
}

bool CKey_Manager::Key_Up(DWORD key)
{
	if (m_key & key)
	{
		m_keyUP |= key;
		return false;
	}
	else if (m_keyUP & key)
	{
		m_keyUP ^= key;
		return true;
	}
	return false;
}

bool CKey_Manager::Key_Down(DWORD key)
{
	if ((m_key & key) && !(m_keyDown & key))
	{
		m_keyDown |= key;
		return true;
	}
	else if (!(m_key & key) && (m_keyDown & key))
	{
		m_keyDown ^= key;
		return false;
	}
	return false;
}

bool CKey_Manager::Key_Pressing(DWORD key)
{
	if (m_key & key)
		return true;
	return false;
}
