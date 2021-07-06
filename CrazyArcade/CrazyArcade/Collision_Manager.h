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
	static int Collision_WaterBall_Object(list<CGameObject*>* waterball, list<CGameObject*>* object);
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
};

