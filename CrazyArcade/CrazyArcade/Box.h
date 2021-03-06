#pragma once
#include "Terrain.h"
class CBox :
	public CTerrain
{
public:
	CBox();
	~CBox();
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void Set_Rect();
};

