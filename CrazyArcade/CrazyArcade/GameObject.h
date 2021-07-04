#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	Object_Info& Get_Object_Info() { return m_info; }
	void Set_State(CHARACTER_STATE state) { m_curState = state; }

	void Increase_Water() { m_playerableInfo.WaterBallNum+=1.f; }
	void Increase_WaterLength() { m_playerableInfo.WaterBallNum +=1.f; }
	void Increase_Speed() { m_playerableInfo.speed +=0.5f; }

	void Set_Kick(bool kick) { m_playerableInfo.isKick = kick; }
	void Set_Ride(bool ride) { m_playerableInfo.isRide = ride; }
	void Set_Shield(bool shield) { m_playerableInfo.isShield = shield; }
	void Set_Revival(bool revival) { m_playerableInfo.isRevival = revival; }


	void Set_Pos(int x, int y) { m_info.pos.x += x; m_info.pos.y += y; }

	virtual int Get_PlayerIndex();
	virtual void Set_PlayerIndex();
	virtual void FrameMove(float speed = 1.f);
public:
	virtual HRESULT Ready_GameObject()PURE;
	virtual int Update_GameObject()PURE;
	virtual void Late_Update_GameObject()PURE;
	virtual void Render_GameObject()PURE;
	virtual void Release_GameObject() PURE;

protected:
	Object_Info m_info;
	Player_Info m_playerableInfo;
	CHARACTER_STATE m_curState;
	CHARACTER_STATE m_preState;
	Frame m_frame;
};


/*

	bool isKick;
	bool isRide;
	bool isShield;
	bool isRevival;
	float WaterNum;
	float speed;
	float isWaterLength;
*/