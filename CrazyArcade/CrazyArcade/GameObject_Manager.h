#pragma once
class CGameObject;
class CGameObject_Manager
{
	DECLARE_SINGLETON(CGameObject_Manager)
private:
	CGameObject_Manager();
	~CGameObject_Manager();

public:
	CGameObject* Get_Tile() { return m_listGameObject[OBJECT_ID::SCENE_TILE].front(); }
	CGameObject* Get_Object() { return m_listGameObject[OBJECT_ID::OBEJCT].front(); }

public:
	HRESULT Add_GameObject_Manager(OBJECT_ID id, CGameObject* object);
	void Update_GameObject_Manager();
	void Render_GameObject_Manager();
	void Release_GameObject_Manager();

private:
	list<CGameObject*> m_listGameObject[OBJECT_END];

};

