#pragma once


// CBulletTool ��ȭ �����Դϴ�.

class CBulletTool : public CDialog
{
	DECLARE_DYNAMIC(CBulletTool)

public:
	CBulletTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBulletTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BULLETTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
