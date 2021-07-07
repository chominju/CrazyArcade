#pragma once
#include "GameObject.h"
class CWater :
	public CGameObject
{
public:
	CWater(int indexX, int indexY);
	~CWater();

	void Set_StateKey(wstring stateKey) { m_stateKey = stateKey; }
	bool Get_IsBoxDestroy(){ return m_isBoxDestroy; }
	void Set_IsBoxDestroy(bool isBoxDestory) { m_isBoxDestroy = isBoxDestory; }

	virtual void FrameMove(float speed = 1.f);
	// CGameObject��(��) ���� ��ӵ�
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
	bool m_isBoxDestroy;
};

