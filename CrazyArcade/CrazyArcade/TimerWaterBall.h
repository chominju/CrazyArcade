#pragma once
#include "WaterBall.h"
class CTimerWaterBall :
	public CWaterBall
{

public:
	CTimerWaterBall(int index);
	~CTimerWaterBall();


	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
};
