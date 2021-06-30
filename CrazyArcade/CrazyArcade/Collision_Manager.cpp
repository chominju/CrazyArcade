#include "framework.h"
#include "Collision_Manager.h"
#include "GameObject.h"

CCollision_Manager::CCollision_Manager()
{
}

CCollision_Manager::~CCollision_Manager()
{
}

bool CCollision_Manager::Collision_Object(list<CGameObject*>* player, list<CGameObject*>* object)
{
	bool check = false;
	for (auto & player_object : *player)
	{
		Object_Info temp = player_object->Get_Object_Info();

		int indexX = temp.pos.x / (TILECX * 1.5);
		int indexY = temp.pos.y / (TILECY * 1.5);
		int index = indexX + indexY * TILEY;
	}
	return true;
}
