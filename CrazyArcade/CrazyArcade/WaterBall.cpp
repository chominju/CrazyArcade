#include "framework.h"
#include "WaterBall.h"
#include "GameObject_Manager.h"
#include "Water.h"

CWaterBall::CWaterBall()
{
}

CWaterBall::CWaterBall(int index)
	: m_animIndex(0)
	, m_time(3.f)
	, m_isBossMonster(false)
	, m_bossState(WALK_DOWN)
	, m_isWaterBallMove(true)
	, m_isCheckIndex(false)
	, m_isTimerWaterBall(false)
	, m_isBossMonsterHit(false)
{
	m_LocationIndex = index;
	indexY = index / TILEX;
	indexX = index % TILEX;

	m_info.pos.x = STARTX + indexX * TILECX * expansionSize;
	m_info.pos.y = STARTY + indexY * TILECY * expansionSize;
	m_info.size = { expansionSize,expansionSize ,expansionSize };

	m_frame.frameStart = 0;
	m_frame.frameEnd = 3;
}

CWaterBall::~CWaterBall()
{
}

HRESULT CWaterBall::Ready_GameObject()
{
	return E_NOTIMPL;
}

int CWaterBall::Update_GameObject()
{
	if (!m_isWaterBallMove && m_isBossMonster)
		m_time -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	else if(!m_isBossMonster)
		m_time -= CTime_Manager::Get_Instance()->Get_DeltaTime();

	Move_BossWaterBall();

	if ((m_time <= 0 || m_dead)&& !m_isPushed)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Water.mp3", CSoundMgr::EFFECT);

		if (m_finishIndex != -1)
			m_LocationIndex = m_finishIndex;
		m_finishIndex = -1;
		indexX = (int)m_LocationIndex % TILEX;
		indexY = (int)m_LocationIndex / TILEX;

		int waterLength = 0;
		if (!m_isBossMonster)
		{
			CGameObject_Manager::Get_Instance()->Get_Player()->Decrese_WaterBall();
			waterLength = CGameObject_Manager::Get_Instance()->Get_Player()->Get_WaterLength();
		}
		else
			waterLength = 2;

		auto waterBallList = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);

		bool dirBlockX[2] = { false , false };
		bool dirBlockY[2] = { false , false };

		for (int i = 1; i <= waterLength; i++)
		{
			int DirX[2] = { -1,-1};
			int DirY[2] = { -1,-1};

			DirX[0] = indexX - i;
			DirX[1] = indexX + i;

			for (int j = 0; j < 2; ++j)
			{
				bool isBoxDestory = false;
				if (DirX[j] >= 0 && DirX[j] < TILEX)
				{
					if (CGameObject_Manager::Get_Instance()->IsExistWater(DirX[j] + indexY * TILEX))
					{
						dirBlockX[j] = true;
						isBoxDestory = true;
						continue;
					}

					if (!dirBlockX[j])
					{
						if (CGameObject_Manager::Get_Instance()->IsExistObject(DirX[j] + indexY * TILEX))
						{
							dirBlockX[j] = true;
							isBoxDestory = true;
							//continue;
						}

						for (auto& waterBall : waterBallList)
						{
							if (waterBall->Get_LocationIndex() == (DirX[j] + indexY * TILEX))
								waterBall->Set_Dead(true);
						}

						CWater* water = new CWater(DirX[j] , indexY);
						water->Ready_GameObject();
						water->Set_IsBoxDestroy(isBoxDestory);
						switch (j)
						{
						case 0:
							if(i==waterLength)
								water->Set_StateKey(L"FlowLeft_End");
							else
								water->Set_StateKey(L"FlowLeft");
							break;
						case 1:
							if (i == waterLength)
								water->Set_StateKey(L"FlowRight_End");
							else
								water->Set_StateKey(L"FlowRight");
							break;
						default:
							break;
						}
						
						CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATER, water);
					}
				}
			}

			DirY[0] = indexY - i;
			DirY[1] = indexY + i;

			for (int j = 0; j < 2; ++j)
			{
				bool isBoxDestory = false;
				if (DirY[j] >= 0 && DirY[j] < TILEY)
				{
					if (!dirBlockY[j])
					{
						if (CGameObject_Manager::Get_Instance()->IsExistWater(indexX + DirY[j] * TILEX))
						{
							dirBlockY[j] = true;
							isBoxDestory = true;
							continue;
						}

						if (CGameObject_Manager::Get_Instance()->IsExistObject(indexX + DirY[j] * TILEX))
						{
							dirBlockY[j] = true;
							isBoxDestory = true;
							//continue;
						}

						for (auto& waterBall : waterBallList)
						{
							if (waterBall->Get_LocationIndex() == (indexX + DirY[j] * TILEX))
								waterBall->Set_Dead(true);
						}

						CWater* water = new CWater(indexX, DirY[j]);
						water->Ready_GameObject();
						water->Set_IsBoxDestroy(isBoxDestory);
						switch (j)
						{
						case 0:
							if (i == waterLength)
								water->Set_StateKey(L"FlowUp_End");
							else
								water->Set_StateKey(L"FlowUp");
							break;
						case 1:
							if (i == waterLength)
								water->Set_StateKey(L"FlowDown_End");
							else
								water->Set_StateKey(L"FlowDown");
							break;
						default:
							break;
						}

						CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATER, water);

					}
				}
			}
		}
		CWater* water = new CWater(indexX, indexY);
		water->Ready_GameObject();
		water->Set_StateKey(L"FlowCenter");
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATER, water);

		return OBJ_DEAD;
	}

	if (m_LocationIndex != m_finishIndex)
	{
		if ((m_isPushed && !m_dead) || m_isBossMonsterHit)
		{
			if (m_curState == WALK_LEFT || m_curState == WALK_RIGHT)
			{
				if (m_finishX == m_info.pos.x || m_isBossMonsterHit)
				{
					m_isPushed = false;
					//m_info.centerX = m_info.pos.x + TILECX * expansionSize / 2;
					m_LocationIndex = m_finishIndex;
					m_finishIndex = -1;
					m_time = 3.f;
					indexY = m_LocationIndex / TILEX;
					indexX = (int)m_LocationIndex % TILEX;
				}
			}
			if (m_curState == WALK_UP || m_curState == WALK_DOWN)
			{
				if (m_finishY == m_info.pos.y || m_isBossMonsterHit)
				{
					m_isPushed = false;
					//m_tileInfo.centerY = m_tileInfo.pos.y + TILECY * expansionSize / 2;
					m_LocationIndex = m_finishIndex;
					m_finishIndex = -1;
					m_time = 3.f;
					indexY = m_LocationIndex / TILEX;
					indexX = (int)m_LocationIndex % TILEX;

				}
			}

			if (m_isPushed)
			{
				switch (m_curState)
				{
				case WALK_LEFT:
					m_info.pos.x -= 10;
					break;
				case WALK_RIGHT:
					m_info.pos.x += 10;
					break;
				case WALK_UP:
					m_info.pos.y -= 10;
					break;
				case WALK_DOWN:
					m_info.pos.y += 10;
					break;
				}
			}
		}
	}
	else
		m_isPushed = false;

	
	return 0;
}

void CWaterBall::Late_Update_GameObject()
{
	FrameMove(1);
	Set_Rect();
}

void CWaterBall::Render_GameObject()
{
		D3DXMATRIX matScale, matTrans, matWorld;
		TCHAR szBuf[32]{};
		wstring objectKey;
		wstring stateKey;

		const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"WaterBall", L"Idle", m_frame.frameStart);
		if (nullptr == textureInfo)
			return;
		//float centerX = float(textureInfo->imageInfo.Width >> 1);
		//float centerY = float(textureInfo->imageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_info.size.x, m_info.size.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_info.pos.x, m_info.pos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



	/*	CGraphic_Device::Get_Instance()->Get_Sprite()->End();

		D3DXVECTOR2 vLine[5]{ {(float)m_rect.left, (float)m_rect.top}, {(float)m_rect.right, (float)m_rect.top},{(float)m_rect.right, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.top }, };
		CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

		CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);*/

}

void CWaterBall::Release_GameObject()
{
}

void CWaterBall::Move_BossWaterBall()
{
	if (m_isBossMonster && m_isWaterBallMove)
	{
		if (!m_isCheckIndex)
		{
			int nextIndex = -1;
			switch (m_bossState)
			{
			case WALK_LEFT:
				if (indexX - 1 >= 0)
				{
					m_finishX = indexX - 1;
					m_finishY = indexY;
					nextIndex = (indexY * TILEX) + indexX - 1;
				}
				break;
			case WALK_RIGHT:
				if (indexX + 1 < TILEX)
				{
					m_finishX = indexX + 1;
					m_finishY = indexY;
					nextIndex = (indexY * TILEX) + indexX + 1;
				}
				break;
			case WALK_UP:
				if (indexY - 1 >= 0)
				{
					m_finishX = indexX;
					m_finishY = indexY - 1;
					nextIndex = ((indexY - 1) * TILEX) + indexX;
				}
				break;
			case WALK_DOWN:
				if (indexY + 1 < TILEY)
				{
					m_finishX = indexX;
					m_finishY = indexY + 1;
					nextIndex = ((indexY + 1) * TILEX) + indexX;
				}
				break;
			default:
				break;
			}

			if (nextIndex == -1)
			{
				m_isWaterBallMove = false;
				return;
			}

			auto playerList = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::PLAYER);
			auto objectList = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::OBEJCT);
			auto waterBallList = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);

			bool isObjectExist = false;

			for (auto player : playerList)
			{
				if (player->Get_LocationIndex() == nextIndex)
				{
					isObjectExist = true;
					m_isWaterBallMove = false;
					break;
				}
			}

			for (auto object : objectList)
			{
				if (object->Get_LocationIndex() == nextIndex)
				{
					isObjectExist = true;
					m_isWaterBallMove = false;
					break;
				}
			}

			for (auto waterBall : waterBallList)
			{
				if (waterBall->Get_LocationIndex() == nextIndex)
				{
					isObjectExist = true;
					m_isWaterBallMove = false;
					break;
				}
			}

			if (!isObjectExist)
			{
				m_isCheckIndex = true;
				m_finishIndex = nextIndex;
				indexX = m_finishX;
				indexY = m_finishY;
				m_finishX = m_finishX * TILECX * expansionSize + STARTX;
				m_finishY = m_finishY * TILECY * expansionSize + STARTY;

			}
		}
		else // m_isCheckIndex == true
		{
			switch (m_bossState)
			{
			case WALK_LEFT:
				if (m_info.pos.x > m_finishX)
					m_info.pos.x -= 10;
				else
				{
					m_LocationIndex = m_finishIndex;
					m_isCheckIndex = false;
				}
				break;
			case WALK_RIGHT:
				if (m_info.pos.x < m_finishX)
					m_info.pos.x += 10;
				else
				{
					m_LocationIndex = m_finishIndex;
					m_isCheckIndex = false;
				}
				break;
			case WALK_UP:
				if (m_info.pos.y > m_finishY)
					m_info.pos.y -= 10;
				else
				{
					m_LocationIndex = m_finishIndex;
					m_isCheckIndex = false;
				}
				break;
			case WALK_DOWN:
				if (m_info.pos.y < m_finishY)
					m_info.pos.y += 10;
				else
				{
					m_LocationIndex = m_finishIndex;
					m_isCheckIndex = false;
				}
				break;
			default:
				break;
			}
		}


	}
}
