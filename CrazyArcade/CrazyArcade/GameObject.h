#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	Object_Info& Get_Object_Info() { return m_info; }
	RECT& Get_Rect() { return m_rect; }
	void Set_State(CHARACTER_STATE state) { m_curState = state; }
	float Get_PlayerCenterX() { return m_player_centerX; }
	float Get_PlayerCenterY() { return m_player_centerY; }

	void Increase_Water() { m_playerableInfo.WaterBallCurrentMax+=1; }
	void Increase_WaterLength() { m_playerableInfo.WaterLength +=1; }
	void Increase_Speed() { m_playerableInfo.speed +=0.5f; }

	void Decrese_WaterBall() { m_playerableInfo.WaterBallCurrent -= 1; }

	void Set_Kick(bool kick) { m_playerableInfo.isKick = kick; }
	void Set_Ride(bool ride) { m_playerableInfo.isRide = ride; }
	void Set_Shield(bool shield) { m_playerableInfo.isShield = shield; }
	void Set_Revival(bool revival) { m_playerableInfo.isRevival = revival; }

	int Get_WaterLength() { return m_playerableInfo.WaterLength; }

	void Set_LocationIndex(float index) { m_LocationIndex = index; }
	float Get_LocationIndex() { return m_LocationIndex; }


	void Set_Pos(float x, float y) { m_info.pos.x += x; m_info.pos.y += y; }

	void Set_MoveLock(bool lock) { m_moveLock = lock; }
	bool Get_MoveLock() { return m_moveLock; }

	bool Get_IsDead() { return m_dead; }
	void Set_Dead(bool dead) { m_dead = dead; }

	virtual int Get_PlayerIndex();
	virtual void Set_PlayerIndex();
	virtual void FrameMove(float speed = 1.f);
public:
	virtual HRESULT Ready_GameObject()PURE;
	virtual int Update_GameObject()PURE;
	virtual void Late_Update_GameObject()PURE;
	virtual void Render_GameObject()PURE;
	virtual void Release_GameObject() PURE;
	virtual void Set_Rect();
protected:
	Object_Info m_info;
	Player_Info m_playerableInfo;
	CHARACTER_STATE m_curState;
	CHARACTER_STATE m_preState;
	Frame m_frame;
	float m_LocationIndex;
	RECT m_rect;

	float m_player_centerX;
	float m_player_centerY;

	bool m_moveLock;

	bool m_dead;
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