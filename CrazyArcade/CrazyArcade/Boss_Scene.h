#pragma once
#include "Scene.h"

class CBoss_Scene :
	public CScene
{
public:
	CBoss_Scene();
	~CBoss_Scene();
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};
