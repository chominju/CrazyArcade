#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	virtual HRESULT Ready_GameObject()PURE;
	virtual int Update_GameObject()PURE;
	virtual void Late_Update_GameObject()PURE;
	virtual void Render_GameObject()PURE;
	virtual void Release_GameObject() PURE;
};

