#pragma once
#include "Scene.h"
class CMonsterStage1 :
	public CScene
{
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};

