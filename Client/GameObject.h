#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();
public:
	void Set_Pos(const D3DXVECTOR3& vPos);
public:
	virtual HRESULT Ready_GameObject()PURE; 
	virtual int		Update_GameObject()PURE;
	virtual void	Late_Update_GameObject()PURE; 
	virtual void	Render_GameObject( )PURE; 
	virtual void	Release_GameObject()PURE; 
protected:
	INFO m_tInfo; 
	//INFO* m_pInfo; 
};

