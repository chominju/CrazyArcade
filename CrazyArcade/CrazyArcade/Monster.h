#pragma once
#include "GameObject.h"
class CMonster :
	public CGameObject
{
	// CGameObject을(를) 통해 상속됨

public:
	CMonster();
	~CMonster();


	void Set_MonsterIndex(int index);
	void Set_RespawnPos(int index);

public:
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void Set_Rect();

	void Set_SeneorRect();

	void Change_CurrentState();

private:
	int m_monsterIndex;
	RECT m_monsterSensorRect;

	int m_monsterSizeX;
	int m_monsterSizeY;
};

