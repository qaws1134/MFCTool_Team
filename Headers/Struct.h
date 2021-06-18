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