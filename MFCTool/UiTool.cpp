// UiTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "UiTool.h"
#include "afxdialogex.h"


// CUiTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUiTool, CDialog)

CUiTool::CUiTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UITOOL, pParent)
{

}

CUiTool::~CUiTool()
{
}

void CUiTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUiTool, CDialog)
END_MESSAGE_MAP()


// CUiTool �޽��� ó�����Դϴ�.
