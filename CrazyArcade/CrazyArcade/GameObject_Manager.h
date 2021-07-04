#pragma once
class CGameObject;
class CGameObject_Manager
{
	DECLARE_SINGLETON(CGameObject_Manager)
private:
	CGameObject_Manager();
	~CGameObject_Manager();

public:
	list<CGameObject*>& Get_Tile() { return m_listGameObject[OBJECT_ID::SCENE_TILE]; }
	list<CGameObject*>& Get_Object(OBJECT_ID id) { return m_listGameObject[id]; }
	CGameObject* Get_Player() { return m_listGameObject[OBJECT_ID::PLAYER].front(); }

public:
	HRESULT Add_GameObject_Manager(OBJECT_ID id, CGameObject* object);
	void Update_GameObject_Manager();
	void Render_GameObject_Manager();
	void Release_GameObject_Manager();

private:
	list<CGameObject*> m_listGameObject[OBJECT_END];

};

