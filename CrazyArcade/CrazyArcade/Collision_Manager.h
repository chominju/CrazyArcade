#pragma once
class CGameObject;
class CItem;
class CCollision_Manager
{
private:
	CCollision_Manager();
	~CCollision_Manager();
public:
	static int Collision_Player_Object(list<CGameObject*>* player, list<CGameObject*>* object, CHARACTER_STATE state);
	static int Collision_Player_WaterBall(list<CGameObject*>* player, list<CGameObject*>* waterBall, CHARACTER_STATE state);
	static int Collision_Player_Water(list<CGameObject*>* player, list<CGameObject*>* water);
	static int Collision_Player_Item(list<CGameObject*>* player, list<CGameObject*>* item);
	static int Collision_Player_Monster(list<CGameObject*>* player, list<CGameObject*>* monster);


	static int Collision_Monster_Object(CGameObject* monster, list<CGameObject*>* object, list<CGameObject*>* waterBall, CHARACTER_STATE& state);
	static int Collision_Monster_Water(list<CGameObject*>* monster, list<CGameObject*>* water);
	static int Collision_Boss_WaterBall(CGameObject* monster, list<CGameObject*>* waterBall, int& bossHp);
	
	static void Collision_Monster_Sensor(RECT * rect, list<CGameObject*>* player, CHARACTER_STATE& state);



	static int Collision_Item_Water(list<CGameObject*>* item, list<CGameObject*>* water);
	static int Collision_Item_Object(list<CGameObject*>* item, list<CGameObject*>* object);
	
	/*static void Collision_Rect(list<CGameObject*>* listDest, list<CGameObject*>* listSrc);
	static void Collision_Portal(list<CGameObject*>* player, list<CGameObject*>* portal);
	static void Collision_Monster(list<CGameObject*>* player, list<CGameObject*>* monster);
	static void Collision_Skill(list<CGameObject*>* skill, list<CGameObject*>* monster);
	static void Collision_Boss(list<CGameObject*>* skill, list<CGameObject*>* boss);
	static void Collision_BossSkill(list<CGameObject*>* skill, list<CGameObject*>* player);
	static bool Collision_Rope(list<CGameObject*>* player, list<CGameObject*>* rope, const RECT** rc);
	static void Collision_DropItem(list<CGameObject*>* player, list<CGameObject*>* dropItem);*/
private:
	static CGameObject* pushObject;
	static CGameObject* pushWater;

	D3DXMATRIX
};

