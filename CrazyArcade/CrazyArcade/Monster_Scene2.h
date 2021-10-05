#pragma once
#include "Scene.h"
class CGameObject_Manager;
class CMonster_Scene2 :
	public CScene
{

public:
	CMonster_Scene2();
	~CMonster_Scene2();
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

	void Create_Monster();
private:
	CGameObject_Manager* m_gameObject_Manager;
};
