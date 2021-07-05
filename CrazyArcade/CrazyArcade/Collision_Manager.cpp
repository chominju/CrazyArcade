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

int CCollision_Manager::Collision_Player_Object(list<CGameObject*>* player, list<CGameObject*>* object, CHARACTER_STATE state)
{
	for (auto * player_object : *player)
	{
		RECT playerRect = player_object->Get_Rect();

		RECT backRc{};
		backRc.left = STARTX;
		backRc.top = STARTY;
		backRc.right = ((TILEX)* TILECX * expansionSize + STARTX);
		backRc.bottom = ((TILEY)* TILECY * expansionSize + STARTY);

		// 왼쪽
		if (playerRect.left <= backRc.left/* && indexX == 0*/ && state == CHARACTER_STATE::WALK_LEFT)
			return OBJ_WALL;

		// 오른쪽
		if (playerRect.right >= backRc.right/* && indexX == TILEX - 1*/ && state == CHARACTER_STATE::WALK_RIGHT)
			return OBJ_WALL;

		// 위쪽
		if (playerRect.top <= backRc.top/* && indexX == 0 */&& state == CHARACTER_STATE::WALK_UP)
			return OBJ_WALL;

		// 아래쪽
		if (playerRect.bottom >= backRc.bottom/* && indexX == 0 */&& state == CHARACTER_STATE::WALK_DOWN)
			return OBJ_WALL;

		float nextIndex[3] = {-1,-1,-1};
		float playerIndex = player_object->Get_LocationIndex();

		if (state == CHARACTER_STATE::WALK_LEFT)
		{
			if (playerIndex - 1 - TILEX >= 0)
				nextIndex[0] = playerIndex - 1 - TILEX;

			nextIndex[1] = playerIndex - 1;

			if (playerIndex - 1 + TILEX < TILEX * TILEY)
				nextIndex[2] = playerIndex - 1 + TILEX;
		}

		if (state == CHARACTER_STATE::WALK_RIGHT)
		{
			if (playerIndex + 1 - TILEX >= 0)
				nextIndex[0] = playerIndex + 1 - TILEX;

			nextIndex[1] = playerIndex + 1;
			
			if (playerIndex + 1 + TILEX < TILEX * TILEY)
				nextIndex[2] = playerIndex + 1 + TILEX;

		}

		if (state == CHARACTER_STATE::WALK_UP)
		{
			if (playerIndex - TILEX - 1 >= 0)
				nextIndex[0] = playerIndex -1 - TILEX;

			nextIndex[1] = playerIndex - TILEX;


			if (playerIndex - TILEX + 1 < TILEX)
				nextIndex[2] = playerIndex + 1 - TILEX;


		}

		if (state == CHARACTER_STATE::WALK_DOWN)
		{
			if (playerIndex - 1 + TILEX >= 0)
				nextIndex[0] = playerIndex - 1 + TILEX;

			nextIndex[1] = playerIndex + TILEX;


			if (playerIndex + 1 + TILEX < TILEX * TILEY)
				nextIndex[2] = playerIndex + 1 + TILEX;
		}

		RECT overlapRect{};
		int overlapCenterX;
		int overlapCenterY;
		bool overlapNum0 = false;
		bool overlapNum1 = false;
		bool overlapNum2 = false;

		bool check = false;
		for (auto & objectBW : *object)
		{
			RECT objectRect = objectBW->Get_Rect();
			auto objectCast = dynamic_cast<CTerrain*>(objectBW);
			Tile_Info tileInfo = objectCast->Get_Terrain_Info();
			int finishIndex = objectCast->Get_finishIndex();
			RECT temp;

				if (IntersectRect(&temp, &playerRect, &objectRect))
				{
					overlapRect = temp;

					if (nextIndex[0] == objectCast->Get_Terrain_Info().index)
						overlapNum0 = true;
					if (nextIndex[1] == objectCast->Get_Terrain_Info().index)
						overlapNum1 = true;
					if (nextIndex[2] == objectCast->Get_Terrain_Info().index)
						overlapNum2 = true;

					if (nextIndex[1] == tileInfo.index || nextIndex[1] == finishIndex)
					{
						if (objectCast->Get_Terrain_Info().isPush)
						{
							check = true;
							bool isNextObjectExist = false;
							switch (state)
							{
							case WALK_LEFT:
								if (((int)nextIndex[1]) % TILEX != 0)
								{
									for (auto nextnext : *object)
									{
										auto nextObjectCast = dynamic_cast<CTerrain*>(nextnext);
										if (nextIndex[1] - 1 == nextObjectCast->Get_Terrain_Info().index)
											isNextObjectExist = true;
									}

									if (isNextObjectExist)
										return OBJ_WALL;

									if (!objectCast->Get_Pushed())
									{
										objectBW->Set_State(state);
										objectCast->Set_Pushed(true);
										objectCast->Set_finish((tileInfo.centerX - TILECX * expansionSize) - TILECX * expansionSize / 2, tileInfo.pos.y, nextIndex[1] - 1);
									}


								}
								else
									return OBJ_WALL;
								break;
							case WALK_RIGHT:
								if (((int)nextIndex[1]) % TILEX != 14)
								{
									for (auto nextnext : *object)
									{
										auto nextObjectCast = dynamic_cast<CTerrain*>(nextnext);
										if (nextIndex[1] + 1 == nextObjectCast->Get_Terrain_Info().index)
											isNextObjectExist = true;
									}

									if (isNextObjectExist)
										return OBJ_WALL;

									if (!objectCast->Get_Pushed())
									{
										objectBW->Set_State(state);
										objectCast->Set_Pushed(true);
										objectCast->Set_finish((tileInfo.centerX + TILECX * expansionSize / 2), tileInfo.pos.y, nextIndex[1] + 1);
									}


								}
								else
									return OBJ_WALL;
								break;
							case WALK_UP:
								if (((int)nextIndex[1]) >= TILEX)
								{
									for (auto nextnext : *object)
									{
										auto nextObjectCast = dynamic_cast<CTerrain*>(nextnext);
										if (nextIndex[1] - TILEX == nextObjectCast->Get_Terrain_Info().index)
											isNextObjectExist = true;
									}

									if (isNextObjectExist)
										return OBJ_WALL;

									if (!objectCast->Get_Pushed())
									{
										objectBW->Set_State(state);
										objectCast->Set_Pushed(true);
										objectCast->Set_finish(tileInfo.pos.x, (tileInfo.centerY - TILECY * expansionSize) - TILECY * expansionSize / 2, nextIndex[1] - TILEX);
									}


								}
								else
									return OBJ_WALL;
								break;
							case WALK_DOWN:
								if (((int)nextIndex[1]) < TILEX*(TILEY - 1))
								{
									for (auto nextnext : *object)
									{
										auto nextObjectCast = dynamic_cast<CTerrain*>(nextnext);
										if (nextIndex[1] + TILEX == nextObjectCast->Get_Terrain_Info().index)
											isNextObjectExist = true;
									}

									if (isNextObjectExist)
										return OBJ_WALL;

									if (!objectCast->Get_Pushed())
									{
										objectBW->Set_State(state);
										objectCast->Set_Pushed(true);
										objectCast->Set_finish(tileInfo.pos.x, tileInfo.centerY + TILECY * expansionSize / 2, nextIndex[1] + TILEX);
									}


								}
								else
									return OBJ_WALL;
								break;
							}

							return OBJ_WALL;
						}
						else
						{
						overlapCenterX = tileInfo.centerX;
						overlapCenterY = tileInfo.centerY;
							continue;
						}
					}
					continue;
			}
		}

		if (!check)
		{
			switch (state)
			{
			case STAND:
				break;
			case WALK_LEFT:
				//if (overlapRect.right < player_object->Get_PlayerCenterX() && overlapRect.right!=0)
				if (overlapNum0 && !overlapNum1)
					player_object->Set_Pos(0, overLapSpeed);
				else if (overlapNum2 && !overlapNum1)
					player_object->Set_Pos(0, -overLapSpeed);
				else if (overlapNum1)
				{
					if (nextIndex[0] != -1 && playerRect.bottom < overlapCenterY + TILECY * expansionSize / 3)
						player_object->Set_Pos(0, -0.5);
					else if (nextIndex[2] != -1 && playerRect.top > overlapCenterY - TILECY * expansionSize / 3)
						player_object->Set_Pos(0, 0.5);
					else
						return OBJ_WALL;
				}
				else
					return OBJ_NONE;
				break;
			case WALK_RIGHT:
				//if (overlapRect.left > player_object->Get_PlayerCenterX() && overlapRect.left != 0)
				if (overlapNum0 && !overlapNum1)
					player_object->Set_Pos(0, overLapSpeed);
				else if (overlapNum2 && !overlapNum1)
					player_object->Set_Pos(0, -overLapSpeed);
				else if (overlapNum1)
				{
					if (nextIndex[0] != -1 && playerRect.bottom < overlapCenterY + TILECY * expansionSize / 3)
						player_object->Set_Pos(0, -overLapSpeed);
					else if (nextIndex[2]!=-1 && playerRect.top > overlapCenterY - TILECY*expansionSize/3)
						player_object->Set_Pos(0, overLapSpeed);
					else
						return OBJ_WALL;
				}
				else
					return OBJ_NONE;
				break;
			case WALK_UP:
			//	if (overlapRect.bottom < player_object->Get_PlayerCenterY()&& overlapRect.bottom!=0)
				if (overlapNum0 && !overlapNum1)
					player_object->Set_Pos(overLapSpeed, 0);
				else if (overlapNum2 && !overlapNum1)
					player_object->Set_Pos(-overLapSpeed, 0);
				else if (overlapNum1)
				{
					if (nextIndex[0] != -1 && playerRect.right < overlapCenterX + TILECY * expansionSize / 3)
						player_object->Set_Pos(-overLapSpeed, 0);
					else if (nextIndex[0] != -1 && playerRect.left > overlapCenterX - TILECY * expansionSize / 3)
						player_object->Set_Pos(overLapSpeed, 0);
					else
						return OBJ_WALL;
				}
				else
					return OBJ_NONE;
				break;
			case WALK_DOWN:
				//if (overlapRect.top > player_object->Get_PlayerCenterY()&& overlapRect.top!=0)
				if (overlapNum0 && !overlapNum1)
					player_object->Set_Pos(overLapSpeed, 0);
				else if (overlapNum2 && !overlapNum1)
					player_object->Set_Pos(-overLapSpeed, 0);
				else if (overlapNum1)
					return OBJ_WALL;
				else
					return OBJ_NONE;
				break;
			default:
				break;
			}
			return OBJ_WALL;
		}
		return OBJ_NONE;
	}
}

int CCollision_Manager::Collision_WaterBall_Object(list<CGameObject*>* waterball, list<CGameObject*>* object)
{
	return 0;
}
