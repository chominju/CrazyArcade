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

void CTerrain::Set_Tile(Tile_Info * tile)
{
	m_vecTile[tile->index] = tile;
}

void CTerrain::Set_Obj(Tile_Info * obj)
{
	m_vecObj.push_back(obj);
}

void CTerrain::Set_Respawn(Tile_Info * obj)
{
	m_vecRespawnTile.push_back(obj);
}

HRESULT CTerrain::Ready_Terrain()
{
	m_vecTile.reserve(TILEX * TILECY);
	m_vecObj.reserve(TILEX*TILECY);
	Tile_Info* tile = nullptr;
	Tile_Info* obj = nullptr;
	float x = 0.f;
	float y = 0.f;

	int tileChange=2;

	for (int i = 1; i <= TILEY; ++i)
	{
		for (int j = 0; j < TILEX; j++)
		{
			x = float(j * TILECX * expansionSize + 30);
			y = float(i * TILECY *expansionSize);
			tile = new Tile_Info;
			tile->pos = { x , y ,0.f };
			tile->size = { expansionSize,expansionSize,expansionSize };

			tile->index = j + ((i - 1) * TILEX);
			tile->centerX = x + (TILECX * expansionSize / 2);
			tile->centerY = y + (TILECY * expansionSize / 2);
			tile->objectKey = 1;
			tile->stateKey = 1;
			tile->drawID = tileChange;
			tile->parentIndex = 0;
			tile->isCollision = false;
			tile->isPush = false;
			tile->isDestroy = false;
			m_vecTile.emplace_back(tile);

			tileChange *= 2;
			if (tileChange > 4)
				tileChange = 2;

			//obj = new Tile_Info;
			//obj->pos = { x  , y ,0.f };
			//obj->size = { expansionSize,expansionSize,expansionSize };

			//obj->index = j + ((i - 1) * TILEX);
			//obj->centerX = x + (TILEX * expansionSize / 2);
			//obj->centerY = y + (TILEY * expansionSize / 2);

			//obj->parentIndex = 0;
			//m_vecObj.emplace_back(obj);
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

	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Ui");
	if (nullptr == textureInfo)
		return;

	D3DXMatrixScaling(&matScale, 1.5,1.5, 0.f);
	D3DXMatrixTranslation(&matTrans, 0, 0, 0.f);
	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



	for (size_t i = 0; i < size; ++i)
	{
		//wstring objectKey(&m_vecTile[i]->ObjectKey[0], &m_vecTile[i]->ObjectKey[sizeof(m_vecTile[i]->ObjectKey)]);
		//wstring stateKey(&m_vecTile[i]->StateKey[0], &m_vecTile[i]->StateKey[sizeof(m_vecTile[i]->StateKey)]);
		wstring objectKey;
		wstring stateKey;
		if (m_vecTile[i]->objectKey == 1)
			objectKey = L"Terrain";
		if (m_vecTile[i]->stateKey == 1)
			stateKey = L"Tile";


		const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_vecTile[i]->drawID);
		if (nullptr == textureInfo)
			continue;
		float centerX = float(textureInfo->imageInfo.Width >> 1);
		float centerY = float(textureInfo->imageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_vecTile[i]->size.x, m_vecTile[i]->size.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vecTile[i]->pos.x - m_view->GetScrollPos(SB_HORZ), m_vecTile[i]->pos.y - m_view->GetScrollPos(SB_VERT), 0.f);
		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0,0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szBuf, L"%d", i);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

	}

	size = m_vecObj.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecObj[i] == NULL)
			continue;

		//wstring objectKey(&m_vecObj[i]->ObjectKey[0], &m_vecObj[i]->ObjectKey[sizeof(m_vecObj[i]->ObjectKey)]);
		//wstring stateKey(&m_vecObj[i]->StateKey[0], &m_vecObj[i]->StateKey[sizeof(m_vecObj[i]->StateKey)]);

		wstring objectKey;
		wstring stateKey;
		if (m_vecObj[i]->objectKey == 2)
			objectKey = L"Box";
		if (m_vecObj[i]->stateKey == 2)
			stateKey = L"BoxObj";

		if (m_vecObj[i]->objectKey == 3)
			objectKey = L"Wall";
		if (m_vecObj[i]->stateKey == 3)
			stateKey = L"WallObj";

		const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_vecObj[i]->drawID);
		if (nullptr == textureInfo)
			continue;
		float centerX = float(textureInfo->imageInfo.Width >> 1);
		float centerY = float(textureInfo->imageInfo.Height) - float(TILECY);

		D3DXMatrixScaling(&matScale, m_vecObj[i]->size.x, m_vecObj[i]->size.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vecObj[i]->pos.x - m_view->GetScrollPos(SB_HORZ), m_vecObj[i]->pos.y - m_view->GetScrollPos(SB_VERT), 0.f);
		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	D3DXMatrixScaling(&matScale, 1.5, 1.5, 0.f);
	D3DXMatrixTranslation(&matTrans, 0, 0, 0.f);
	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	size = m_vecRespawnTile.size();
	for (size_t i = 0; i < size; ++i)
	{
		//wstring objectKey(&m_vecTile[i]->ObjectKey[0], &m_vecTile[i]->ObjectKey[sizeof(m_vecTile[i]->ObjectKey)]);
		//wstring stateKey(&m_vecTile[i]->StateKey[0], &m_vecTile[i]->StateKey[sizeof(m_vecTile[i]->StateKey)]);
		wstring objectKey;
		wstring stateKey;
		if (m_vecRespawnTile[i]->objectKey == 1)
			objectKey = L"Terrain";
		if (m_vecRespawnTile[i]->stateKey == 1)
			stateKey = L"Tile";


		const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_vecRespawnTile[i]->drawID);
		if (nullptr == textureInfo)
			continue;
		float centerX = float(textureInfo->imageInfo.Width >> 1);
		float centerY = float(textureInfo->imageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_vecRespawnTile[i]->size.x, m_vecRespawnTile[i]->size.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vecRespawnTile[i]->pos.x - m_view->GetScrollPos(SB_HORZ), m_vecRespawnTile[i]->pos.y - m_view->GetScrollPos(SB_VERT), 0.f);
		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(/*centerX, centerY*/0, 0, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
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

	for (auto & obj : m_vecObj)
		Safe_Delete(obj);
	m_vecObj.clear();
	m_vecTile.shrink_to_fit();

	for (auto & respawn : m_vecRespawnTile)
		Safe_Delete(respawn);
	m_vecRespawnTile.clear();
	m_vecRespawnTile.shrink_to_fit();
}

void CTerrain::Tile_Change_Terrain(const D3DXVECTOR3 & pos, const wstring objectKey, const wstring stateKey, const BYTE & drawID)
{
	int index = Get_TileIndex(pos);

	if (-1 == index)
		return;

	int strSize1 = WideCharToMultiByte(CP_ACP, 0, objectKey.c_str(), -1, NULL, 0, NULL, NULL);
	int strSize2 = WideCharToMultiByte(CP_ACP, 0, stateKey.c_str(), -1, NULL, 0, NULL, NULL);

	if (objectKey == L"Terrain")
	{
		if (drawID == 1)
		{
			Tile_Info * newRespawnTile = new Tile_Info;
			
			newRespawnTile->pos.x = m_vecTile[index]->pos.x;
			newRespawnTile->pos.y = m_vecTile[index]->pos.y;
			newRespawnTile->pos.z = m_vecTile[index]->pos.z;

			newRespawnTile->size.x = m_vecTile[index]->size.x;
			newRespawnTile->size.y = m_vecTile[index]->size.y;
			newRespawnTile->size.z = m_vecTile[index]->size.z;

			newRespawnTile->index = index;

			newRespawnTile->centerX= m_vecTile[index]->centerX;
			newRespawnTile->centerY= m_vecTile[index]->centerY;
			newRespawnTile->objectKey = 1;
			newRespawnTile->stateKey = 1;
			newRespawnTile->drawID = drawID;
			newRespawnTile->parentIndex = 0;
			newRespawnTile->isCollision = false;
			newRespawnTile->isPush = false;
			newRespawnTile->isDestroy = false;

			m_vecRespawnTile.emplace_back(newRespawnTile);
		}
		else
		{
			if (m_vecTile[index] == NULL)
				m_vecTile[index] = new Tile_Info;


			m_vecTile[index]->drawID = drawID;
			m_vecTile[index]->objectKey = 1;
			m_vecTile[index]->stateKey = 1;
			m_vecTile[index]->index = index;
			//strcpy_s(m_vecTile[index]->ObjectKey,sizeof(m_vecTile[index]->ObjectKey), objectKey.c_str());
			//WideCharToMultiByte(CP_ACP, 0, objectKey.c_str(), -1, m_vecTile[index]->ObjectKey, strSize1, 0, 0);
			//WideCharToMultiByte(CP_ACP, 0, stateKey.c_str(), -1, m_vecTile[index]->StateKey, strSize2, 0, 0);
		}
	}
	else
	{
		for (auto & obj : m_vecObj)
		{
			if (obj->index == index)
			{
				if (objectKey == L"Box")
				{
					obj->objectKey = 2;
					obj->stateKey = 2;
					obj->isCollision = true;
					obj->isDestroy = true;
					obj->isPush = true;
				}
				else
				{
					obj->objectKey = 3;
					obj->stateKey = 3;
					obj->isCollision = true;
					obj->isDestroy = false;
					obj->isPush = false;
				}
				obj->drawID = drawID;
				obj->size.x = expansionSize;
				obj->size.y = expansionSize;
				obj->size.z = expansionSize;
				obj->index = index;
				obj->pos.x = m_vecTile[index]->pos.x;
				obj->pos.y = m_vecTile[index]->pos.y;
				obj->pos.z = m_vecTile[index]->pos.z;
				return;
			}
		}

		Tile_Info* newObj = new Tile_Info;
		if (objectKey == L"Box")
		{
			newObj->objectKey = 2;
			newObj->stateKey = 2;
			newObj->isCollision = true;
			newObj->isDestroy = true;
			newObj->isPush = true;
		}
		else
		{
			newObj->objectKey = 3;
			newObj->stateKey = 3;
			newObj->isCollision = true;
			newObj->isDestroy = false;
			newObj->isPush = false;
		}
		newObj->pos.x = m_vecTile[index]->pos.x;
		newObj->pos.y = m_vecTile[index]->pos.y;
		newObj->pos.z = m_vecTile[index]->pos.z;
		newObj->size.x = expansionSize;
		newObj->size.y = expansionSize;
		newObj->size.z = expansionSize;
		newObj->centerX = newObj->pos.x + (TILECX * expansionSize / 2);
		newObj->centerY = newObj->pos.y + (TILECY * expansionSize / 2);
		newObj->index = index;
		newObj->drawID = drawID;
		newObj->parentIndex = 1;
		m_vecObj.emplace_back(newObj);

		sort(m_vecObj.begin(), m_vecObj.end(), [&](Tile_Info* a, Tile_Info* b) {
			return a->index < b->index;
		});


	}
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
	//wstring objectKey(&m_vecTile[index]->ObjectKey[0], &m_vecTile[index]->ObjectKey[sizeof(m_vecTile[index]->ObjectKey)]);
	//wstring stateKey(&m_vecTile[index]->StateKey[0], &m_vecTile[index]->StateKey[sizeof(m_vecTile[index]->StateKey)]);

	wstring objectKey;
	wstring stateKey;
	if (m_vecTile[index]->objectKey == 1)
		objectKey = L"Terrain";
	if (m_vecTile[index]->stateKey == 1)
		stateKey = L"Tile";

	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(objectKey, stateKey, m_vecTile[index]->drawID);
	if (nullptr == textureInfo)
		return false;

	if (m_vecTile[index]->pos.x <= pos.x && pos.x < m_vecTile[index]->pos.x + TILECX)
		if (m_vecTile[index]->pos.y  <= pos.y && pos.y < m_vecTile[index]->pos.y +TILECY)
			return true;

	//if (m_vecTile[index]->pos.x - textureInfo->imageInfo.Width / 2*m_vecTile[index]->size.x <= pos.x && pos.x < m_vecTile[index]->pos.x + textureInfo->imageInfo.Width / 2 * m_vecTile[index]->size.x)
	//	if (m_vecTile[index]->pos.y - textureInfo->imageInfo.Height / 2 * m_vecTile[index]->size.y <= pos.y && pos.y < m_vecTile[index]->pos.y + textureInfo->imageInfo.Height / 2 * m_vecTile[index]->size.y)
	//		return true;
	

	//D3DXVECTOR3 vertex[4] =
	//{
	//	{ m_vecTile[index]->pos.x ,m_vecTile[index]->pos.y + (TILECY >> 1), 0.f },
	//	{ m_vecTile[index]->pos.x + (TILECX >> 1),m_vecTile[index]->pos.y , 0.f },
	//	{ m_vecTile[index]->pos.x ,m_vecTile[index]->pos.y - (TILECY >> 1), 0.f },
	//	{ m_vecTile[index]->pos.x - (TILECX >> 1) ,m_vecTile[index]->pos.y , 0.f }
	//};

	//// 챕터 2. 각 꼭지점에서 방향벡터를 구하자! 0 ->1 1 ->2 ...
	//D3DXVECTOR3 vertexDir[4] =
	//{
	//	vertex[1] - vertex[0],
	//	vertex[2] - vertex[1],
	//	vertex[3] - vertex[2],
	//	vertex[0] - vertex[3]
	//};
	//// 챕터3. 법선백터를 뽑아내자!!! 
	//D3DXVECTOR3 normal[4] =
	//{
	//	{-vertexDir[0].y, vertexDir[0].x, 0.f },
	//	{ -vertexDir[1].y, vertexDir[1].x, 0.f },
	//	{ -vertexDir[2].y, vertexDir[2].x, 0.f },
	//	{ -vertexDir[3].y, vertexDir[3].x, 0.f },
	//};

	////챕터 4. 각 꼭지점에서 마우스를 바라보는 방향벡터를 구해라!!!! 
	//D3DXVECTOR3 mouseDir[4] =
	//{
	//	// 마우스 - 꼭지점 영번째 
	//	pos - vertex[0],
	//	pos - vertex[1],
	//	pos - vertex[2],
	//	pos - vertex[3],
	//};

	//// 이제 나 다 구했다!!! 

	//for (int i = 0; i < 4; ++i)
	//{
	//	if (0 < D3DXVec3Dot(&mouseDir[i], &normal[i]))
	//		return false;
	//}
	return false;
}
