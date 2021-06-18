#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	//:m_tInfo({})
{
	ZeroMemory(&m_tInfo, sizeof(INFO)); 
}


CGameObject::~CGameObject()
{
}

void CGameObject::Set_Pos(const D3DXVECTOR3 & vPos)
{
	m_tInfo.vPos = vPos; 
}
