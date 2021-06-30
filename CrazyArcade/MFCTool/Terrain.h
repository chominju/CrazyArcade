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
public:
	void Set_Tile(Tile_Info* tile) { m_vecTile[tile->index] = tile; }
	void Set_Obj(Tile_Info* obj)
	{ 
		m_vecObj[obj->index] = obj;
		m_vecObj[obj->index]->pos = m_vecTile[obj->index]->pos;
	}
public:
	void Set_View(CMFCToolView* view) { m_view = view; }
public:
	HRESULT Ready_Terrain();
	void Update_Terrain();
	void Render_Terrain();
	void MiniRender_Terrain();

	void Release_Terrain();
public:
	void Tile_Change_Terrain(const D3DXVECTOR3& pos,const wstring objectKey , const wstring stateKey ,const BYTE& drawID, const D3DXVECTOR3 & size, const BYTE& option = 0);
private:
	int Get_TileIndex(const D3DXVECTOR3& pos);
	bool IsPicking(const D3DXVECTOR3& pos, const int index);

public:
	vector<Tile_Info*> m_vecTile;
	vector<Tile_Info*> m_vecObj;

	CMFCToolView* m_view;
};

