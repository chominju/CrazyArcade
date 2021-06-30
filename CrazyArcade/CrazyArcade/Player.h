#pragma once
#include "GameObject.h"
class CPlayer :
	public CGameObject
{
public:
	explicit CPlayer();
	virtual ~CPlayer();
public:
	void FrameMove(float speed = 1.f);
	void PlayerActrion();
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject()			override;
	virtual int		Update_GameObject()			override;
	virtual void	Late_Update_GameObject()	override;
	virtual void	Render_GameObject()	override;
	virtual void	Release_GameObject()		override;
private:
	Object_Info m_info;
	CHARACTER_STATE m_curState;
	CHARACTER_STATE m_preState;
	wstring m_objectKey;
	wstring m_stateKey;
	float m_startIndex;
	Frame m_frame;
	float m_speed;
};

