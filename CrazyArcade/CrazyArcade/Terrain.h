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

	void Set_Pushed(bool push) { m_isPushed = push; }
	bool Get_Pushed() { return m_isPushed; }
	void Set_finish(int x, int y, int index) { m_finishX = x; m_finishY = y; m_finishIndex = index;}
	int Get_finishIndex() { return m_finishIndex; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;


	float Get_PushTime() { return m_pushTime; }
	void Set_PushTime() { m_pushTime += CTime_Manager::Get_Instance()->Get_DeltaTime(); }
	void ResetPushTime() { m_pushTime = 0.f; }


protected:
	Tile_Info m_tileInfo;
	bool m_isPushed;
	int m_finishX;
	int m_finishY;
	int m_finishIndex;

	float m_textureSizeX;
	float m_textureSizeY;
	
	float m_pushTime;

};

