#pragma once
class CScroll_Manager
{
public:
	enum ID {SCROLL_X, SCROLL_Y};
public:
	CScroll_Manager();
	~CScroll_Manager();
public:
	const D3DXVECTOR3& Get_Scroll() const { return m_vScroll; }
	void Set_Scroll(const D3DXVECTOR3& vScroll ); 
	void Set_Scroll(ID eScrollID, float fScroll);
private:
	static D3DXVECTOR3 m_vScroll; 
};

