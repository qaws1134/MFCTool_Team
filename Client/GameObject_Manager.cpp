#include "stdafx.h"
#include "GameObject_Manager.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)
CGameObject_Manager::CGameObject_Manager()
{
}


CGameObject_Manager::~CGameObject_Manager()
{
	Release_GameObject_Manager(); 
}

void CGameObject_Manager::Add_GameObject_Manager(ID eID, CGameObject * pObject)
{
	if (END <= eID || nullptr == pObject || 0 > eID)
		return; 

	m_listGameObject[eID].emplace_back(pObject); 
}

void CGameObject_Manager::Update_GameObject_Manager()
{
	for (int i = 0 ; i < END ; ++i)
	{
		for (auto& iter = m_listGameObject[i].begin() ; iter != m_listGameObject[i].end() ; )
		{
			int iEvent = (*iter)->Update_GameObject(); 
			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete(*iter); 
				iter = m_listGameObject[i].erase(iter); 
			}
			else ++iter; 
		}
	}

	for (int i = 0; i < END; ++i)
	{
		for (auto& pGameObject : m_listGameObject[i])
			pGameObject->Late_Update_GameObject();
	}
}

void CGameObject_Manager::Render_GameObject_Manager()
{
	for (int i = 0; i < END; ++i)
	{
		for (auto& pGameObject : m_listGameObject[i])
			pGameObject->Render_GameObject();
	}
}

void CGameObject_Manager::Release_GameObject_Manager()
{
	for (int i = 0; i < END; ++i)
	{
		for (auto& pObject : m_listGameObject[i])
		{
			Safe_Delete(pObject);
		}
		m_listGameObject[i].clear();
	}
}
