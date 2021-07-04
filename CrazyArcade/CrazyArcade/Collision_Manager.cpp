#include "framework.h"
#include "Collision_Manager.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Player.h"
#include "GameObject_Manager.h"

CCollision_Manager::CCollision_Manager()
{
}

CCollision_Manager::~CCollision_Manager()
{
}

int CCollision_Manager::Collision_Object(list<CGameObject*>* player, list<CGameObject*>* object, CHARACTER_STATE state)
{
	for (auto * player_object : *player)
	{
		auto playerSize = dynamic_cast<CPlayer*>(player_object)->Get_PlayerSize();
		Object_Info tempInfo = dynamic_cast<CPlayer*>(player_object)->Get_Object_Info();

		float player_centerX = tempInfo.pos.x + playerSize[0] / 2; 
		float player_centerY = tempInfo.pos.y + playerSize[1] / 2 + 15; 

		int indexX = (player_centerX - STARTX) / (TILECX * 1.5);
		int indexY = (player_centerY - STARTY) / (TILECY * 1.5);

		float obj_CenterX = ((indexX-1) * TILECX * 1.5) + (TILECX * 1.5);
		float obj_CenterY = ((indexY-1) * TILECY * 1.5) + (TILECY * 1.5);

		RECT backRc{};
		backRc.left = STARTX + (TILECX * expansionSize / 2);
		backRc.top = STARTY + (TILECY * expansionSize / 2);;
		backRc.right = ((TILEX) * TILECX * expansionSize + STARTX) - (TILECX * expansionSize / 2);
		backRc.bottom = ((TILEY) * TILECY * expansionSize + STARTY) - (TILECY * expansionSize / 2);

		RECT rc{};
		rc.left = obj_CenterX - (TILECX * 1.5) / 2;
		rc.right = obj_CenterX + (TILECX * 1.5) / 2;
		rc.top = obj_CenterY - (TILECY * 1.5) / 2;
		rc.top = obj_CenterY + (TILECY * 1.5) / 2;

		// 왼쪽
		if (player_centerX <= backRc.left/* && indexX == 0*/ && state == CHARACTER_STATE::WALK_LEFT)
			return OBJ_WALL;

		// 오른쪽
		if (player_centerX >= backRc.right/* && indexX == TILEX - 1*/ && state == CHARACTER_STATE::WALK_RIGHT)
			return OBJ_WALL;

		// 위쪽
		if (player_centerY <= backRc.top/* && indexX == 0 */&& state == CHARACTER_STATE::WALK_UP)
			return OBJ_WALL;

		// 아래쪽
		if (player_centerY >= backRc.bottom/* && indexX == 0 */&& state == CHARACTER_STATE::WALK_DOWN)
			return OBJ_WALL;

		float nextIndex;
		float playerIndex = indexX + indexY * TILEX;

		if (state == CHARACTER_STATE::WALK_LEFT)
			nextIndex = playerIndex - 1;

		if (state == CHARACTER_STATE::WALK_RIGHT)
			nextIndex = playerIndex + 1;

		if (state == CHARACTER_STATE::WALK_UP)
			nextIndex = playerIndex - TILEX;

		if (state == CHARACTER_STATE::WALK_DOWN)
			nextIndex = playerIndex + TILEX;

		for (auto & objectBW : *object)
		{
			auto castObject = dynamic_cast<CTerrain*>(objectBW);
			Tile_Info tileInfo = castObject->Get_Terrain_Info();
			int finishIndex = castObject->Get_finishIndex();
			if (nextIndex == tileInfo.index || nextIndex == finishIndex)
			{
			if (tileInfo.isPush)
			{
				switch (state)
				{
				case WALK_LEFT:
					if (player_centerX <= (tileInfo.pos.x + TILECX * expansionSize)+TILECX * expansionSize/2)
					{
						bool isEmpty = true;
						for (auto & nextBW : *object)
						{
							auto obj = dynamic_cast<CTerrain*>(nextBW);
							if (nextIndex - 1 == obj->Get_Terrain_Info().index)
							{
								obj->Set_Pushed(false);
								isEmpty = false;
							}
						}
						if (isEmpty && !castObject->Get_Pushed())
						{
							objectBW->Set_State(state);
							dynamic_cast<CTerrain*>(objectBW)->Set_Pushed(true);
							dynamic_cast<CTerrain*>(objectBW)->Set_finish((tileInfo.centerX - TILECX * expansionSize) - TILECX * expansionSize / 2, tileInfo.pos.y, nextIndex - 1);
						}
						return OBJ_WALL;
					}
					else
						return OBJ_NONE;
				case WALK_RIGHT:
					if (player_centerX >= tileInfo.pos.x - TILECX * expansionSize / 2)
					{
						bool isEmpty = true;
						for (auto & nextBW : *object)
						{
							auto obj = dynamic_cast<CTerrain*>(nextBW);
							if (nextIndex + 1 == obj->Get_Terrain_Info().index)
							{
								obj->Set_Pushed(false);
								isEmpty = false;
							}
						}
						if (isEmpty && !castObject->Get_Pushed())
						{
							objectBW->Set_State(state);
							dynamic_cast<CTerrain*>(objectBW)->Set_Pushed(true);
							dynamic_cast<CTerrain*>(objectBW)->Set_finish((tileInfo.centerX + TILECX * expansionSize / 2)/* + TILECX * expansionSize / 2*/, tileInfo.pos.y, nextIndex + 1);
						}
						return OBJ_WALL;
					}
					else
						return OBJ_NONE;
				case WALK_UP:
					if (player_centerY <= (tileInfo.pos.y + TILECY * expansionSize) + TILECY * expansionSize/2)
					{
						bool isEmpty = true;
						for (auto & nextBW : *object)
						{
							auto obj = dynamic_cast<CTerrain*>(nextBW);
							if (nextIndex - TILEX == obj->Get_Terrain_Info().index)
							{
								obj->Set_Pushed(false);
								isEmpty = false;
							}
						}
						if (isEmpty && !castObject->Get_Pushed())
						{
							objectBW->Set_State(state);
							dynamic_cast<CTerrain*>(objectBW)->Set_Pushed(true);
							dynamic_cast<CTerrain*>(objectBW)->Set_finish(tileInfo.pos.x, (tileInfo.centerY - TILECY) - TILECY * expansionSize / 2, nextIndex - TILEX);
						}
						return OBJ_WALL;
					}
					else
						return OBJ_NONE;
				case WALK_DOWN:
					//if (player_centerY >= tileInfo.centerY - TILECY * expansionSize)
					if (player_centerY >= tileInfo.pos.y - TILECY * expansionSize / 2)
					{
						bool isEmpty = true;
						for (auto & nextBW : *object)
						{
							auto obj = dynamic_cast<CTerrain*>(nextBW);
							if (nextIndex + TILEX == obj->Get_Terrain_Info().index)
							{
								obj->Set_Pushed(false);
								isEmpty = false;
							}
						}
						if (isEmpty && !castObject->Get_Pushed())
						{
							objectBW->Set_State(state);
							dynamic_cast<CTerrain*>(objectBW)->Set_Pushed(true);
							dynamic_cast<CTerrain*>(objectBW)->Set_finish(tileInfo.pos.x, tileInfo.centerY + TILECY * expansionSize / 2, nextIndex + TILEX);
						}
						return OBJ_WALL;
					}
					else
						return OBJ_NONE;
				}
			}
			else
				return OBJ_WALL;
			}
		}
		return OBJ_NONE;




	/*	bool isObj = false;
		for (auto & objectBW : *object)
		{
			Tile_Info tileInfo = dynamic_cast<CTerrain*>(objectBW)->Get_Terrain_Info();
			if (nextIndex == tileInfo.index)
			{
				if (state == CHARACTER_STATE::WALK_LEFT)
				{
					if (player_centerX < obj_CenterX)
						return OBJ_NONE;
					else
					{
						if (tileInfo.isCollision)
						{
							isObj = true;
							return OBJ_WALL;
						}
						else
							return OBJ_NONE;
					}
				}
			}
		}
		if (!isObj)
			return OBJ_NONE;*/


		//// 왼쪽
		//if (player_centerX > obj_CenterX && (player_centerY >= rc.top && player_centerY < rc.bottom) && state==CHARACTER_STATE::WALK_LEFT)
		//	return OBJ_NONE;

		//// 오른쪽
		//if (player_centerX < obj_CenterX && (player_centerY >= rc.top && player_centerY < rc.bottom) && state == CHARACTER_STATE::WALK_RIGHT)
		//	return OBJ_NONE;

		//// 위쪽
		//if (player_centerY > obj_CenterY && (player_centerX >= rc.left && player_centerX < rc.right) && state == CHARACTER_STATE::WALK_UP)
		//	return OBJ_NONE;

		//// 아래쪽
		//if (player_centerY < obj_CenterY && (player_centerX >= rc.left && player_centerX < rc.right) && state == CHARACTER_STATE::WALK_DOWN)
		//	return OBJ_NONE;
	}

	//return OBJ_NONE;

	//bool check = false;
	//for (auto & player_object : *player)
	//{
	//	Object_Info temp = player_object->Get_Object_Info();

	//	int indexX = temp.pos.x / (TILECX * 1.5);
	//	int indexY = temp.pos.y / (TILECY * 1.5);
	//	int index = indexX + indexY * TILEX;
	//	int indexDiv = index % TILEX;

	//	// 왼쪽 <<
	//	if (indexDiv == 0 && state == CHARACTER_STATE::WALK_LEFT)
	//		return OBJ_WALL;
	//	// 위쪽
	//	if (index < TILEX && state == CHARACTER_STATE::WALK_UP)
	//		return OBJ_WALL;

	//	// 아래
	//	if (index >= TILEX * (TILEY - 1) && index < TILEX * TILEY && state == CHARACTER_STATE::WALK_DOWN)
	//		return OBJ_WALL;

	//	// 오른쪽
	//	if (indexDiv == TILEX - 1 && state == CHARACTER_STATE::WALK_RIGHT)
	//		return OBJ_WALL;


	//	int objIndex = -1;

	//	if (state == CHARACTER_STATE::WALK_LEFT)
	//	{
	//		objIndex = index - 1;
	//	}
	//	if (state == CHARACTER_STATE::WALK_RIGHT)
	//	{
	//		objIndex = index + 1;
	//	}
	//	if (state == CHARACTER_STATE::WALK_UP)
	//	{
	//		objIndex = index - TILEX;
	//	}
	//	if (state == CHARACTER_STATE::WALK_DOWN)
	//	{
	//		objIndex = index + TILEX;
	//	}


	//	bool isObj = false;
	//	for (auto & objectBW : *object)
	//	{
	//		Tile_Info tileInfo = dynamic_cast<CTerrain*>(objectBW)->Get_Terrain_Info();
	//		if (objIndex == tileInfo.index)
	//		{
	//			isObj = true;
	//			if (tileInfo.option == 2)
	//				return OBJ_WALL;
	//			else
	//				return OBJ_BOX;
	//		}

	//		if (!isObj)
	//			return OBJ_NONE;
	//	}
	//}

	//	int dirIndex[4] = { -1,-1,-1,-1 };

	//
	//	if (indexDiv != 0) // 왼쪽 index
	//	{
	//		dirIndex[0] = index - 1;
	//	}
	//	if (indexDiv != TILEX - 1)	// 오른쪽 index
	//	{
	//		dirIndex[1] = index + 1;
	//	}
	//	if (index < TILEX *(TILEY - 1))	// 위쪽 index
	//	{
	//		dirIndex[2] = index - TILEY;
	//	}
	//	if(index >= TILEX)	// 아래쪽 index
	//	{
	//		dirIndex[3] = index + TILEX;
	//	}


	//	bool isObj = false;

	//	for (auto & objectBW : *object)
	//	{

	//		Tile_Info tileInfo = dynamic_cast<CTerrain*>(objectBW)->Get_Terrain_Info();
	//		for (int i = 0; i < 4; ++i)
	//		{
	//			if (dirIndex[i] == tileInfo.index)
	//			{
	//				isObj = true;
	//				if (tileInfo.option == 2)
	//					return OBJ_WALL;
	//				else
	//					return OBJ_BOX;
	//			}
	//			
	//		}
	//		if (!isObj)
	//			return OBJ_NONE;
	//	}
	//}
}
