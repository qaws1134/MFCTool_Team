#pragma once


// CUiTool ��ȭ �����Դϴ�.

class CUiTool : public CDialog
{
	DECLARE_DYNAMIC(CUiTool)

public:
	CUiTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUiTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UITOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
