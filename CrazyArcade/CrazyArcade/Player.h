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

	virtual void FrameMove(float speed = 1.f);
	virtual int Get_PlayerIndex() {return  m_LocationIndex;}
	//virtual void Set_PlayerIndex();
	virtual void Set_Rect();


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject()			override;
	virtual int		Update_GameObject()			override;
	virtual void	Late_Update_GameObject()	override;
	virtual void	Render_GameObject()	override;
	virtual void	Release_GameObject()		override;

private:
	//CHARACTER_STATE m_curState;
	//CHARACTER_STATE m_preState;
	wstring m_objectKey;
	wstring m_stateKey;
	float m_startIndex;
	float m_speed;

	float m_playerSize[2];

};

