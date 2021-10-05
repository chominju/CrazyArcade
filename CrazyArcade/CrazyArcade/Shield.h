#pragma once
#include "GameObject.h"
class CShield :
	public CGameObject
{

public:
	CShield(Object_Info info);
	~CShield();
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float m_disappear;
};

