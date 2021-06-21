#include "pch.h"
#include "framework.h"
#include "Terrain.h"
#include "MFCToolView.h"

CTerrain::CTerrain()
	:m_view(nullptr)
{
}

CTerrain::~CTerrain()
{
	Release_Terrain();
}

HRESULT CTerrain::Ready_Terrain()
{
	m_vecTile.reserve(TILECX * TILECY);
	Tile_Info* tile = nullptr;
	float x = 0.f;
	float y = 0.f;

	//x = float(2*TILECX);
	//y = float(2*TILECY);
	///*	x = float((j*TILECX) + ((i % 2)*(TILECX >> 1)));
	//	y = float(i*(TILECY >> 1));*/
	//tile = new Tile_Info;
	//tile->pos = { x,y,0.f };
	//tile->size = { 2.f, 2.f, 0.f };
	//tile->index = 0; // j + (i*TILEX);
	//tile->parentIndex = 0;
	//tile->drawID = 2;
	//tile->option = 0;
	//m_vecTile.emplace_back(tile);

	for (int i = 0; i < TILEX; ++i)
	{
		for (int j = 0; j < TILEY; j++)
		{
			x = float(j*TILECX * 1.5);
			y = float(i*TILECY * 1.5);
			/*	x = float((j*TILECX) + ((i % 2)*(TILECX >> 1)));
				y = float(i*(TILECY >> 1));*/
			tile = new Tile_Info;
			tile->pos = { x + TILECX * 0.75f, y + TILECY * 0.75f,0.f };
			tile->size = { 1.5f, 1.5f, 0.f };
			tile->index = j + (i*TILEX);
			tile->parentIndex = 0;
			tile->drawID = 2;
			tile->option = 0;
			m_vecTile.emplace_back(tile);
		}
	}

	//for (int i = 0; i < TILEX; ++i)
	//{
	//	for (int j = 0; j < TILEX; j++)
	//	{
	//		x = float(j*TILECX);
	//		y = float(i*TILECY);
	//	/*	x = float((j*TILECX) + ((i % 2)*(TILECX >> 1)));
	//		y = float(i*(TILECY >> 1));*/
	//		tile = new Tile_Info;
	//		tile->pos = { x,y,0.f };
	//		tile->size = { 1.f, 1.f, 0.f };
	//		tile->index = j + (i*TILEX);
	//		tile->parentIndex = 0;
	//		tile->drawID = 2;
	//		tile->option = 0;
	//		m_vecTile.emplace_back(tile);
	//	}
	//}

	return S_OK;
}

void CTerrain::Update_Terrain()
{
}

void CTerrain::Render_Terrain()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	DWORD size = m_vecTile.size();
	TCHAR szBuf[32]{};
	//m_vecTile[31]->drawID = 0;
	for (size_t i = 0; i < size; ++i)
	{
		const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Terrain", L"Tile", m_vecTile[i]->drawID);
		if (nullptr == textureInfo)
			return;
		float centerX = float(textureInfo->imageInfo.Width >> 1);
		float centerY = float(textureInfo->imageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_vecTile[i]->size.x, m_vecTile[i]->size.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vecTile[i]->pos.x - m_view->GetScrollPos(SB_HORZ), m_vecTile[i]->pos.y - m_view->GetScrollPos(SB_VERT), 0.f);
		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(centerX, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szBuf, L"%d", i);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

	}
}

void CTerrain::MiniRender_Terrain()
{
}

void CTerrain::Release_Terrain()
{
	for (auto & tile : m_vecTile)
		Safe_Delete(tile);

	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTerrain::Tile_Change_Terrain(const D3DXVECTOR3 & pos, const BYTE & drawID, const BYTE & option)
{
	int index = Get_TileIndex(pos);

	if (-1 == index)
		return;

	m_vecTile[index]->drawID = drawID;
	m_vecTile[index]->option = option;
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3 & pos)
{
	int size = m_vecTile.size();
	for (int i = 0; i < size; ++i)
	{
		if (IsPicking(pos, i))
			return i;
	}
	return -1;
}

bool CTerrain::IsPicking(const D3DXVECTOR3 & pos, const int index)
{
	D3DXVECTOR3 vertex[4] =
	{
		{ m_vecTile[index]->pos.x ,m_vecTile[index]->pos.y + (TILECY >> 1), 0.f },
		{ m_vecTile[index]->pos.x + (TILECX >> 1),m_vecTile[index]->pos.y , 0.f },
		{ m_vecTile[index]->pos.x ,m_vecTile[index]->pos.y - (TILECY >> 1), 0.f },
		{ m_vecTile[index]->pos.x - (TILECX >> 1) ,m_vecTile[index]->pos.y , 0.f }
	};

	// 챕터 2. 각 꼭지점에서 방향벡터를 구하자! 0 ->1 1 ->2 ...
	D3DXVECTOR3 vertexDir[4] =
	{
		vertex[1] - vertex[0],
		vertex[2] - vertex[1],
		vertex[3] - vertex[2],
		vertex[0] - vertex[3]
	};
	// 챕터3. 법선백터를 뽑아내자!!! 
	D3DXVECTOR3 normal[4] =
	{
		{-vertexDir[0].y, vertexDir[0].x, 0.f },
		{ -vertexDir[1].y, vertexDir[1].x, 0.f },
		{ -vertexDir[2].y, vertexDir[2].x, 0.f },
		{ -vertexDir[3].y, vertexDir[3].x, 0.f },
	};

	//챕터 4. 각 꼭지점에서 마우스를 바라보는 방향벡터를 구해라!!!! 
	D3DXVECTOR3 mouseDir[4] =
	{
		// 마우스 - 꼭지점 영번째 
		pos - vertex[0],
		pos - vertex[1],
		pos - vertex[2],
		pos - vertex[3],
	};

	// 이제 나 다 구했다!!! 

	for (int i = 0; i < 4; ++i)
	{
		if (0 < D3DXVec3Dot(&mouseDir[i], &normal[i]))
			return false;
	}
	return true;
}
