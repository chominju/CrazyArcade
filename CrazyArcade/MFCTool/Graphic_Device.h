#pragma once
class CGraphic_Device
{
	DECLARE_SINGLETON(CGraphic_Device);
private:
	CGraphic_Device();
	~CGraphic_Device();

public:
	LPDIRECT3DDEVICE9 Get_Device() { return m_device; }
	LPD3DXSPRITE Get_Sprite() { return m_sprite; }
	LPD3DXFONT Get_Font() { return m_font; }

public:
	HRESULT Ready_Graphic_Device();
	void Release_Graphic_Device();
public:
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);

private:
	LPDIRECT3D9				m_sdk;
	LPDIRECT3DDEVICE9		m_device;
	LPD3DXSPRITE			m_sprite;
	LPD3DXFONT				m_font;
};

