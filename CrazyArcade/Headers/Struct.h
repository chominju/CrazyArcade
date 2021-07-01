#pragma once

typedef struct
{
	LPDIRECT3DTEXTURE9 texture; // �ؽ��� ������ �����ϱ� ���� �İ�ü
	D3DXIMAGE_INFO imageInfo;
}Texture_Info;


typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	int index;
	float centerX;
	float centerY;
	wstring objectKey;
	wstring stateKey;
	BYTE drawID;
	int parentIndex;
	bool isCollision;
	bool isPush;
}Tile_Info;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	float centerX;
	float centerY;
	wstring objectKey;
	wstring stateKey;
	BYTE drawID;
	bool isCollision;
	
	float speedUp;
	int waterBallonUp;
	int waterLengthUp;
	bool isKick;
	bool isRide;
	bool isRevival;



}Item_Info;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;
	D3DXVECTOR3 size;

}Object_Info;

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

typedef struct
{
	float frameStart;
	float frameEnd;
}Frame;