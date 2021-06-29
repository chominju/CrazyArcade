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
	void MovePlayer();
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject()			override;
	virtual int		Update_GameObject()			override;
	virtual void	Late_Update_GameObject()	override;
	virtual void	Render_GameObject()	override;
	virtual void	Release_GameObject()		override;
private:
	Object_Info m_info;
	Frame m_frame;
	float m_speed;
};

