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



typedef struct tagOBJECTINFO

{
	enum OBJID { BACKGROUND, PLAYER, MONSTER, PLAYER_BULLET, ENEMY_BULLET, EFFECT, UI, OBJID_END };
	enum BULLET_TYPE { NORMAL, GUIDE, SHOTGUN, TYPE_END };
#ifdef _AFX
	CString cstrName;
	CString cstrObjectImage_ObjectKey;
	CString cstrObjectImage_Path;
	CString cstrDeathAnimImage_ObjectKey;
	CString cstrDeathAnimImage_StateKey;
#else
	wstring wstrName;
	wstring wstrObjectImage_ObjectKey;
	wstring wstrObjectImage_Path;
	wstring wstrDeathAnimImage_ObjectKey;
	wstring wstrDeathAnimImage_StateKey;
#endif
	float	fMaxHp;
	float	fAtk;
	float	fAtkRatio;
	float	fMoveSpeed;
	BYTE	eObjId;
//Bullet
	bool	bDestructable;
	BYTE	eBulletType;
//�����϶�
	float	fShotGunAngle;
	int		iShotGunCount;

}OBJECTINFO;



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


typedef struct tagMatrixInfo
{
	D3DXVECTOR3 mat[MATID::END];
}MATRIXINFO;



typedef struct tagPlacementInfo
{	
	tagPlacementInfo() :m_bRender(false){}
	RENDERID::ID eRenderID;
	MATRIXINFO m_tMatInfo;	//��Ʈ������ ������ ���͸� ����
	bool m_bRender;			//�׷����� ������ ����
	
#ifdef _AFX
	CString wstrPrefabName;
	CString wstrName;
	CString wstrObjectKey;
	CString	wstrFilePath;
#else
	wstring wstrName;
	wstring wstrObjectKey;
	wstring wstrFilePath;
	wstring wstrObjName;
#endif
}PLACEMENT;