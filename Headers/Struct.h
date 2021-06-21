#pragma once
typedef struct tagTEXINFO
{
	LPDIRECT3DTEXTURE9 pTexture; // HBITMAP ?? 

	D3DXIMAGE_INFO tImageInfo; // �̹����� ���� ������ ������ ���� ���� . 
}TEXINFO;
typedef struct tagTile
{
	D3DXVECTOR3 vPos; 
	D3DXVECTOR3 vSize; 
	BYTE byDrawID; 
	BYTE byOption; 
}TILE;

typedef struct tagUnitInfo
{
	// �ٽ��ѹ� ���������� ������ �ȿ� ������ ������ ��� ���� �ٽ� �����غ�. 
#ifdef _AFX
	CString wstrName; 
#else
	wstring wstrName; 
#endif // _AFX
	int		iAtt; 
	int		iDef; 
	BYTE	byJob; 
	BYTE	byItem; 
}UNITINFO;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vSize;
	//D3DXVECTOR2; 
	//D3DXVECTOR4; 

}INFO;


typedef struct tagBulletData
{
	enum TYPE { NORMAL, GUIDE, SHOTGUN, TYPE_END };
#ifdef _AFX
	CString cstrName;
	CString cstrBulletImage_ObjectKey;
	CString cstrAnimImage_ObjectKey;
	CString cstrAnimImage_StateKey;
#else
	wstring wstrName;
	wstring wstrBulletImage_ObjectKey;
	wstring wstrAnimImage_ObjectKey;
	wstring wstrAnimImage_StateKey;
#endif
	int		iAtk;
	float	fSpeed;
	bool	bDestructable;
	BYTE	eType;
//�����϶�
	float	fAngle;
	int		iCount;

}BULLETDATA;



typedef struct tagAnimationInfo
{
#ifdef _AFX
	CString wstrObjectKey;
	CString wstrStateKey;
	CString	wstrFilePath;
#else
	wstring wstrObjectKey;
	wstring wstrStateKey;
	wstring wstrFilePath;
#endif
	float	fPlay_Speed;
	int		iMax_Index;
	bool	bLoop;
	bool	bIsSingle;

}ANIMATION;

