#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"

CStage::CStage()
{
}


CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	CGameObject* pObject = CTerrain::Create(); 
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(CGameObject_Manager::TERRAIN, pObject); 

	return S_OK;
}

void CStage::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager(); 
}

void CStage::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStage::Release_Scene()
{
}

CScene * CStage::Create()
{
	CScene* pInstance = new CStage; 
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}
