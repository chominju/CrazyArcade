#pragma once
#include "GameObject.h"
class CTerrain :
	public CGameObject
{

public:
	void Set_Terrain_Info(Tile_Info& tile) { m_info = tile; }
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

protected:
	Tile_Info m_info;
};
