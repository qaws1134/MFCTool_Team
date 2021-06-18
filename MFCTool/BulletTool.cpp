// BulletTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "BulletTool.h"
#include "afxdialogex.h"


// CBulletTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBulletTool, CDialog)

CBulletTool::CBulletTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BULLETTOOL, pParent)
{

}

CBulletTool::~CBulletTool()
{
}

void CBulletTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBulletTool, CDialog)
END_MESSAGE_MAP()


// CBulletTool 메시지 처리기입니다.
