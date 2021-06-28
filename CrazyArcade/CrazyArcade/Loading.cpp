#include "framework.h"
#include "Loading.h"
#include "Scene_Manager.h"


CLoading::CLoading()
{
}

CLoading::~CLoading()
{
}

unsigned  CLoading::ImageLoading(LPVOID pVoid)
{
	CLoading* pLoading = (CLoading*)pVoid;
	EnterCriticalSection(&pLoading->m_criticalSection);
	//TileTexture
	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_MULTI,
		L"../Resource/Tile/Tile%d.png",
		L"Terrain", L"Tile", 23)))
		return E_FAIL;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_MULTI,
		L"../Resource/Wall/Obj%d.png",
		L"Wall", L"WallObj", 17)))
		return E_FAIL;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_MULTI,
		L"../Resource/Box/Box%d.png",
		L"Box", L"BoxObj", 12)))
		return E_FAIL;

	////Player-Attack Texture 
	//if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
	//	L"../Texture/Stage/Player/Attack/AKIHA_AKI01_00%d.png", L"Player", L"Attack", 6)))
	//	return E_FAIL;

	////Player-Dash Texture 
	//if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
	//	L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png", L"Player", L"Dash", 11)))
	//	return E_FAIL;

	////Player-Stand Texture 
	//if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
	//	L"../Texture/Stage/Player/Stand/AKIHA_AKI00_00%d.png", L"Player", L"Stand", 12)))
	//	return E_FAIL;
	CTexture_Manager::Get_Instance()->Set_LoadingList(L"로딩 완료!!");
	LeaveCriticalSection(&pLoading->m_criticalSection);
	return 0;
}

HRESULT CLoading::Ready_Scene()
{
	InitializeCriticalSection(&m_criticalSection);
	m_thread = (HANDLE)_beginthreadex(nullptr, 0, ImageLoading, this, 0, nullptr);
	if (nullptr == m_thread)
		return E_FAIL;
	return S_OK;
}

void CLoading::Update_Scene()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		WaitForSingleObject(m_thread, INFINITE);

		DeleteCriticalSection(&m_criticalSection);
		CloseHandle(m_thread);
		CScene_Manager::Get_Instance()->Change_Scene_Manager(SCENE_ID::STAGE);
	}
}

void CLoading::Render_Scene()
{
	D3DXMATRIX matTrans;
	const Texture_Info* pTexInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Loading");

	if (nullptr == pTexInfo)
		return;
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	float fCenterX = pTexInfo->imageInfo.Width >> 1;
	float fCenterY = pTexInfo->imageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->texture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	wstring wstrLoadingList = CTexture_Manager::Get_Instance()->Get_LoadingList();
	D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
		wstrLoadingList.c_str(),
		wstrLoadingList.length(),
		nullptr,
		0,
		D3DCOLOR_ARGB(255, 0, 0, 0));
}

void CLoading::Release_Scene()
{
}
