#include "framework.h"
#include "Terrain.h"

CTerrain::CTerrain()
	:m_isPushed(false)
	,m_finishIndex(-1)
{
}

CTerrain::~CTerrain()
{

}

HRESULT CTerrain::Ready_GameObject()
{
	return E_NOTIMPL;
}

int CTerrain::Update_GameObject()
{
	return 0;
}

void CTerrain::Late_Update_GameObject()
{
}

void CTerrain::Render_GameObject()
{
}

void CTerrain::Release_GameObject()
{
}
