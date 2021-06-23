#pragma once
typedef struct tagTEXINFO
{
	LPDIRECT3DTEXTURE9 pTexture; // HBITMAP ?? 

	D3DXIMAGE_INFO tImageInfo; // 이미지에 대한 정보를 가지고 있을 변수 . 
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
	// 다시한번 강조하지만 저장할 안에 포인터 있으면 어떻게 될지 다시 생각해봐. 
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
//샷건일때
	float	fAngle;
	int		iCount;

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
	MATRIXINFO m_tMatInfo;	//매트릭스를 조정할 백터를 저장
	bool m_bRender;			//그려질지 말지를 결정
	OBJECTINFO tPrafabInfo;  //완성된 프리펩 데이터는 있을 수도 있고 없을 수도 있음
	
							  	   
	//싱글텍스쳐는 저장 될 수도 있고 안될 수도 있음
#ifdef _AFX
	CString wstrObjectKey;
	CString	wstrFilePath;
#else
	wstring wstrObjectKey;
	wstring wstrFilePath;
#endif
}PLACEMENT;