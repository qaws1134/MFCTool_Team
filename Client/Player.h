#pragma once
#include "GameObject.h"
class CPlayer final:
	public CGameObject
{
private:
	explicit CPlayer();
public:
	virtual ~CPlayer();
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public:
	static CGameObject* Create();

private:
	
	float m_fAngle = 0.f; 
	float m_fBulletSpeed = 5.f; 
};
