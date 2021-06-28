#pragma once
#include "GameObject.h"
class CTerrain final:
	public CGameObject
{
public:
	CTerrain();
	~CTerrain();

	const vector<Tile_Info*>& Get_VecTile()const { return m_vecTile; }
	const vector<Tile_Info*>& Get_VecObj()const { return m_vecObj; }

public:
	HRESULT LoadTileData_Terrain(const wstring& filePath);



	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;


private:
	vector<Tile_Info*>m_vecTile;
	vector<Tile_Info*>m_vecObj;
};

