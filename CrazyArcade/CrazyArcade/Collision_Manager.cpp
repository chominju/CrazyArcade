#include "framework.h"
#include "Collision_Manager.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "Water.h"
#include "Item.h"
#include "WaterBall.h"
CGameObject* CCollision_Manager::pushObject = nullptr;
CGameObject* CCollision_Manager::pushWater = nullptr;

CCollision_Manager::CCollision_Manager()
	//:pushObject(nullptr)
{
}

CCollision_Manager::~CCollision_Manager()
{
}

int CCollision_Manager::Collision_Player_Object(list<CGameObject*>* player, list<CGameObject*>* object, CHARACTER_STATE state)
{
	if (state == CHARACTER_STATE::STAND)
	{
		if (pushObject != nullptr && !pushObject->Get_IsDead())
		{
			dynamic_cast<CTerrain*>(pushObject)->ResetPushTime();
			pushObject = nullptr;
		}
		/*for (auto & objectBW : *object)
		{
			auto objectCast = dynamic_cast<CTerrain*>(objectBW);
			objectCast->ResetPushTime();
		}*/
	}

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

		float nextIndex[5] = {-1,-1,-1,-1,-1};
		float playerIndex = player_object->Get_LocationIndex();

		if (state == CHARACTER_STATE::WALK_LEFT)
		{
			// 왼쪽 위
			if (playerIndex - 1 - TILEX >= 0)
				nextIndex[0] = playerIndex - 1 - TILEX;

			nextIndex[1] = playerIndex - 1;

			// 왼쪽 아래
			if (playerIndex - 1 + TILEX < TILEX * TILEY)
				nextIndex[2] = playerIndex - 1 + TILEX;

			// 위
			if (playerIndex - TILEX >= 0)
				nextIndex[3] = playerIndex - TILEX;

			// 아래
			if(playerIndex + TILEX< TILEX*TILEY)
				nextIndex[4] = playerIndex + TILEX;


		}

		if (state == CHARACTER_STATE::WALK_RIGHT)
		{
			if (playerIndex + 1 - TILEX >= 0)
				nextIndex[0] = playerIndex + 1 - TILEX;

			nextIndex[1] = playerIndex + 1;
			
			if (playerIndex + 1 + TILEX < TILEX * TILEY)
				nextIndex[2] = playerIndex + 1 + TILEX;

			// 위
			if (playerIndex - TILEX >= 0)
				nextIndex[3] = playerIndex - TILEX;

			// 아래
			if (playerIndex + TILEX < TILEX*TILEY)
				nextIndex[4] = playerIndex + TILEX;

		}

		if (state == CHARACTER_STATE::WALK_UP)
		{
			if (playerIndex - TILEX - 1 >= 0)
				nextIndex[0] = playerIndex -1 - TILEX;

			nextIndex[1] = playerIndex - TILEX;


			if (playerIndex - TILEX + 1 < TILEX)
				nextIndex[2] = playerIndex + 1 - TILEX;

			if ((int)playerIndex % TILEX != 0)
				nextIndex[3] = playerIndex - 1;

			if ((int)playerIndex % TILEX != TILEX - 1)
				nextIndex[4] = playerIndex + 1;

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
		bool overlapNum3 = false;
		bool overlapNum4 = false;

		bool check = false;
		for (auto & objectBW : *object)
		{
			RECT objectRect = objectBW->Get_Rect();
			CTerrain* objectCast = dynamic_cast<CTerrain*>(objectBW);
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
					if (nextIndex[3] == objectCast->Get_Terrain_Info().index)
						overlapNum3 = true;
					if (nextIndex[4] == objectCast->Get_Terrain_Info().index)
						overlapNum4 = true;

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
									auto waterBallTemp = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);
									for (auto nextnext : waterBallTemp)
									{
										if (nextIndex[1] - 1 == nextnext->Get_LocationIndex())
											isNextObjectExist = true;
									}


									if (isNextObjectExist)
										return OBJ_WALL;

									if (!objectCast->Get_Pushed())
									{
										objectCast->Set_PushTime();
										pushObject = objectCast;
										if (objectCast->Get_PushTime() >= 0.5)
										{
											objectBW->Set_State(state);
											objectCast->Set_Pushed(true);
											objectCast->Set_finish((tileInfo.centerX - TILECX * expansionSize) - TILECX * expansionSize / 2, tileInfo.pos.y, nextIndex[1] - 1);
											objectCast->ResetPushTime();
										}
					
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
									auto waterBallTemp = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);
									for (auto nextnext : waterBallTemp)
									{
										if (nextIndex[1] - 1 == nextnext->Get_LocationIndex())
											isNextObjectExist = true;
									}
									if (isNextObjectExist)
										return OBJ_WALL;

									if (!objectCast->Get_Pushed())
									{
										objectCast->Set_PushTime();
										pushObject = objectCast;
										if (objectCast->Get_PushTime() >= 0.5)
										{
										objectBW->Set_State(state);
										objectCast->Set_Pushed(true);
										objectCast->Set_finish((tileInfo.centerX + TILECX * expansionSize / 2), tileInfo.pos.y, nextIndex[1] + 1);
										objectCast->ResetPushTime();
										}
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
									auto waterBallTemp = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);
									for (auto nextnext : waterBallTemp)
									{
										if (nextIndex[1] - 1 == nextnext->Get_LocationIndex())
											isNextObjectExist = true;
									}
									if (isNextObjectExist)
										return OBJ_WALL;

									if (!objectCast->Get_Pushed())
									{
										objectCast->Set_PushTime();
										pushObject = objectCast;
										if (objectCast->Get_PushTime() >= 0.5)
										{
										objectBW->Set_State(state);
										objectCast->Set_Pushed(true);
										objectCast->Set_finish(tileInfo.pos.x, (tileInfo.centerY - TILECY * expansionSize) - TILECY * expansionSize / 2, nextIndex[1] - TILEX);
										objectCast->ResetPushTime();
										}
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
									auto waterBallTemp = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);
									for (auto nextnext : waterBallTemp)
									{
										if (nextIndex[1] - 1 == nextnext->Get_LocationIndex())
											isNextObjectExist = true;
									}
									if (isNextObjectExist)
										return OBJ_WALL;

									if (!objectCast->Get_Pushed())
									{
										objectCast->Set_PushTime();
										pushObject = objectCast;
										if (objectCast->Get_PushTime() >= 0.5)
										{
											objectBW->Set_State(state);
											objectCast->Set_Pushed(true);
											objectCast->Set_finish(tileInfo.pos.x, tileInfo.centerY + TILECY * expansionSize / 2, nextIndex[1] + TILEX);
											objectCast->ResetPushTime();
										}
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
					else
					{
						objectCast->ResetPushTime();
						continue;
					}
 
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
					if (overlapNum1 && playerRect.bottom < overlapCenterY + TILECY * expansionSize / 4)
						player_object->Set_Pos(0, -0.5);
					else if (!overlapNum2 && playerRect.top > overlapCenterY - TILECY * expansionSize / 4)
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
					if (overlapNum1 && playerRect.bottom < overlapCenterY + TILECY * expansionSize / 4)
						player_object->Set_Pos(0, -overLapSpeed);
					else if (overlapNum2 && playerRect.top > overlapCenterY - TILECY*expansionSize/4)
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
					if (overlapNum1 && playerRect.right < overlapCenterX + TILECY * expansionSize / 4)
						player_object->Set_Pos(-overLapSpeed, 0);
					else if (overlapNum2 && playerRect.left > overlapCenterX - TILECY * expansionSize / 4)
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
				{
					if (overlapNum1 && playerRect.right < overlapCenterX + TILECY * expansionSize / 4)
						player_object->Set_Pos(-overLapSpeed, 0);
					else if (overlapNum2 && playerRect.left > overlapCenterX - TILECY * expansionSize / 4)
						player_object->Set_Pos(overLapSpeed, 0);
					else
						return OBJ_WALL;
				}
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

int CCollision_Manager::Collision_Player_WaterBall(list<CGameObject*>* player, list<CGameObject*>* waterBall, CHARACTER_STATE state)
{
	if (state == CHARACTER_STATE::STAND)
		return OBJ_NONE;

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

		float nextIndex[3];
		float playerIndex = player_object->Get_LocationIndex();

		if (state == CHARACTER_STATE::WALK_LEFT)
		{
			// 왼쪽 위
			if (playerIndex - 1 - TILEX >= 0)
				nextIndex[0] = playerIndex - 1 - TILEX;

			nextIndex[1] = playerIndex - 1;

			// 왼쪽 아래
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
				nextIndex[0] = playerIndex - 1 - TILEX;

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

		RECT overlapRect[3]{};
		int overlapCenterX;
		int overlapCenterY;
		bool overlapNum[3] = { false,false,false };

		bool check = false;
		for (auto & waterBallIndex : *waterBall)
		{
			RECT objectRect = waterBallIndex->Get_Rect();
			int waterBall_index = waterBallIndex->Get_LocationIndex();

			//int finishIndex = objectCast->Get_finishIndex();
			RECT temp;

			if (IntersectRect(&temp, &playerRect, &objectRect))
			{

				if (nextIndex[0] == waterBall_index)
				{
					overlapNum[0] = true;
					overlapRect[0] = objectRect;
				}
				if (nextIndex[1] == waterBall_index)
				{
					overlapNum[1] = true;
					overlapRect[1] = objectRect;
					check = true;

					if (player_object->Get_IsKick())
					{
						/*if (objectCast->Get_Terrain_Info().isPush)
						{*/
							bool isNextObjectExist = false;
							bool isNextWaterBallExist = false;
							int indexX = dynamic_cast<CWaterBall*>(waterBallIndex)->Get_indexX();
							int indexY = dynamic_cast<CWaterBall*>(waterBallIndex)->Get_indexY();

							int tempIndexX = indexX;
							int tempIndexY = indexY;

							auto obejctTemp = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT);

							int kickIndex = 0;
							int endIndex = -1;
							switch (state)
							{
							case WALK_LEFT:
								while (tempIndexX > 0 && !isNextWaterBallExist)
								{
									kickIndex++;
									for (auto nextnext : *waterBall)
									{
										if (nextIndex[1] - kickIndex == nextnext->Get_LocationIndex())
										{
											endIndex = nextIndex[1] - kickIndex + 1;
											isNextWaterBallExist = true;
											break;
										}
									}
									tempIndexX--;
								}

								tempIndexX = indexX;
								tempIndexY = indexY;
								kickIndex = 0;
								while (tempIndexX > 0 && !isNextObjectExist)
								{
									kickIndex++;
									for (auto nextnext : obejctTemp)
									{
										if (nextIndex[1] - kickIndex == nextnext->Get_LocationIndex())
										{
											if (endIndex < nextIndex[1] - kickIndex + 1)
											{
												endIndex = nextIndex[1] - kickIndex + 1;
												isNextObjectExist = true;
												break;
											}
										}
									}
									tempIndexX--;
								}

								if (!isNextObjectExist &&!isNextWaterBallExist)
									endIndex = waterBallIndex->Get_LocationIndex() - indexX;

								if (!waterBallIndex->Get_Pushed())
								{
									waterBallIndex->Set_PushTime();
									pushWater = waterBallIndex;
									if (waterBallIndex->Get_PushTime() >= 0.3)
	 	 							{
										waterBallIndex->Set_State(state);
										waterBallIndex->Set_Pushed(true);

										int endIndexY = endIndex / TILEX;
										int endIndexX = endIndex % TILEX;

										waterBallIndex->Set_finish((endIndexX * TILECX * expansionSize) + (TILECX * expansionSize / 2), waterBallIndex->Get_Object_Info().pos.y, endIndex);
										waterBallIndex->ResetPushTime();
									}

									//if (endIndex == -1)
								}



								break;
							case WALK_RIGHT:
								endIndex = 1000;
								while (tempIndexX > 0 && !isNextWaterBallExist)
								{
									kickIndex++;
									for (auto nextnext : *waterBall)
									{
										if (nextIndex[1] + kickIndex == nextnext->Get_LocationIndex())
										{
											endIndex = nextIndex[1] + kickIndex - 1;
											isNextWaterBallExist = true;
											break;
										}
									}
									tempIndexX--;
								}

								tempIndexX = TILEX - indexX;
								tempIndexY = indexY;
								kickIndex = 0;
								while (tempIndexX > 0 && !isNextObjectExist)
								{
									kickIndex++;
									for (auto nextnext : obejctTemp)
									{
										if (nextIndex[1] + kickIndex == nextnext->Get_LocationIndex())
										{
											if (endIndex > nextIndex[1] + kickIndex - 1)
											{
												endIndex = nextIndex[1] + kickIndex - 1;
												isNextObjectExist = true;
												break;
											}
										}
									}
									tempIndexX--;
								}

								if (!isNextObjectExist && !isNextWaterBallExist)
									endIndex = waterBallIndex->Get_LocationIndex() + (TILEX - indexX - 1);

								if (!waterBallIndex->Get_Pushed())
								{
									waterBallIndex->Set_PushTime();
									pushWater = waterBallIndex;
									if (waterBallIndex->Get_PushTime() >= 0.3)
									{
										waterBallIndex->Set_State(state);
										waterBallIndex->Set_Pushed(true);

										int endIndexY = endIndex / TILEX;
										int endIndexX = endIndex % TILEX;

										waterBallIndex->Set_finish((endIndexX * TILECX * expansionSize) + (TILECX * expansionSize / 2), waterBallIndex->Get_Object_Info().pos.y, endIndex);
										waterBallIndex->ResetPushTime();
									}

									//if (endIndex == -1)
								}
								break;
							case WALK_UP:
								while (tempIndexY > 0 && !isNextWaterBallExist)
								{
									kickIndex +=TILEX;
									for (auto nextnext : *waterBall)
									{
										if (nextIndex[1] - kickIndex == nextnext->Get_LocationIndex())
										{
											endIndex = nextIndex[1] - kickIndex + TILEX;
											isNextWaterBallExist = true;
											break;
										}
									}
									tempIndexY--;
								}

								tempIndexX = indexX;
								tempIndexY = indexY;
								kickIndex = 0;
								while (tempIndexY > 0 && !isNextObjectExist)
								{
									kickIndex += TILEX;
									for (auto nextnext : obejctTemp)
									{
										if (nextIndex[1] - kickIndex == nextnext->Get_LocationIndex())
										{
											if (endIndex < nextIndex[1] - kickIndex + TILEX)
											{
												endIndex = nextIndex[1] - kickIndex + TILEX;
												isNextObjectExist = true;
												break;
											}
										}
									}
									tempIndexY--;
								}

								if (!isNextObjectExist && !isNextWaterBallExist)
									endIndex = indexX;

								if (!waterBallIndex->Get_Pushed())
								{
									waterBallIndex->Set_PushTime();
									pushWater = waterBallIndex;
									if (waterBallIndex->Get_PushTime() >= 0.3)
									{
										waterBallIndex->Set_State(state);
										waterBallIndex->Set_Pushed(true);

										int endIndexY = endIndex / TILEX;
										int endIndexX = endIndex % TILEX;

										waterBallIndex->Set_finish(waterBallIndex->Get_Object_Info().pos.x , (endIndexY * TILECY * expansionSize) + (TILECY * expansionSize), endIndex);
										waterBallIndex->ResetPushTime();
									}

									//if (endIndex == -1)
								}



								break;
							case WALK_DOWN:
								endIndex = 999;
								tempIndexX = indexX;
								tempIndexY = TILEY - indexY;
								while (tempIndexY > 0 && !isNextWaterBallExist)
								{
									kickIndex += TILEX;
									for (auto nextnext : *waterBall)
									{
										if (nextIndex[1] + kickIndex == nextnext->Get_LocationIndex())
										{
											endIndex = nextIndex[1] + kickIndex - TILEX;
											isNextWaterBallExist = true;
											break;
										}
									}
									tempIndexY--;
								}

								tempIndexX = indexX;
								tempIndexY = TILEY - indexY;
								kickIndex = 0;
								while (tempIndexY > 0 && !isNextObjectExist)
								{
									kickIndex += TILEX;
									for (auto nextnext : obejctTemp)
									{
										if (nextIndex[1] + kickIndex == nextnext->Get_LocationIndex())
										{
											if (endIndex > nextIndex[1] + kickIndex - TILEX)
											{
												endIndex = nextIndex[1] + kickIndex - TILEX;
												isNextObjectExist = true;
												break;
											}
										}
									}
									tempIndexY--;
								}

								if (!isNextObjectExist && !isNextWaterBallExist)
									endIndex = indexX + (TILEY -1)*TILEX;

								if (!waterBallIndex->Get_Pushed())
								{
									waterBallIndex->Set_PushTime();
									pushWater = waterBallIndex;
									if (waterBallIndex->Get_PushTime() >= 0.3)
									{
										waterBallIndex->Set_State(state);
										waterBallIndex->Set_Pushed(true);

										int endIndexY = endIndex / TILEX;
										int endIndexX = endIndex % TILEX;

										waterBallIndex->Set_finish(waterBallIndex->Get_Object_Info().pos.x, (endIndexY * TILECY * expansionSize) + (TILECY * expansionSize), endIndex);
										waterBallIndex->ResetPushTime();
									}

									//if (endIndex == -1)
								}
								
								break;
							default:
								break;
							}

							return OBJ_WALL;
						/*}
						else
							return OBJ_WALL;*/
 					}
					else
						return OBJ_WALL;
				}
				if (nextIndex[2] == waterBall_index)
				{
					overlapNum[2] = true;
					overlapRect[2] = objectRect;
				}


			}
		}

		if (!check)
		{
			switch (state)
			{
			case WALK_LEFT:
				if (overlapNum[0])
				{
					if (overlapRect[0].bottom > playerRect.top)
						return OBJ_WALL;
					else
						return OBJ_NONE;
				}
				if (overlapNum[1])
				{
					if (overlapRect[1].top < playerRect.bottom)
						return OBJ_WALL;
					else
						return OBJ_NONE;
				}
				break;
			case WALK_RIGHT:
				if (overlapNum[0])
				{
					if (overlapRect[0].bottom > playerRect.top)
						return OBJ_WALL;
					else
						return OBJ_NONE;
				}
				if (overlapNum[1])
				{
					if (overlapRect[1].top < playerRect.bottom)
						return OBJ_WALL;
					else
						return OBJ_NONE;
				}
				break;
			case WALK_UP:
				if (overlapNum[0])
				{
					if (overlapRect[0].right > playerRect.left)
						return OBJ_WALL;
					else
						return OBJ_NONE;
				}
				if (overlapNum[1])
				{
					if (overlapRect[1].left < playerRect.right)
						return OBJ_WALL;
					else
						return OBJ_NONE;
				}
				break;
			case WALK_DOWN:
				if (overlapNum[0])
				{
					if (overlapRect[0].bottom > playerRect.top)
						return OBJ_WALL;
					else
						return OBJ_NONE;
				}
				if (overlapNum[1])
				{
					if (overlapRect[1].top < playerRect.bottom)
						return OBJ_WALL;
					else
						return OBJ_NONE;
				}
				break;
			default:
				break;
			}
			return OBJ_NONE;
		}
	}
}

int CCollision_Manager::Collision_Player_Water(list<CGameObject*>* player, list<CGameObject*>* water)
{
	for (auto playerObj : *player)
	{
		if(!dynamic_cast<CPlayer*>(playerObj)->Get_IsInvincibility())
		{ 
			for (auto waterObj : *water)
			{
				if (!dynamic_cast<CWater*>(waterObj)->Get_IsBoxDestroy())
				{
					RECT temp;
					RECT waterRect = waterObj->Get_Rect();
					RECT playerRect = playerObj->Get_Rect();

					playerRect.left += 5;
					playerRect.top += 5;
					playerRect.right -= 5;
					playerRect.bottom -= 5;

					if (IntersectRect(&temp, &playerRect, &waterRect))
					{
						int x = temp.right - temp.left;
						int y = temp.bottom - temp.top;

						if (x*y > TILECX * TILECY * expansionSize * expansionSize / 2)
						{
							if (playerObj->Get_IsRide())
							{
								playerObj->Set_Ride(false);
								dynamic_cast<CPlayer*>(playerObj)->Set_StateKey(L"WalkDown");
								dynamic_cast<CPlayer*>(playerObj)->Set_IsInvincibility(true);
								return 0;
							}

							//dynamic_cast<CPlayer*>(playerObj)->Set_IsTrappeed(true);
							//playerObj->Set_Dead(true);
						}
					}
				}
			}
		}
	}
	return 0;
}

int CCollision_Manager::Collision_Player_Item(list<CGameObject*>* player, list<CGameObject*>* item)
{
	for (auto playerObj : *player)
	{
		for (auto itemObj : *item)
		{
			RECT temp;
			RECT itemRect = itemObj->Get_Rect();
			RECT playerRect = playerObj->Get_Rect();
			itemRect.left += 25;
			itemRect.top += 25;
			itemRect.right -= 25;
			itemRect.bottom -= 25;

			playerRect.left += 10;
			playerRect.top += 10;
			playerRect.right -= 10;
			playerRect.bottom -= 10;

			if (IntersectRect(&temp, &playerRect, &itemRect))
			{
				if(dynamic_cast<CItem*>(itemObj)->Get_ItemData().isUserUse)
					dynamic_cast<CPlayer*>(playerObj)->Manual_Item(dynamic_cast<CItem*>(itemObj)->Get_ItemData());
				else
					dynamic_cast<CPlayer*>(playerObj)->Auto_Item(dynamic_cast<CItem*>(itemObj)->Get_ItemData() , dynamic_cast<CItem*>(itemObj)->Get_Object_Info());
				itemObj->Set_Dead(true);
			}
		}
	}
	return 0;
}

int CCollision_Manager::Collision_Item_Water(list<CGameObject*>* item, list<CGameObject*>* water)
{
	for (auto itemObj : *item)
	{
		for (auto waterObj : *water)
		{
			RECT temp;
			if (IntersectRect(&temp, &itemObj->Get_Rect(), &waterObj->Get_Rect()))
				if (!dynamic_cast<CWater*>(waterObj)->Get_IsBoxDestroy())
					itemObj->Set_Dead(true);
		}
	}
	return 0;
}

int CCollision_Manager::Collision_Item_Object(list<CGameObject*>* item, list<CGameObject*>* object)
{
	for (auto itemObj : *item)
	{
		for (auto Obj : *object)
		{
			RECT temp;
			if (IntersectRect(&temp, &itemObj->Get_Rect(), &Obj->Get_Rect()))
				itemObj->Set_Dead(true);
		}
	}
	return 0;
}
