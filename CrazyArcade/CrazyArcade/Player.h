#pragma once
#include "GameObject.h"
class CPlayer :
	public CGameObject
{
public:
	explicit CPlayer();
	virtual ~CPlayer();
public:
	void PlayerActrion();
	void Use_WaterBall();
	float* Get_PlayerSize() { return m_playerSize; }

	void Set_StateKey(wstring state) { m_stateKey = state; }

	virtual void FrameMove(float speed = 1.f);
	virtual int Get_PlayerIndex() {return  m_LocationIndex;}
	//virtual void Set_PlayerIndex();
	virtual void Set_Rect();

	void Auto_Item(Item_Info eatItem, Object_Info posInfo);
	void Manual_Item(Item_Info eatItem);

	void Set_IsTrappeed(bool trapped) { m_isTrapped = trapped; m_frame.frameStart = 0; }
	bool Get_IsTrappeed() { return m_isTrapped; }


	void Set_IsDeadAnim(bool deadAnim) { m_isDeadAnim = deadAnim; }
	bool Get_IsDeadAnim() { return m_isDeadAnim; }
	void IsRide();

	void Set_IsUseShield(bool shield) { m_isUseShield = shield; }
	bool Get_IsUseShield() { return m_isUseShield; }

	void Use_Item();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject()			override;
	virtual int		Update_GameObject()			override;
	virtual void	Late_Update_GameObject()	override;
	virtual void	Render_GameObject()			override;
	virtual void	Release_GameObject()		override;

private:
	//CHARACTER_STATE m_curState;
	//CHARACTER_STATE m_preState;
	float m_startIndex;
	float m_playerSize[2];


	bool m_isTrapped;

	float m_rideSpeed;

	bool m_animCheck;

	bool m_itemUseable;

	bool m_isUseShield;

	int m_waterBallNumber;

	Item_Info m_haveItem;

	int m_TimerwaterBallNum;
};

