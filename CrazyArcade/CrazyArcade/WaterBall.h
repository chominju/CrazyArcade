#pragma once
#include "GameObject.h"
class CWaterBall :
	public CGameObject
{
public:
	CWaterBall(int index);
	~CWaterBall();
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	int m_animIndex;
	float m_time;
};

