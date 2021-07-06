#pragma once
#include "GameObject.h"
class CWater :
	public CGameObject
{
public:
	CWater(int indexX, int indexY);
	~CWater();

	void Set_StateKey(wstring stateKey) { m_stateKey = stateKey; }

	virtual void FrameMove(float speed = 1.f);
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	wstring m_objectKey;
	wstring m_stateKey;
	int m_indexX;
	int m_indexY;
};

