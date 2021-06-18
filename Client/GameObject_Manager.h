#pragma once
class CGameObject; 
class CGameObject_Manager
{
	DECLARE_SINGLETON(CGameObject_Manager)
public:
	enum ID {TERRAIN, PLAYER, MONSTER, END};
private:
	CGameObject_Manager();
	~CGameObject_Manager();
public:
	void Add_GameObject_Manager(ID eID, CGameObject* pObject );  
	void Update_GameObject_Manager(); 
	void Render_GameObject_Manager(); 
	void Release_GameObject_Manager(); 
private:
	list<CGameObject*> m_listGameObject[END]; 
};

