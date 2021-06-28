#include "framework.h"
#include "Graphic_Device.h"


IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
{
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
	*/
}


CGraphic_Device::~CGraphic_Device()
{
	Release_Graphic_Device();
}
/*
vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
*/
HRESULT CGraphic_Device::Ready_Graphic_Device(WINDOW_MODE mode)
{
	D3DCAPS9 d3dCaps{};
	//ZeroMemory(&d3dCaps, sizeof(D3DCAPS9)); 

	m_sdk = Direct3DCreate9(D3D_SDK_VERSION);
	DWORD vp = 0;

	// 어뎁터 - 현재 진단도구에 나열된 그래픽 카드를 조사하겠다라는 뜻. 
	// 디바이스 타입 - 애는 장치의 정보를 어디서 얻어올 것이냐. 
	// HAL 이라는 곳에서 정보를 얻어오겠다. 
	// 이건 그래픽 카드 사들에서 공통적인 데이터를 여기에 넣어주고 
	// 마소에들이 거기서 데이터 가져다 쓰는 거. 
	if (FAILED(m_sdk->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
		goto ERR;
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
	*/
	// 버텍스 프로세싱 - 정점 연산과 조명연산 합친거 

	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
	*/
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	// 정점 변환과 조명연산을 하드웨어에서 처리 하겠다. 
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	// 정점 변환과 조명연산을 소프트웨어에서 처리 하겠다. 

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
	*/
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	//D3DSWAPEFFECT_DISCARD - 스왑체인 방식을 사용하겠다 라는 뜻. 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hwnd;
	d3dpp.Windowed = mode;// TRUE 일 경우 창모드, FALSE일 경우 전체화면 모드 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
	*/
	// 장치를 제어하기 위한 컴객체 생성한적 없는데 나는. .. 저건 기본 데이터만 세팅한거고 
	// 이제 여기서 만들어 줘야지 저 정보들을 토대로. 
	if (FAILED(m_sdk->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, vp, &d3dpp, &m_device)))
		goto ERR;

	if (FAILED(D3DXCreateSprite(m_device, &m_sprite)))
		goto ERR;

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));
	tFontInfo.Height = 20; // 높이
	tFontInfo.Width = 10; // 너비
	tFontInfo.Weight = FW_HEAVY;//두께 그럼 홍두께? ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"배달의민족 주아");
	if (FAILED(D3DXCreateFontIndirectW(m_device, &tFontInfo, &m_font)))
		goto ERR;

	if (FAILED(D3DXCreateLine(m_device, &m_line)))
		goto ERR;
	/*
	vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
	D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
	*/
	return S_OK;
ERR:
	ERR_MSG(L"Graphic_Device Ready Failed");

	return E_FAIL;
}
/*
vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
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
	D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말!안 넣어주면 문제 생김!!!
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
D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
*/
void CGraphic_Device::Render_End(HWND hWnd/* = nullptr*/)
{
	m_sprite->End();
	m_device->EndScene();
	m_device->Present(nullptr, nullptr, hWnd, nullptr);
}
/*
vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | ;
D3DCREATE_MULTITHREADED - 멀티 쓰레드 쓰겟다는 말! 안 넣어주면 문제 생김!!!
*/