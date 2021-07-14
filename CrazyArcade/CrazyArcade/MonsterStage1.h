#pragma once
#include "Scene.h"
class CMonsterStage1 :
	public CScene
{
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};

