#include "framework.h"
#include "Stage.h"
#include "GameObject_Manager.h"
#include "Terrain.h"
#include "Player.h"

CStage::CStage()
	:m_gameObject_Manager(CGameObject_Manager::Get_Instance())
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	/*CGameObject* object = new CTerrain;
	if (FAILED(object->Ready_GameObject()))
		return E_FAIL;
	
	m_gameObject_Manager->Add_GameObject_Manager(OBJECT_ID::SCENE_TILE, object);
	object = new CPlayer;
	if(FAILED(object->Ready_GameObject()))
		return E_FAIL;
	m_gameObject_Manager->Add_GameObject_Manager(OBJECT_ID::PLAYER, object);*/
	return S_OK;
}

void CStage::Update_Scene()
{
	m_gameObject_Manager->Update_GameObject_Manager();
}

void CStage::Render_Scene()
{
	m_gameObject_Manager->Render_GameObject_Manager();
}

void CStage::Release_Scene()
{
	m_gameObject_Manager->Destroy_Instance();
}
