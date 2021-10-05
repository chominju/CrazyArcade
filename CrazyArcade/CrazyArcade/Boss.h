#pragma once
#include "GameObject.h"
class CBoss :
	public CGameObject
{
public:
	CBoss();
	~CBoss();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void Set_Rect();

	void Set_SeneorRect();
	void Change_CurrentState();

	void Decrease_BossHp() { m_bossHp--; if (m_bossHp <= 0) m_bossHp = 0; }
	int Get_BossHp() { return m_bossHp; }


	void Boss_Pattern();
	void Boss_Action();
	bool Boss_Move(int x, int y);
	bool Boss_WaterBall();
	
	bool Boss_AroundWater(int index);
	void Boss_HorizonWater(int startX, int endX , int indexY);
	void Boss_VerticalWater(int startY, int endY , int indexX);

	void Set_IsDeadAnimFin(bool isDeadAnimFin) { m_isDeadAnimFin = isDeadAnimFin; }
	bool Get_IsDeadAnimFin() { return m_isDeadAnimFin; }

private:
	RECT m_bossSensorRect;

	int m_bossSizeX;
	int m_bossSizeY;

	float m_bossSummonsTime;

	float m_waterBallTime;
	int waterBallNum;
	int m_waterNum;
	int m_waterNum2;
	float m_waterTime;


	bool m_isStopMove;

	int rectLeftIndex;
	int rectRightIndex;
	int rectUpIndex;
	int rectDownIndex;

	float m_timerFix;

	float m_hitTime;
	

	int m_bossHp;


	bool pattern[50];

	float m_horizonWaterTime;

	int m_waterX;
	int m_waterY;

	bool m_isBossHpHalf;

	//CHARACTER_STATE m_waterDir;
};

