#include "framework.h"
#include "Boss_Scene.h"
#include "GameObject_Manager.h"
#include "Terrain.h"
#include "Player.h"
#include "Collision_Manager.h"
#include "Monster.h"
#include "Load_Manager.h"
#include "Scene_Manager.h"
#include "Boss.h"
#include "Item.h"

CBoss_Scene::CBoss_Scene()
	:m_gameObject_Manager(CGameObject_Manager::Get_Instance())
{
}

CBoss_Scene::~CBoss_Scene()
{
}

HRESULT CBoss_Scene::Ready_Scene()
{
	if (m_gameObject_Manager->Get_Player() != nullptr)
	{
		m_gameObject_Manager->Get_Player()->Reset_PlayerInfo();
		m_gameObject_Manager->Get_Player()->Set_Pos2(600, 720);
	}
	else
	{
		CPlayer * object = new CPlayer;
		if (FAILED(object->Ready_GameObject()))
			return E_FAIL;
		object->Set_Pos2(600, 720);
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

	CLoad_Manager::LoadTerrainData(L"../Data/MonsterMap3.dat");


	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"BossScene.wav");

	Create_Monster();
	Create_Item();

	return S_OK;
}

void CBoss_Scene::Update_Scene()
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
		CScene_Manager::Get_Instance()->Change_Scene_Manager(SCENE_ID::LOADING);
	}
}

void CBoss_Scene::Render_Scene()
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

void CBoss_Scene::Release_Scene()
{
}

void CBoss_Scene::Create_Monster()
{
	auto respawnTile = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::RESPAWN_TILE);
	int respawnSize = respawnTile.size();

	/*for (int i = 0; i < respawnSize; i++)
	{
		CMonster* monster = new CMonster;
		if (i == 0 || i == respawnSize - 1)
		{
			monster->Set_MonsterIndex(2);
			monster->Ready_GameObject();
			monster->Set_RespawnPos(i);
			
			int temp = rand() % 2;
			if (temp == 0)
				monster->Set_State(WALK_LEFT);
			else
				monster->Set_State(WALK_RIGHT);
		}
		else
		{
			monster->Set_MonsterIndex(1);
			monster->Ready_GameObject();
			monster->Set_RespawnPos(i);

			int temp = rand() % 2;
			if (temp == 0)
				monster->Set_State(WALK_LEFT);
			else
				monster->Set_State(WALK_RIGHT);
		}

		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::MONSTER, monster);
	}*/

	CBoss* boss = new CBoss;
	boss->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::MONSTER, boss);
}

void CBoss_Scene::Create_Item()
{
	CItem * item = new CItem(0);
	item->Set_ItemData();
	Tile_Info temp;
	temp.pos.x = 5 * TILECX *expansionSize + STARTX;
	temp.pos.y = 7 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);


	item = new CItem(0);
	item->Set_ItemData();
	temp.pos.x = 5 * TILECX *expansionSize + STARTX;
	temp.pos.y = 8 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);


	item = new CItem(0);
	item->Set_ItemData();
	temp.pos.x = 5 * TILECX *expansionSize + STARTX;
	temp.pos.y = 9 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);


	item = new CItem(1);
	item->Set_ItemData();
	temp.pos.x = 5 * TILECX *expansionSize + STARTX;
	temp.pos.y = 10 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);


	item = new CItem(0);
	item->Set_ItemData();
	temp.pos.x = 9 * TILECX *expansionSize + STARTX;
	temp.pos.y = 10 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);


	item = new CItem(2);
	item->Set_ItemData();
	temp.pos.x = 9 * TILECX *expansionSize + STARTX;
	temp.pos.y = 9 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);

	item = new CItem(2);
	item->Set_ItemData();
	temp.pos.x = 9 * TILECX *expansionSize + STARTX;
	temp.pos.y = 8 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);

	item = new CItem(2);
	item->Set_ItemData();
	temp.pos.x = 9 * TILECX *expansionSize + STARTX;
	temp.pos.y = 7 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);




	item = new CItem(3);	// ¹Ù´Ã
	item->Set_ItemData();
	temp.pos.x = 0 * TILECX *expansionSize + STARTX;
	temp.pos.y = 2 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);

	item = new CItem(6);	// ºÎ¾ûÀÌ 
	item->Set_ItemData();
	temp.pos.x = 14 * TILECX *expansionSize + STARTX;
	temp.pos.y = 2 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);

	item = new CItem(8);	// ½¯µå 
	item->Set_ItemData();
	temp.pos.x = 14 * TILECX *expansionSize + STARTX;
	temp.pos.y = 10 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);

	item = new CItem(9);	// ¹ßÂ÷±â 
	item->Set_ItemData();
	temp.pos.x = 0 * TILECX *expansionSize + STARTX;
	temp.pos.y = 10 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);

	item = new CItem(10);	// ¿ø°Ý ÆøÅº 
	item->Set_ItemData();
	temp.pos.x = 0 * TILECX *expansionSize + STARTX;
	temp.pos.y = 12 * TILECX *expansionSize + STARTY;
	temp.size = { expansionSize,expansionSize,0 };
	item->Set_Info(temp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::ITEM, item);
}
