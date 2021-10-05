#include "framework.h"
#include "Boss.h"
#include "Time_Manager.h"
#include "WaterBall.h"
#include "GameObject_Manager.h"
#include "Collision_Manager.h"
#include "Water.h"

CBoss::CBoss()
	: m_bossSummonsTime(0.f)
	, m_waterBallTime(0.f)
	, m_waterTime(1.f)
	, m_isStopMove(false)
	, m_timerFix(5.f)
	, m_hitTime(0.5)
	, waterBallNum(3)
	, m_horizonWaterTime(0.f)
	, m_waterNum(5)
	, m_waterNum2(6)
	, m_waterX(4)
	, m_waterY(4)
	, m_isBossHpHalf(false)
{
}

CBoss::~CBoss()
{
}


HRESULT CBoss::Ready_GameObject()
{
	m_info.pos.x = 360.0f + STARTX;
	m_info.pos.y = 60.0f + STARTY;
	m_info.dir = { 1.f,1.f,0.f };
	m_info.size = { expansionSize ,expansionSize,0.f };

	m_curState = CHARACTER_STATE::STAND;
	m_preState = m_curState;
	//m_curState = CHARACTER_STATE::WALK_LEFT;
	//m_curState = CHARACTER_STATE::WALK_RIGHT;
	//m_curState = CHARACTER_STATE::WALK_UP;
	//m_curState = CHARACTER_STATE::DIE;
	m_frame = { 0.f,5.f };

	m_bossSizeX = 120 * expansionSize;
	m_bossSizeY = 160 * expansionSize;
	m_playerableInfo.speed = 1.f;
	m_objectKey = L"Boss";
	m_stateKey = L"Boss_Down";

	m_bossHp = 50;

	m_isBossMonster = true;

	m_invincibilityTime = 0.5f;

	Set_Rect();

	ZeroMemory(&pattern, sizeof(pattern));

	m_invincibilityTime = 0.2f;

	return S_OK;
}

int CBoss::Update_GameObject()
{
	if (m_isDeadAnimFin)
		return OBJ_DEAD;

	if (m_dead && !m_isDeadAnim)
	{
		m_stateKey = L"Boss_Die";
		m_frame.frameStart = 0;
		m_playerableInfo.speed = 0.1;
		m_isDeadAnim = true;
	}

	CCollision_Manager::Collision_Boss_WaterBall(this, &CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL), m_bossHp);

	if (m_hitTime <= 0)
	{
		m_curState = WALK_DOWN;
		m_hitTime = 0.5;
	}
	
	else if(!m_dead && !m_isDeadAnim)
	{
		Set_Rect();
		Boss_Pattern();
		Boss_Action();
	}
	return S_OK;
}

void CBoss::Late_Update_GameObject()
{
	Set_InvincibilityCurrentTime();
	FrameMove(0.7f * m_playerableInfo.speed * 2);
}

void CBoss::Render_GameObject()
{
	const Texture_Info* pTexInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(m_objectKey, m_stateKey, m_frame.frameStart);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_info.size.x, m_info.size.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_info.pos.x, m_info.pos.y, 0.f);
	matWorld = matScale * matTrans;
	//float fCenterX = pTexInfo->imageInfo.Width >> 1;
	//float fCenterY = pTexInfo->imageInfo.Height >> 1;



	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->texture, nullptr, &D3DXVECTOR3(0/*fCenterX, fCenterY*/, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	

	pTexInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"HpBar");
	if (nullptr == pTexInfo)
		return;
	D3DXMatrixTranslation(&matTrans, m_info.pos.x+5 ,m_info.pos.y , 0.f);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	matWorld = matScale * matTrans;


	RECT temp;
	temp.left = 0;
	temp.right = (171 * m_bossHp)/50;
	temp.top = 0;
	temp.bottom = 13;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->texture, &temp, &D3DXVECTOR3(0, 0, 0.f), &D3DXVECTOR3(0, 0, 0.f), D3DCOLOR_ARGB(255, 255, 255, 255));



	//CGraphic_Device::Get_Instance()->Get_Sprite()->End();

	//D3DXVECTOR2 vLine[5]{ {(float)m_rect.left, (float)m_rect.top}, {(float)m_rect.right, (float)m_rect.top},{(float)m_rect.right, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.bottom },{ (float)m_rect.left, (float)m_rect.top }, };
	//CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 0, 0, 0));

	//CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CBoss::Release_GameObject()
{
}

void CBoss::Set_Rect()
{
	m_player_centerX = m_info.pos.x + m_bossSizeX / 2;
	m_player_centerY = m_info.pos.y + m_bossSizeY / 2;

	m_rect.left = m_info.pos.x;
	m_rect.top = (m_info.pos.y + m_bossSizeY) - (TILECY * expansionSize * 3);

	m_rect.right = m_player_centerX + m_bossSizeX / 2;
	m_rect.bottom = m_info.pos.y + m_bossSizeY;



	indexX = ((m_rect.left + m_rect.right - STARTX*2) / 2) / (TILECX * expansionSize);
	indexY = ((m_rect.bottom + m_rect.top - STARTY*2) / 2/*- 38*/) / (TILECY * expansionSize);

	m_LocationIndex = indexX + indexY * TILEX;


	rectLeftIndex = indexX - 1;
	rectRightIndex = indexX + 1;
	rectUpIndex = indexY - 1;
	rectDownIndex = indexY + 1;



}

void CBoss::Set_SeneorRect()
{
}

void CBoss::Change_CurrentState()
{
}

void CBoss::Boss_Pattern()
{
	if (m_curState != HIT)
		m_bossSummonsTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	else
		return;

	if (m_bossHp < 25 && !m_isBossHpHalf)
	{
		m_waterTime = 0;
		Boss_AroundWater(2);
		m_waterTime = 0;
		Boss_AroundWater(4);
		m_waterTime = 0;
		Boss_AroundWater(6);
		m_waterTime = 0;
		Boss_AroundWater(8);
		m_waterTime = 0;
		Boss_AroundWater(10);

		m_isBossHpHalf = true;
	}


	/*if (m_bossSummonsTime < m_timerFix)
	{
		Boss_Move(2, 1);
	}*/

	if (!pattern[0] || !pattern[1])
	{
		m_horizonWaterTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_horizonWaterTime < 0.f)
		{
			m_horizonWaterTime = 1.f;
			Boss_HorizonWater(0, 14, 5);
		}
	}

	if (!pattern[0])
	{
		if (Boss_Move(2, 1))
			pattern[0] = true;
	}
	else if (!pattern[1])
	{
		if (Boss_Move(10, 1))
			pattern[1] = true;
	}
	else if (!pattern[2])
	{
		if (Boss_Move(6, 4))
			pattern[2] = true;
	}

	//물 광역기
	else if (!pattern[3])
	{
		m_isStopMove = true;
		m_curState = WALK_DOWN;
		if (Boss_AroundWater(5))
			pattern[3] = true;
	}
	else if (!pattern[4])
	{
		m_isStopMove = true;
		if (Boss_AroundWater(4))
			pattern[4] = true;
	}
	else if (!pattern[5])
	{
		m_isStopMove = true;
		if (Boss_AroundWater(3))
			pattern[5] = true;
	}
	else if (!pattern[6])
	{
		m_isStopMove = true;
		if (Boss_AroundWater(2))
			pattern[6] = true;
	}
	else if (!pattern[7])
	{
		m_isStopMove = true;
		if (Boss_AroundWater(2))
			pattern[7] = true;
	}
	else if (!pattern[8])
	{
		m_isStopMove = true;
		if (Boss_AroundWater(3))
			pattern[8] = true;
	}
	else if (!pattern[9])
	{
		m_isStopMove = true;
		if (Boss_AroundWater(4))
			pattern[9] = true;
	}
	else if (!pattern[10])
	{
		m_isStopMove = true;
		if (Boss_AroundWater(5))
			pattern[10] = true;
	}
	else if (!pattern[11])
	{
		m_isStopMove = true;
		if (Boss_AroundWater(6))
			pattern[11] = true;
	}

	// 물폭탄 사방향 (아래 위 왼쪽 오른쪽)
	else if (!pattern[12])
	{
		m_isStopMove = true;
		m_curState = WALK_DOWN;
		if (Boss_WaterBall())
			pattern[12] = true;
	}
	else if (!pattern[13])
	{
		m_isStopMove = true;
		m_curState = WALK_UP;
		if (Boss_WaterBall())
			pattern[13] = true;
	}
	else if (!pattern[14])
	{
		m_isStopMove = true;
		m_curState = WALK_LEFT;
		if (Boss_WaterBall())
			pattern[14] = true;
	}
	else if (!pattern[15])
	{
		m_isStopMove = true;
		m_curState = WALK_RIGHT;
		if (Boss_WaterBall())
			pattern[15] = true;
	}


	// 물방울 왼쪽 가로
	else if (!pattern[16])
	{
		m_horizonWaterTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_horizonWaterTime < 0.f)
		{
			m_horizonWaterTime = 0.5f;
			Boss_HorizonWater(0, 5, m_waterY);
			m_waterNum--;
			m_waterY++;
		}
		if (m_waterNum <= 0)
		{
			pattern[16] = true;
			m_waterNum = 5;
			m_waterY = 4;
		}
	}

	// 물방울 오른쪽 가로
	else if (!pattern[17])
	{
		m_horizonWaterTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_horizonWaterTime < 0.f)
		{
			m_horizonWaterTime = 0.5f;
			Boss_HorizonWater(9, TILEX - 1, m_waterY);
			m_waterNum--;
			m_waterY++;
		}
		if (m_waterNum <= 0)
		{
			pattern[17] = true;
			m_waterNum = 5;
			m_waterY = 4;
		}
	}

	// 물방울 위쪽 세로
	else if (!pattern[18])
	{
		m_horizonWaterTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_horizonWaterTime < 0.f)
		{
			m_horizonWaterTime = 0.5f;
			Boss_VerticalWater(0, 4, m_waterX +1);
			m_waterNum--;
			m_waterX++;
		}
		if (m_waterNum <= 0)
		{
			pattern[18] = true;
			m_waterNum = 5;
			m_waterX = 4;
		}
	}


	// 물방울 아래쪽 세로
	else if (!pattern[19])
	{
		m_horizonWaterTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_horizonWaterTime < 0.f)
		{
			m_horizonWaterTime = 0.5f;
			Boss_VerticalWater(8, TILEY - 1 , m_waterX + 1);
			m_waterNum--;
			m_waterX++;
		}
		if (m_waterNum <= 0)
		{
			pattern[19] = true;
			m_waterNum = 5;
			m_waterX = 4;
		}
	}


	// 물방울 움직이는 세로
	else if (!pattern[20])
	{
		m_horizonWaterTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_horizonWaterTime < 0.f)
		{
			m_horizonWaterTime = 0.3f;
			Boss_VerticalWater(3 + (6 - m_waterNum2), 4 + (6 - m_waterNum2), m_waterX); // 왼쪽 아래로 내려감
			Boss_VerticalWater(8 - (6 - m_waterNum2), 9 -  (6 - m_waterNum2), TILEX - m_waterX -1); // 오른쪽 위로 올라감
			m_waterNum2--;
		}
		if (m_waterNum2 <= 0)
		{
			pattern[20] = true;
			m_waterNum2 = 6;
			m_waterX = 4;
			m_waterY = 4;
			//pattern[21] = false;
		}
	}


	// 물방울 움직이는 가로
	else if (!pattern[21])
	{
		m_horizonWaterTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_horizonWaterTime < 0.f)
		{
			m_horizonWaterTime = 0.3f;
			Boss_HorizonWater(4 + (6 - m_waterNum2), 5 + (6 - m_waterNum2), TILEX - m_waterY - 1); // 왼쪽 오른쪽으로 감
			Boss_HorizonWater(9 - (6 - m_waterNum2), 10 - (6 - m_waterNum2), m_waterY - 1); // 오른쪽 왼쪽으로감
			m_waterNum2--;
		}
		if (m_waterNum2 <= 1)
		{
			pattern[21] = true;
			m_waterNum2 = 6;
			m_waterX = 4;
			//pattern[20] = false;
		}
	}

	else if (!pattern[22])
	{
		m_playerableInfo.speed = 1.5f;
		if (Boss_Move(6, 9))
			pattern[22] = true;
	}
	else if (!pattern[23])
	{
		if (Boss_Move(12, 9))
			pattern[23] = true;
	}
	else if (!pattern[24])
	{
		if (Boss_Move(12, 0))
			pattern[24] = true;
	}
	else if (!pattern[25])
	{
		if (Boss_Move(0, 0))
			pattern[25] = true;
	}
	else if (!pattern[26])
	{
		if (Boss_Move(0, 9))
			pattern[26] = true;
	}
	else if (!pattern[27])
	{
		if (Boss_Move(6, 9))
			pattern[27] = true;
	}
	else if (!pattern[28])
	{
		if (Boss_Move(6, 4))
		{
			for (int i = 3; i <= 28; i++)
			{
				pattern[i] = false;
			}
		}
	}
}

void CBoss::Boss_Action()
{
	switch (m_curState)
	{
	case WALK_LEFT:
		m_stateKey = L"Boss_Left";
		break;

	case WALK_RIGHT:
		m_stateKey = L"Boss_Right";
		break;

	case WALK_UP:
		m_stateKey = L"Boss_Up";
		break;

	case WALK_DOWN:
		m_stateKey = L"Boss_Down";
		break;
	case STAND:
		m_frame.frameStart = m_frame.frameEnd;
		break;
	case DIE:
		m_stateKey = L"Boss_Die";
	case HIT:
		m_stateKey = L"Boss_Hit"; 
		m_hitTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		break;
	default:

		break;
	}

	m_preState = m_curState;
}

bool CBoss::Boss_Move(int x, int y)
{
	int resultPosX = x * TILECX * expansionSize + STARTX;
	int resultPosY = y * TILECY * expansionSize + STARTY;

	if (resultPosX < m_info.pos.x)
	{
		m_info.pos.x -= m_playerableInfo.speed;
		m_curState = WALK_LEFT;
		//m_stateKey = L"Boss_Left";
		return false;
	}
	else if (resultPosX > m_info.pos.x)
	{
		m_info.pos.x += m_playerableInfo.speed;
		m_curState = WALK_RIGHT;
		//m_stateKey = L"Boss_Right";
		return false;
	}


	if (resultPosY < m_info.pos.y)
	{
		m_info.pos.y -= m_playerableInfo.speed;
		m_curState = WALK_UP;
		//m_stateKey = L"Boss_Up";
		return false;
	}
	else if (resultPosY > m_info.pos.y)
	{
		m_info.pos.y += m_playerableInfo.speed;
		m_curState = WALK_DOWN;
		//m_stateKey = L"Boss_Down";
		return false;
	}


	return true;
}

bool CBoss::Boss_WaterBall()
{
		if(m_curState!=HIT)
			m_waterBallTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_waterBallTime < 0)
		{
			m_waterBallTime = 1.f;
			indexX = (m_player_centerX - STARTX) / (TILECX * expansionSize);
			indexY = (m_player_centerY - STARTY) / (TILECY * expansionSize);

			bool isWaterBallable = true;
			switch (m_curState)
			{
			case WALK_LEFT:
				if (indexX - 2 >= 0)
					m_LocationIndex = ((indexY)* TILEX) + indexX - 3;
				else
					isWaterBallable = false;
				break;
			case WALK_RIGHT:
				if (indexX + 2 <= TILEX - 1)
					m_LocationIndex = ((indexY)* TILEX) + indexX + 3;
				else
					isWaterBallable = false;
				break;
			case WALK_UP:
				if (indexY - 3 >= 0)
					m_LocationIndex = ((indexY - 3) * TILEX) + indexX;
				else
					isWaterBallable = false;
				break;
			case WALK_DOWN:
				if (indexY + 3 <= TILEY - 1)
					m_LocationIndex = (indexY + 3) * TILEX + indexX;
				else
					isWaterBallable = false;
				break;
			default:
				break;
			}
			//m_LocationIndex = indexY * TILEX + indexX + TILEX * 2;

			if (isWaterBallable)
			{
				CWaterBall * waterBall = new CWaterBall(m_LocationIndex);
				waterBall->Set_IsBossMonster(true);
				waterBall->Set_WaterBallDir(m_curState);
				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATERBALL, waterBall);
				waterBallNum--;
			}
		}

		if (waterBallNum == 0)
		{
			waterBallNum = 3;
			return true;
		}
		else
			return false;
}

bool CBoss::Boss_AroundWater(int index)
{
	m_waterTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_waterTime < 0)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Water.mp3", CSoundMgr::EFFECT);

		rectLeftIndex = indexX - index;
		rectRightIndex = indexX + index;
		rectUpIndex = indexY - index;
		rectDownIndex = indexY + index;

		auto waterBallList = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);

		// 맨 윗줄 가로
		for (int i = rectLeftIndex; i <= rectRightIndex; i++)
		{
			if (i < 0 || i >= TILEX)
				continue;
			else
			{
				for (auto& waterBall : waterBallList)
				{
					if (waterBall->Get_LocationIndex() == (i + rectUpIndex * TILEX))
						waterBall->Set_Dead(true);
				}

				CWater* water = new CWater(i, rectUpIndex);
				water->Ready_GameObject();
				water->Set_StateKey(L"FlowCenter");
				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATER, water);
			}
		}

		// 젤 왼쪽 세로
		for (int i = rectUpIndex + 1; i <= rectDownIndex; i++)
		{
			if (i < 0 || i >= TILEX)
				continue;
			else
			{
				for (auto& waterBall : waterBallList)
				{
					if (waterBall->Get_LocationIndex() == (rectLeftIndex + i * TILEX))
						waterBall->Set_Dead(true);
				}

				CWater* water = new CWater(rectLeftIndex, i);
				water->Ready_GameObject();
				water->Set_StateKey(L"FlowCenter");
				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATER, water);
			}
		}

		// 젤 아래 가로
		for (int i = rectLeftIndex + 1; i <= rectRightIndex; i++)
		{
			if (i < 0 || i >= TILEX)
				continue;
			else
			{
				for (auto& waterBall : waterBallList)
				{
					if (waterBall->Get_LocationIndex() == (i + rectDownIndex * TILEX))
						waterBall->Set_Dead(true);
				}

				CWater* water = new CWater(i, rectDownIndex);
				water->Ready_GameObject();
				water->Set_StateKey(L"FlowCenter");
				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATER, water);
			}
		}


		// 젤 오른쪽 세로
		for (int i = rectUpIndex + 1; i <= rectDownIndex; i++)
		{
			if (i < 0 || i >= TILEX)
				continue;
			else
			{
				for (auto& waterBall : waterBallList)
				{
					if (waterBall->Get_LocationIndex() == (rectRightIndex + i * TILEX))
						waterBall->Set_Dead(true);
				}

				CWater* water = new CWater(rectRightIndex, i);
				water->Ready_GameObject();
				water->Set_StateKey(L"FlowCenter");
				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATER, water);
			}
		}
		m_waterTime = 1.f;
		return true;
	}
	else
		return false;

}

void CBoss::Boss_HorizonWater(int startX, int endX, int indexY)
{
	CSoundMgr::Get_Instance()->PlaySound(L"Water.mp3", CSoundMgr::EFFECT);
	for (int i = startX; i <= endX; i++)
	{
		if (i < 0 || i >= TILEX)
			continue;
		else
		{
			auto waterBallList = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);

			for (auto& waterBall : waterBallList)
			{
				if (waterBall->Get_LocationIndex() == (i + indexY * TILEX))
					waterBall->Set_Dead(true);
			}

			CWater* water = new CWater(i, indexY);
			water->Ready_GameObject();
			water->Set_StateKey(L"FlowCenter");
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATER, water);
		}
	}
}

void CBoss::Boss_VerticalWater(int startY, int endY, int indexX)
{
	CSoundMgr::Get_Instance()->PlaySound(L"Water.mp3", CSoundMgr::EFFECT);

	
	for (int i = startY; i <= endY; i++)
	{
		if (i < 0 || i >= TILEY)
			continue;
		else
		{
			auto waterBallList = CGameObject_Manager::Get_Instance()->Get_Object(OBJECT_ID::WATERBALL);

			for (auto& waterBall : waterBallList)
			{
				if (waterBall->Get_LocationIndex() == (indexX + i * TILEX))
					waterBall->Set_Dead(true);
			}

			CWater* water = new CWater(indexX, i);
			water->Ready_GameObject();
			water->Set_StateKey(L"FlowCenter");
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::WATER, water);
		}
	}
}
	