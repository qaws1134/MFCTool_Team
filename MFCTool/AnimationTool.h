#pragma once


// CAnimationTool ��ȭ �����Դϴ�.

class CAnimationTool : public CDialog
{
	DECLARE_DYNAMIC(CAnimationTool)

public:
	CAnimationTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimationTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
