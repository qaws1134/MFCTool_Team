#pragma once
class CGameObject; 
class CMainApp final 
{
	// abstract - �߻� - Ŭ������ �߻�Ŭ������ ������ִ� Ű����. 
	// ���� �߻� Ŭ������ ���� �����Լ��� �� �ϳ��� ���� ��� �����������
	// abstract��� Ű���尡 ����� ������ ���� �����Լ��� ��� �߻� Ŭ����ȭ ��ų�� �ִ�. 
	// ���� . �����ϱ� ���� Ű�����ϼ� ? �ȱ׷��� ? 
	// final - ��..�̺��� �ǻ���... �װ� �������̿�... - ���νô� - feat. �ɿ�. 
private:
	CMainApp();
public:
	~CMainApp();
public:
	HRESULT Ready_MainApp(); 
	void	Update_MainApp(); 
	void	Render_MainApp(); 
	void	Release_MainApp(); 
public:
	static CMainApp* Create();
private:

};

