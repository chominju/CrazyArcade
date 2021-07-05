#pragma once

typedef struct
{
	LPDIRECT3DTEXTURE9 texture; // 텍스쳐 한장을 보관하기 위한 컴객체
	D3DXIMAGE_INFO imageInfo;
}Texture_Info;


typedef struct
{
	//D3DXVECTOR3 pos;
	//D3DXVECTOR3 size;
	//int index;
	//int parentIndex;
	//int objectKey;
	//int stateKey;
	//BYTE drawID;
	//BYTE option;


	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	int index;
	float centerX;
	float centerY;
	int objectKey;
	int stateKey;
	BYTE drawID;
	int parentIndex;
	bool isCollision;
	bool isPush;
	bool isDestroy;

}Tile_Info;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	float centerX;
	float centerY;
	// wstring objectKey;
	// wstring stateKey;
	BYTE drawID;
	int parentIndex;
	bool isCollision;
	
	float speedUp;
	bool isWaterBall;
	bool isWaterLengthUp;
	bool isKick;
	bool isRide;
	bool isShield;
	bool isRevival;


}Item_Info;

typedef struct
{
	bool isKick;
	bool isRide;
	bool isShield;
	bool isRevival;
	float WaterBallMax;
	float WaterBallCurrentMax;
	float WaterBallCurrent;
	float speed;
	float WaterLength;
	float WaterLengthMax;
}Player_Info;

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