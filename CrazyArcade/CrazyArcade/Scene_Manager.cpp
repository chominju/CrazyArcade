#include "framework.h"
#include "Scene_Manager.h"

IMPLEMENT_SINGLETON(CScene_Manager)

CScene_Manager::CScene_Manager()
	: m_currentScene(SCENE_ID::END)
	, m_nextScene(SCENE_ID::END)
	, m_scene(nullptr)
{

}

CScene_Manager::~CScene_Manager()
{
	Release_Scene_Manager();
}

HRESULT CScene_Manager::Change_Scene_Manager(SCENE_ID nextScene)
{
	return E_NOTIMPL;
}

void CScene_Manager::Update_Scene_Manager()
{
}

void CScene_Manager::Render_Scene_Manager()
{
}

void CScene_Manager::Release_Scene_Manager()
{
}
