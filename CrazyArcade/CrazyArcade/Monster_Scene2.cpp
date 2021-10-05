#include "framework.h"
#include "Monster_Scene2.h"
#include "GameObject_Manager.h"
#include "Terrain.h"
#include "Player.h"
#include "Collision_Manager.h"
#include "Monster.h"
#include "Load_Manager.h"
#include "Scene_Manager.h"


CMonster_Scene2::CMonster_Scene2()
	:m_gameObject_Manager(CGameObject_Manager::Get_Instance())
{
}

CMonster_Scene2::~CMonster_Scene2()
{
}

HRESULT CMonster_Scene2::Ready_Scene()
{
	if (m_gameObject_Manager->Get_Player() != nullptr)
	{
		m_gameObject_Manager->Get_Player()->Reset_PlayerInfo();
		m_gameObject_Manager->Get_Player()->Set_Pos2(500, 450);
	}
	else
	{
		CGameObject * object = new CPlayer;
		if (FAILED(object->Ready_GameObject()))
			return E_FAIL;
		m_gameObject_Manager->Add_GameObject_Manager(OBJECT_ID::PLAYER, object);
	}

	CGameObject_Manager::Get_Instance()->Reset_Object(OBJECT_ID::RESPAWN_TILE);
	CGameObject_Manager::Get_Instance()->Reset_Object(OBJECT_ID::MONSTER);
	CGameObject_Manager::Get_Instance()->Reset_Object(OBJECT_ID::WATER);
	CGameObject_Manager::Get_Instance()->Reset_Object(OBJECT_ID::WATERBALL);
	CGameObject_Manager::Get_Instance()->Reset_RenderingList();

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_SINGLE,
		L"../Resource/Ui/UIFrm.png", L"MainUI")))
		return E_FAIL;

	CLoad_Manager::LoadTerrainData(L"../Data/MonsterMap2.dat");

	Create_Monster();

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"MonsterScene.mp3");

	return S_OK;
}

void CMonster_Scene2::Update_Scene()
{
	CKey_Manager::Get_Instance()->Update_Key_Manager();
	m_gameObject_Manager->Update_GameObject_Manager();

	CCollision_Manager::Collision_Player_Item(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::ITEM));
	CCollision_Manager::Collision_Item_Water(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::ITEM), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATER));
	CCollision_Manager::Collision_Item_Object(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::ITEM), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT));
	CCollision_Manager::Collision_Player_Water(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATER));

	CCollision_Manager::Collision_Monster_Water(&CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::MONSTER), &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATER));

	if (CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::MONSTER).size() == 0)
	{
		if (!m_winSound)
		{
			m_winSound = true;
			CSoundMgr::Get_Instance()->PlaySound(L"GameWin.wav", CSoundMgr::PLAYER);
		}
		m_nextSceneTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_nextSceneTime <= 0)
			CScene_Manager::Get_Instance()->Change_Scene_Manager(SCENE_ID::BOSS_SCENE);
	}
}

void CMonster_Scene2::Render_Scene()
{
	D3DXMATRIX matTrans, matScale, matWorld;
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

void CMonster_Scene2::Release_Scene()
{
	m_gameObject_Manager->Destroy_Instance();
}

void CMonster_Scene2::Create_Monster()
{
	auto respawnTile = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::RESPAWN_TILE);
	int respawnSize = respawnTile.size();

	for (int i = 0; i < respawnSize; i++)
	{
		CMonster* monster = new CMonster;
		if (i == 0 || i == respawnSize - 1)
		{
			monster->Set_MonsterIndex(2);
			monster->Ready_GameObject();
			monster->Set_RespawnPos(i);
		}
		else
		{
			monster->Set_MonsterIndex(1);
			monster->Ready_GameObject();
			monster->Set_RespawnPos(i);
		}

		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::MONSTER, monster);
	}
}
