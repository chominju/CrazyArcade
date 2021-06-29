#include "framework.h"
#include "Load_Manager.h"
#include "Tile.h"
#include "Box.h"
#include "Wall.h"


CLoad_Manager::CLoad_Manager()
{
}

CLoad_Manager::~CLoad_Manager()
{
}

// 	LoadTileData_Terrain(L"../Data/TerrainData.dat");
HRESULT CLoad_Manager::LoadTerrainData(const wstring & filePath)
{
	HANDLE file = CreateFile(filePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == file)
		return E_FAIL;

	DWORD byte = 0;
	Tile_Info * tile = nullptr;

	while (true)
	{
		tile = new Tile_Info;
		ReadFile(file, tile, sizeof(Tile_Info), &byte, nullptr);
		if (0 == byte)
		{
			Safe_Delete(tile);
			break;
		}

		CGameObject* object;
		switch (tile->objectKey)
		{
		case SCENE_OBJECT_ID::TILE:
			object = new CTile;
			break;

		case SCENE_OBJECT_ID::BOX:
			//object = new CBox;
			break;

		case SCENE_OBJECT_ID::WALL:
			//object = new CWall;
			break;
		default:
			break;
		};
	}
	CloseHandle(file);
	return S_OK;
}

HRESULT CLoad_Manager::LoadTextureData(const wstring & filePath)
{
	while (true)
	{
		wstring filePath;
		wstring objectKey;
		wstring stateKey;


	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_MULTI,
		L"../Resource/Tile/Tile%d.png",
		L"Terrain", L"Tile", 23)))
		return E_FAIL;
	}


	return S_OK;
}
