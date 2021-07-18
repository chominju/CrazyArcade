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

CGameObject * CGameObject_Manager::Get_Player()
{
	if (m_listGameObject[OBJECT_ID::PLAYER].empty())
		return nullptr;
	return m_listGameObject[OBJECT_ID::PLAYER].front(); 
}

Item_Info CGameObject_Manager::Get_ItemData(BYTE index)
{
	auto iter = m_mapItemInfo.find(index);
	return iter->second;
}

void CGameObject_Manager::Reset_Object(OBJECT_ID id)
{
	for (auto& gameObject : m_listGameObject[id])
		Safe_Delete(gameObject);
	m_listGameObject[id].clear();
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

HRESULT CGameObject_Manager::Add_ItemData(Item_Info & data)
{
	m_mapItemInfo.emplace(data.drawID, data);
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
			if (m_listGameObject[i].empty())
				break;
			RENDER_ID eID = gameObject->Get_RenderID();
			m_listRenderingGameObject[eID].emplace_back(gameObject);
		}
	}
}

void CGameObject_Manager::Render_GameObject_Manager()
{
	/*for (int i = 0; i < OBJECT_ID::OBJECT_END; ++i)
	{
		if (i == OBJECT_ID::OBEJCT)
		{
			m_listGameObject[OBJECT_ID::OBEJCT].sort([](CGameObject* a, CGameObject* b) {
				return dynamic_cast<CTerrain*>(a)->Get_Terrain_Info().index < dynamic_cast<CTerrain*>(b)->Get_Terrain_Info().index;
			});
		}
		for (auto& gameObject : m_listGameObject[i])
			gameObject->Render_GameObject();
	}*/


	for (int i = 0; i < RENDER_ID::RENDER_END; ++i)
	{
		m_listRenderingGameObject[i].sort([](auto& pDst, auto& pSrc)
		{
			//return pDst->Get_Object_Info().pos.y < pSrc->Get_Object_Info().pos.y;
			return pDst->Get_Rect().top < pSrc->Get_Rect().top;
		});
		for (auto& pObject : m_listRenderingGameObject[i])
			pObject->Render_GameObject();
		// 여기서 랜더하기 위한 리스트에 들어가 있는 데이터는 그냥 클리어로만 제거해야한다
		// 왜인지는 알지?? 두개의 리스트에 같은 주소값 들고 있는데 . 
		// 이시키는 단순 그리기 위한 용도로만 쓸거고 
		// 실제 객체를 지워 버리면 ObjectList에 뎅글링 포인터 완성! 
		// 망.
		m_listRenderingGameObject[i].clear();
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
