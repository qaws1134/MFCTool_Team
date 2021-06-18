#pragma once
#include "Scene.h"
class CStage final :
	public CScene
{
private:
	explicit CStage();
public:
	virtual ~CStage();
public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
public:
	static CScene* Create(); 

};

