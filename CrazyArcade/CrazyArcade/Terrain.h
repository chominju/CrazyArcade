#pragma once
#include "GameObject.h"
class CTerrain :
	public CGameObject
{

public:
	Tile_Info& Get_Terrain_Info() { return m_info; }
	void Set_Terrain_Info(Tile_Info& tile) { m_info = tile; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

protected:
	Tile_Info m_info;
};

