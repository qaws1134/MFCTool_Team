#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	m_tInfo.vPos.x = 400.f; 
	m_tInfo.vPos.y = 300.f; 
	m_tInfo.vDir = { 1.f, -1.f, 0.f }; 
	m_tInfo.vSize = D3DXVECTOR3(100.f, 100.f, 0.f); 


	return S_OK;
}

int CPlayer::Update_GameObject()
{

	return 0;
}

void CPlayer::Late_Update_GameObject()
{
}

void CPlayer::Render_GameObject()
{

}

void CPlayer::Release_GameObject()
{
}

CGameObject * CPlayer::Create()
{
	CGameObject* pInstance = new CPlayer;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}
