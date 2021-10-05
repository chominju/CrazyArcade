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
	CGameObject* Get_Player();

	Item_Info Get_ItemData(BYTE index);

	void Reset_Object(OBJECT_ID id);
	void Reset_RenderingList();

	bool IsExistObject(int index);
	bool IsExistWater(int index);

	void SavePlayerData();
	Player_Info GetSavePlayerData() { return m_savePlayerData; }

public:
	HRESULT Add_GameObject_Manager(OBJECT_ID id, CGameObject* object);
	HRESULT Add_ItemData(Item_Info& data);
	void Update_GameObject_Manager();
	void Render_GameObject_Manager();
	void Release_GameObject_Manager();

private:
	list<CGameObject*> m_listGameObject[OBJECT_END];
	list<CGameObject*> m_listRenderingGameObject[OBJECT_END];

	map<BYTE, Item_Info>m_mapItemInfo;

	Player_Info m_savePlayerData;

};

