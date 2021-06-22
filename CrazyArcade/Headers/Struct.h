#pragma once

typedef struct
{
	LPDIRECT3DTEXTURE9 texture; // 텍스쳐 한장을 보관하기 위한 컴객체
	D3DXIMAGE_INFO imageInfo;
}Texture_Info;


typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	int index;
	int parentIndex;
	wstring ObjectKey;
	wstring StateKey;
	BYTE drawID;
	BYTE option;
}Tile_Info;

typedef struct
{
#ifdef _AFX
	CString name;
#else
	wstring name;
#endif	// _AFX

	int atk;
	int def;
	BYTE job;
	BYTE item;

};