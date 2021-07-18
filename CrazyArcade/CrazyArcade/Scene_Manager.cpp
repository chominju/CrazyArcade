#include "framework.h"
#include "Scene_Manager.h"
#include "Stage.h"
#include "Loading.h"
#include "Monster_Scene1.h"
#include "Monster_Scene2.h"
#include "Boss_Scene.h"

IMPLEMENT_SINGLETON(CScene_Manager)

CScene_Manager::CScene_Manager()
	: m_currentScene(SCENE_ID::SCENE_END)
	, m_nextScene(SCENE_ID::SCENE_END)
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
	if (m_nextScene != m_currentScene)
	{
		Safe_Delete(m_scene);
		switch (m_nextScene)
		{
		case SCENE_ID::LOADING:
			m_scene = new CLoading;
			break;
		case SCENE_ID::STAGE:
			m_scene = new CStage;
			break;
		case SCENE_ID::MONSTER_SCENE1:
			m_scene = new CMonster_Scene1;
			m_scene->Ready_Scene();
			break;
		case SCENE_ID::MONSTER_SCENE2:
			m_scene = new CMonster_Scene2;
			break;
		case SCENE_ID::BOSS_SCENE:
			m_scene = new CBoss_Scene;
			break;
		case SCENE_ID::SCENE_END:
			break;
		default:
			break;
		}

		if (FAILED(m_scene->Ready_Scene()))
		{
			Safe_Delete(m_scene);
			return E_FAIL;
		}

		m_currentScene = m_nextScene;
	}
	return E_NOTIMPL;
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
