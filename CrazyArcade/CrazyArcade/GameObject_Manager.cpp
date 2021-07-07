#include "framework.h"
#include "GameObject_Manager.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Water.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)
CGameObject_Manager::CGameObject_Manager()
{

}

CGameObject_Manager::~CGameObject_Manager()
{
	Release_GameObject_Manager();
}

bool CGameObject_Manager::IsExistObject(int index)
{
	for (auto object : m_listGameObject[OBJECT_ID::OBEJCT])
	{
		auto castObject = dynamic_cast<CTerrain*>(object);
		if (index == castObject->Get_Terrain_Info().index)
		{
			if (castObject->Get_Terrain_Info().isDestroy)
			{
				castObject->Set_Dead(true);
			}
			return true;
		}
	}
	return false;
}

bool CGameObject_Manager::IsExistWater(int index)
{
	for (auto object : m_listGameObject[OBJECT_ID::WATER])
	{
		auto castObject = dynamic_cast<CWater*>(object);
		if (index == castObject->Get_LocationIndex())
		{
			if (castObject->Get_IsBoxDestroy())
				return true;
		}
	}
	return false;
}

HRESULT CGameObject_Manager::Add_GameObject_Manager(OBJECT_ID id, CGameObject * object)
{
	if (id >= OBJECT_ID::OBJECT_END || nullptr == object)
		return E_FAIL;
	m_listGameObject[id].emplace_back(object);
	return S_OK;
}

void CGameObject_Manager::Update_GameObject_Manager()
{
	for (int i = 0; i < OBJECT_ID::OBJECT_END; ++i)
	{
		list<CGameObject*>::iterator iter = m_listGameObject[i].begin();
		for (; iter != m_listGameObject[i].end();)
		{
			int event = (*iter)->Update_GameObject();
			if (OBJ_DEAD == event)
			{
				Safe_Delete(*iter);
				iter = m_listGameObject[i].erase(iter);
			}
			else
				++iter;
		}
		for (auto & gameObject : m_listGameObject[i])
		{
			gameObject->Late_Update_GameObject();
		}
	}
}

void CGameObject_Manager::Render_GameObject_Manager()
{
	for (int i = 0; i < OBJECT_ID::OBJECT_END; ++i)
	{
		if (i == OBJECT_ID::OBEJCT)
		{
			m_listGameObject[OBJECT_ID::OBEJCT].sort([](CGameObject* a, CGameObject* b) {
				return dynamic_cast<CTerrain*>(a)->Get_Terrain_Info().index < dynamic_cast<CTerrain*>(b)->Get_Terrain_Info().index;
			});
		}
		for (auto& gameObject : m_listGameObject[i])
			gameObject->Render_GameObject();
	}
}

void CGameObject_Manager::Release_GameObject_Manager()
{
	for (int i = 0; i < OBJECT_ID::OBJECT_END; ++i)
	{
		for (auto& gameObject : m_listGameObject[i])
			Safe_Delete(gameObject);

		m_listGameObject[i].clear();
	}
}
