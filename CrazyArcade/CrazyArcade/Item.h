#pragma once
#include "GameObject.h"
class CItem :
	public CGameObject
{
public:
	CItem(int index);
	~CItem();
	void Set_ItemData();
	void Set_Info(Tile_Info info);

	Item_Info Get_ItemData() { return m_itemData; }

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;


private:
	Item_Info m_itemData;
	BYTE m_itemIndex;
};

