#include "framework.h"
#include "Stage.h"
#include "GameObject_Manager.h"
#include "Terrain.h"
#include "Player.h"
#include "Collision_Manager.h"

CStage::CStage()
	:m_gameObject_Manager(CGameObject_Manager::Get_Instance())
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	CGameObject * object = new CPlayer;
	if (FAILED(object->Ready_GameObject()))
		return E_FAIL;
	m_gameObject_Manager->Add_GameObject_Manager(OBJECT_ID::PLAYER, object);

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_SINGLE,
		L"../Resource/Ui/UIFrm.png", L"MainUI")))
		return E_FAIL;

	CGameObject_Manager::Get_Instance()->Get_ItemData(1);

	/*CGameObject* object = new CTerrain;
	if (FAILED(object->Ready_GameObject()))
		return E_FAIL;
	
	m_gameObject_Manager->Add_GameObject_Manager(OBJECT_ID::SCENE_TILE, object);
	object = new CPlayer;
	if(FAILED(object->Ready_GameObject()))
		return E_FAIL;
	m_gameObject_Manager->Add_GameObject_Manager(OBJECT_ID::PLAYER, object);*/
	return S_OK;
}

void CStage::Update_Scene()
{
	CKey_Manager::Get_Instance()->Update_Key_Manager();
	m_gameObject_Manager->Update_GameObject_Manager();

	CCollision_Manager::Collision_Player_Item(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::ITEM));
	CCollision_Manager::Collision_Item_Water(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::ITEM), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATER));
	CCollision_Manager::Collision_Item_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::ITEM), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT));
	CCollision_Manager::Collision_Player_Water(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATER));
}

void CStage::Render_Scene()
{
	D3DXMATRIX matTrans , matScale , matWorld;
	const Texture_Info* pTexInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"MainUI");

	if (nullptr == pTexInfo)
		return;
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	D3DXMatrixScaling(&matScale, expansionSize, expansionSize, expansionSize);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->texture, nullptr, &D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_gameObject_Manager->Render_GameObject_Manager();
}

void CStage::Release_Scene()
{
	m_gameObject_Manager->Destroy_Instance();
}
