#pragma once
class CMFCToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();
public:
	const vector<Tile_Info*>& Get_VecTile() { return m_vecTile; }
	const vector<Tile_Info*>& Get_VecObj() { return m_vecObj; }
	const vector<Tile_Info*>& Get_VecRespawnTile() { return m_vecRespawnTile; }
public:
	void Set_Tile(Tile_Info* tile);
	void Set_Obj(Tile_Info* obj);
	void Set_Respawn(Tile_Info* obj);
public:
	void Set_View(CMFCToolView* view) { m_view = view; }
public:
	HRESULT Ready_Terrain();
	void Update_Terrain();
	void Render_Terrain();
	void MiniRender_Terrain();

	void Release_Terrain();
public:
	void Tile_Change_Terrain(const D3DXVECTOR3& pos,const wstring objectKey , const wstring stateKey ,const BYTE& drawID);
private:
	int Get_TileIndex(const D3DXVECTOR3& pos);
	bool IsPicking(const D3DXVECTOR3& pos, const int index);

public:
	vector<Tile_Info*> m_vecTile;
	vector<Tile_Info*> m_vecObj;
	vector<Tile_Info*> m_vecRespawnTile;
	CMFCToolView* m_view;
};

