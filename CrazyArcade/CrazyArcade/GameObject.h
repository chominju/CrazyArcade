#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	Object_Info& Get_Object_Info() { return m_info; }
public:
	virtual HRESULT Ready_GameObject()PURE;
	virtual int Update_GameObject()PURE;
	virtual void Late_Update_GameObject()PURE;
	virtual void Render_GameObject()PURE;
	virtual void Release_GameObject() PURE;

protected:
	Object_Info m_info;
};

