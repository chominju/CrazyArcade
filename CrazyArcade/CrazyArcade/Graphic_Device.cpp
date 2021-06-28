#include "framework.h"
#include "Graphic_Device.h"


IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
{
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
	*/
}


CGraphic_Device::~CGraphic_Device()
{
	Release_Graphic_Device();
}
/*
vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
*/
HRESULT CGraphic_Device::Ready_Graphic_Device(WINDOW_MODE mode)
{
	D3DCAPS9 d3dCaps{};
	//ZeroMemory(&d3dCaps, sizeof(D3DCAPS9)); 

	m_sdk = Direct3DCreate9(D3D_SDK_VERSION);
	DWORD vp = 0;

	// ��� - ���� ���ܵ����� ������ �׷��� ī�带 �����ϰڴٶ�� ��. 
	// ����̽� Ÿ�� - �ִ� ��ġ�� ������ ��� ���� ���̳�. 
	// HAL �̶�� ������ ������ �����ڴ�. 
	// �̰� �׷��� ī�� ��鿡�� �������� �����͸� ���⿡ �־��ְ� 
	// ���ҿ����� �ű⼭ ������ ������ ���� ��. 
	if (FAILED(m_sdk->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
		goto ERR;
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
	*/
	// ���ؽ� ���μ��� - ���� ����� ������ ��ģ�� 

	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
	*/
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	// ���� ��ȯ�� �������� �ϵ����� ó�� �ϰڴ�. 
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	// ���� ��ȯ�� �������� ����Ʈ����� ó�� �ϰڴ�. 

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
	*/
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	//D3DSWAPEFFECT_DISCARD - ����ü�� ����� ����ϰڴ� ��� ��. 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hwnd;
	d3dpp.Windowed = mode;// TRUE �� ��� â���, FALSE�� ��� ��üȭ�� ��� 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
	*/
	// ��ġ�� �����ϱ� ���� �İ�ü �������� ���µ� ����. .. ���� �⺻ �����͸� �����ѰŰ� 
	// ���� ���⼭ ����� ����� �� �������� ����. 
	if (FAILED(m_sdk->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, vp, &d3dpp, &m_device)))
		goto ERR;

	if (FAILED(D3DXCreateSprite(m_device, &m_sprite)))
		goto ERR;

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));
	tFontInfo.Height = 20; // ����
	tFontInfo.Width = 10; // �ʺ�
	tFontInfo.Weight = FW_HEAVY;//�β� �׷� ȫ�β�? ��������������������������
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"����ǹ��� �־�");
	if (FAILED(D3DXCreateFontIndirectW(m_device, &tFontInfo, &m_font)))
		goto ERR;

	if (FAILED(D3DXCreateLine(m_device, &m_line)))
		goto ERR;
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
	*/
	return S_OK;
ERR:
	ERR_MSG(L"Graphic_Device Ready Failed");

	return E_FAIL;
}
/*
vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
*/
void CGraphic_Device::Release_Graphic_Device()
{
	if (m_line)
		m_line->Release();
	if (m_font)
		m_font->Release();

	if (m_sprite)
		m_sprite->Release();

	if (m_device)
		m_device->Release();
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��!�� �־��ָ� ���� ����!!!
	*/
	if (m_sdk)
		m_sdk->Release();
}

void CGraphic_Device::Render_Begin()
{
	m_device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 255, 0), 0.f, 1);
	m_device->BeginScene();
	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
}
/*
vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
*/
void CGraphic_Device::Render_End(HWND hWnd/* = nullptr*/)
{
	m_sprite->End();
	m_device->EndScene();
	m_device->Present(nullptr, nullptr, hWnd, nullptr);
}
/*
vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
D3DCREATE_MULTITHREADED - ��Ƽ ������ ���ٴٴ� ��! �� �־��ָ� ���� ����!!!
*/