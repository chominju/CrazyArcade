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

	CLoading load;
	load.Ready_Scene();


	temp = new CTerrain;
	temp->Ready_GameObject();

	return S_OK;
}

void CMainApp::Update_MainApp()
{
}

void CMainApp::Render_MainApp(CFrame_Manager * pFrame_Manager)
{
	temp->Render_GameObject();
}

void CMainApp::Release_MainApp()
{
}
