#pragma once
#include "GameObject.h"
class CTerrain :
	public CGameObject
{
public:
	CTerrain();
	~CTerrain();
public:
	Tile_Info& Get_Terrain_Info() { return m_tileInfo; }
	void Set_Terrain_Info(Tile_Info& tile) { m_tileInfo = tile; }
	void Set_Info_Pos() { m_info.pos.x = m_tileInfo.pos.x;  m_info.pos.y = m_tileInfo.pos.y; m_info.pos.z = m_tileInfo.pos.z;}


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

protected:
	Tile_Info m_tileInfo;

	float m_textureSizeX;
	float m_textureSizeY;
	

};

