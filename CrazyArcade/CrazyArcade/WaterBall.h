#pragma once
#include "GameObject.h"
class CWaterBall :
	public CGameObject
{
public:
	CWaterBall();
	CWaterBall(int index);
	~CWaterBall();
	// CGameObject을(를) 통해 상속됨

	int Get_indexX() { return indexX; }
	int Get_indexY() { return indexY; }

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;


	bool Get_isTimerWaterBall() { return m_isTimerWaterBall; }

	void Move_BossWaterBall();

	void Set_UniqueNumber(int number) { m_uniqueNumber = number;}
	int Get_UniqueNumber() { return m_uniqueNumber; }


	void Set_IsBossMonster(bool bossMonster) { m_isBossMonster = bossMonster; }
	void Set_WaterBallDir(CHARACTER_STATE state) { m_bossState = state; }

	void Set_IsBossMonsterHit(bool bossMonster) { m_isBossMonsterHit = bossMonster; }
protected:
	int m_animIndex;
	float m_time;
	int indexX;
	int indexY;

	bool m_isBossMonster;
	bool m_isWaterBallMove;

	bool m_isCheckIndex;

	bool m_isTimerWaterBall;

	bool m_isBossMonsterHit;

	CHARACTER_STATE m_bossState;


	int m_uniqueNumber;

};

