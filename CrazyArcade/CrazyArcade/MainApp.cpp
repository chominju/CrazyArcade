#include "framework.h"
#include "MainApp.h"
#include "Scene_Manager.h"
#include "Graphic_Device.h"
#include "Frame_Manager.h"
#include "Terrain.h"
#include "Loading.h"

CMainApp::CMainApp()
	: m_graphic_Device(CGraphic_Device::Get_Instance())
	, m_scene_Manager(CScene_Manager::Get_Instance())

{
}

CMainApp::~CMainApp()
{
	Release_MainApp();
}

HRESULT CMainApp::Ready_MainApp()
{
	CTime_Manager::Get_Instance()->Ready_Time_Manager();
	if (FAILED(m_graphic_Device->Ready_Graphic_Device(WINDOW_MODE::MODE_WINDOW)))
		return E_FAIL;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_SINGLE,
		L"../Texture/DayounNim.jpg", L"Loading")))
		return E_FAIL;

	m_scene_Manager->Change_Scene_Manager(SCENE_ID::LOADING);
	return S_OK;
}

void CMainApp::Update_MainApp()
{
	CTime_Manager::Get_Instance()->Update_Time_Manager();
	m_scene_Manager->Update_Scene_Manager();
}

void CMainApp::Render_MainApp(CFrame_Manager * pFrame_Manager)
{
	m_graphic_Device->Render_Begin();
	m_scene_Manager->Render_Scene_Manager();
	pFrame_Manager->Render_Frame_Manager();
	m_graphic_Device->Render_End();
}

void CMainApp::Release_MainApp()
{
	CTime_Manager::Destroy_Instance();
	CTexture_Manager::Destroy_Instance();
	//CGameObject_Manager::Destroy_Instance();
	m_scene_Manager->Destroy_Instance();
	m_graphic_Device->Destroy_Instance();
}
