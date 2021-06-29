#include "framework.h"
#include "Scene_Manager.h"
#include "Loading.h"

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
	m_nextScene = nextScene;
	if (m_nextScene != nextScene)
	{
		Safe_Delete(m_scene);
		switch (m_nextScene)
		{
		case SCENE_ID::LOADING:
			m_scene = new CLoading;
			break;
		case  SCENE_ID::MAIN:
			m_pScene = new CStage;
			break;
		case CScene_Manager::END:
			break;
		default:
			break;
		}
		if (FAILED(m_pScene->Ready_Scene()))
		{
			Safe_Delete(m_pScene);
			return E_FAIL;
		}

		m_eCurScene = m_eNextScene;
	}
	return S_OK;
}

void CScene_Manager::Update_Scene_Manager()
{
	m_scene->Update_Scene();
}

void CScene_Manager::Render_Scene_Manager()
{
	m_scene->Render_Scene();
}
void CScene_Manager::Release_Scene_Manager()
{
	Safe_Delete(m_scene);
}
