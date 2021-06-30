#include "framework.h"
#include "Key_Manager.h"

IMPLEMENT_SINGLETON(CKey_Manager)
CKey_Manager::CKey_Manager()
	: m_key(0)
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
		m_key != KEY_SPACE;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		m_key != KEY_ENTER;
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
