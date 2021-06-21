#include "pch.h"
#include "framework.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)
CGraphic_Device::CGraphic_Device()
{
}


CGraphic_Device::~CGraphic_Device()
{
	Release_Graphic_Device();
}

HRESULT CGraphic_Device::Ready_Graphic_Device()
{
	D3DCAPS9 d3dCaps{};
	//ZeroMemory(&d3dCaps, sizeof(D3DCAPS9)); 

	m_sdk = Direct3DCreate9(D3D_SDK_VERSION);
	// 嬢紀斗 - 薄仙 遭舘亀姥拭 蟹伸吉 益掘波 朝球研 繕紫馬畏陥虞澗 倶. 
	// 巨郊戚什 展脊 - 蕉澗 舌帖税 舛左研 嬢巨辞 条嬢臣 依戚劃. 
	// HAL 戚虞澗 員拭辞 舛左研 条嬢神畏陥. 
	// 戚闇 益掘波 朝球 紫級拭辞 因搭旋昔 汽戚斗研 食奄拭 隔嬢爽壱 
	// 原社拭級戚 暗奄辞 汽戚斗 亜閃陥 床澗 暗. 
	if (FAILED(m_sdk->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
		goto ERR;

	// 獄努什 覗稽室縮 - 舛繊 尻至引 繕誤尻至 杯庁暗 
	DWORD vp = 0;

	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	// 舛繊 痕発引 繕誤尻至聖 馬球裾嬢拭辞 坦軒 馬畏陥. 
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	// 舛繊 痕発引 繕誤尻至聖 社覗闘裾嬢拭辞 坦軒 馬畏陥. 

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	//D3DSWAPEFFECT_DISCARD - 什尋端昔 号縦聖 紫遂馬畏陥 虞澗 倶. 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hwnd;
	d3dpp.Windowed = TRUE;// TRUE 析 井酔 但乞球, FALSE析 井酔 穿端鉢檎 乞球 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 舌帖研 薦嬢馬奄 是廃 陳梓端 持失廃旋 蒸澗汽 蟹澗. .. 煽闇 奄沙 汽戚斗幻 室特廃暗壱 
	// 戚薦 食奄辞 幻級嬢 操醤走 煽 舛左級聖 塘企稽. 
	if (FAILED(m_sdk->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, vp, &d3dpp, &m_device)))
		goto ERR;

	if (FAILED(D3DXCreateSprite(m_device, &m_sprite)))
		goto ERR;

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));
	tFontInfo.Height = 15; // 株戚
	tFontInfo.Width = 5; // 格搾
	tFontInfo.Weight = FW_LIGHT;//砧臆 益軍 畠砧臆? せせせせせせせせせせせせせ
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"壕含税肯膳 爽焼");
	if (FAILED(D3DXCreateFontIndirectW(m_device, &tFontInfo, &m_font)))
		goto ERR;

	return S_OK;
ERR:
	ERR_MSG(L"Graphic_Device Ready Failed");

	return E_FAIL;
}

void CGraphic_Device::Release_Graphic_Device()
{
	if (m_font)
		m_font->Release();

	if (m_sprite)
		m_sprite->Release();

	if (m_device)
		m_device->Release();
	// 
	if (m_sdk)
		m_sdk->Release();
}

void CGraphic_Device::Render_Begin()
{
	m_device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 255, 0), 0.f, 1);
	m_device->BeginScene();
	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphic_Device::Render_End(HWND hWnd/* = nullptr*/)
{
	m_sprite->End();
	m_device->EndScene();
	m_device->Present(nullptr, nullptr, hWnd, nullptr);
}
