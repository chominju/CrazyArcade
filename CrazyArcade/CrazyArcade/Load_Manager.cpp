#include "framework.h"
#include "Load_Manager.h"
#include "Tile.h"
#include "Box.h"
#include "Wall.h"
#include "Terrain.h"
#include "GameObject_Manager.h"
#include <fstream>

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

	while (true)
	{
		DWORD byte = 0;

		Tile_Info tile;
		//tile = new Tile_Info;
		ReadFile(file, &tile, sizeof(Tile_Info), &byte, nullptr);
		if (0 == byte)
			break;

		CGameObject* object;
		switch (tile.objectKey)
		{
		case SCENE_OBJECT_ID::TILE:
			object = new CTile;
			object->Set_RenderID(RENDER_ID::RENDER_TILE);
			dynamic_cast<CTerrain*>(object)->Set_Terrain_Info(tile);
			dynamic_cast<CTerrain*>(object)->Set_Info_Pos();
			if (dynamic_cast<CTerrain*>(object)->Get_Terrain_Info().drawID == 1)
				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::RESPAWN_TILE, object);
			else
				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::SCENE_TILE, object);
			break;

		case SCENE_OBJECT_ID::BOX:
			object = new CBox;
			dynamic_cast<CTerrain*>(object)->Set_Terrain_Info(tile);
			dynamic_cast<CTerrain*>(object)->Set_Info_Pos();
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::OBEJCT, object);
			break;

		case SCENE_OBJECT_ID::WALL:
			object = new CWall;
			dynamic_cast<CTerrain*>(object)->Set_Terrain_Info(tile);
			dynamic_cast<CTerrain*>(object)->Set_Info_Pos();
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECT_ID::OBEJCT, object);
			break;
		default:
			break;
		};
	}
	CloseHandle(file);
	return S_OK;
}

HRESULT CLoad_Manager::LoadTextureData(const string & filePath)
{
	ifstream readFile;
	readFile.open(filePath);

	if (readFile.is_open())
	{
		while (!readFile.eof())
		{

			string tempFilePath;
			string tempObjectKey;
			string tempStateKey;
			string indexString;

			getline(readFile, tempFilePath);
			getline(readFile, tempObjectKey);
			getline(readFile, tempStateKey);
			getline(readFile, indexString);

			wstring filePath;
			wstring objectKey;
			wstring stateKey;
			int index;

			filePath.assign(tempFilePath.begin(), tempFilePath.end());
			objectKey.assign(tempObjectKey.begin(), tempObjectKey.end());
			stateKey.assign(tempStateKey.begin(), tempStateKey.end());
			index = stoi(indexString);

			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_MULTI,
				filePath,
				objectKey, stateKey, index)))
				return E_FAIL;
		}
	}
	else
		return false;

	readFile.close();

	return S_OK;

}

HRESULT CLoad_Manager::LoadItemData(const wstring & filePath)
{
	HANDLE file = CreateFile(filePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == file)
		return E_FAIL;

	while (true)
	{
		DWORD byte = 0;

		Item_Info item;
		//tile = new Tile_Info;
		ReadFile(file, &item, sizeof(Item_Info), &byte, nullptr);
		if (0 == byte)
			break;

		CGameObject_Manager::Get_Instance()->Add_ItemData(item);
	}
	CloseHandle(file);
	return S_OK;
}
