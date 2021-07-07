#include "framework.h"
#include "WaterBall.h"
#include "GameObject_Manager.h"
#include "Water.h"

CWaterBall::CWaterBall(int index)
	: m_animIndex(0)
	, m_time(3.f)
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
	m_time -= CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_time <= 0 || m_dead)
	{
		CGameObject_Manager::Get_Instance()->Get_Player()->Decrese_WaterBall();
		int waterLength = CGameObject_Manager::Get_Instance()->Get_Player()->Get_WaterLength();

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
}

void CWaterBall::Release_GameObject()
{
}
