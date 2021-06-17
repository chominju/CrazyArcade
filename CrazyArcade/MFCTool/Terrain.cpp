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
	if(FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_MULTI,
		L"../Resource/", 10)))
		return E_FAIL;

	m_vecTile.reserve(TILECX * TILECY);
	Tile_Info* tile = nullptr;
	float x = 0.f;
	float y = 0.f;


}

void CTerrain::Update_Terrain()
{
}

void CTerrain::Render_Terrain()
{
}

void CTerrain::MiniRender_Terrain()
{
}

void CTerrain::Release_Terrain()
{
}

void CTerrain::Tile_Change_Terrain(const D3DXVECTOR3 & pos, const BYTE & drawID, const BYTE & option)
{
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3 & pos)
{
	return 0;
}

bool CTerrain::IsPicking(const D3DXVECTOR3 & pos, const int index)
{
	return false;
}
