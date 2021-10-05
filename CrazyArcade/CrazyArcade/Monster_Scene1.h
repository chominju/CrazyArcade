#pragma once
#include "Scene.h"
class CGameObject_Manager;

class CMonster_Scene1 :
	public CScene
{
public:
	explicit CMonster_Scene1();
	virtual ~CMonster_Scene1();
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;


	void Create_Monster();
private:
	CGameObject_Manager* m_gameObject_Manager;

};
