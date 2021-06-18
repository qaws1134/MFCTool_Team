#pragma once
class CGraphic_Device
{
	DECLARE_SINGLETON(CGraphic_Device)
private:
	CGraphic_Device();
	~CGraphic_Device();
	void operator=(const CGraphic_Device& rhs) {} 
	CGraphic_Device(const CGraphic_Device& rhs) {}; 
public:
	LPD3DXLINE			Get_Line()		{ return m_pLine;	}
	LPD3DXSPRITE		Get_Sprite()	{ return m_pSprite;  }
	LPD3DXFONT			Get_Font()		{ return m_pFont;  }
	LPDIRECT3DDEVICE9	Get_Device()	{ return m_pDevice; }
public:
	HRESULT Ready_Graphic_Device(); 
	void Release_Graphic_Device(); 

public:
	void Render_Begin(); 
	void Render_End(HWND hWnd = nullptr); 
private:
	// ��ġ�� �ʱ�ȭ �ϴ� �Ϸ��� ���� 
	//1. ��ġ�� ������ ������ �İ�ü��� ���� �����ؾߵ�. 
	//2. ��ġ�� ������ �����ϰ� 
	//3. �� ������ ���� ��ġ�� �����ϴ� �İ�ü�� ������ ���̴�. 
	
	//compenent oBject model �� ���� . 
	//���� ���� 4��¥�� 2�� ¥�� ���ΰ� ���. �ϳ��� �ϼ�ǰ�� �����ݾ� ���� ? 
	//  �� ���صƾ�� ?? 
	//
	LPDIRECT3D9			m_pSDK; // ��ġ(���⼭�� �׷���ī��)�� ������ �������� �İ�ü. 
	LPDIRECT3DDEVICE9	m_pDevice; // �ְ� ���������� ��ġ�� ������ �İ�ü��. 
	LPD3DXSPRITE		m_pSprite; 
	LPD3DXFONT			m_pFont; 
	LPD3DXLINE			m_pLine; 
};

