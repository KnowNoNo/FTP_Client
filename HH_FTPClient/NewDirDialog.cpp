// NewDirDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "NewDirDialog.h"


// CNewDirDialog 对话框

IMPLEMENT_DYNAMIC(CNewDirDialog, CDialog)

CNewDirDialog::CNewDirDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDirDialog::IDD, pParent)
	, m_csEdit(_T(""))
{

}

CNewDirDialog::~CNewDirDialog()
{
}

void CNewDirDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT1, m_csEdit);
}


BEGIN_MESSAGE_MAP(CNewDirDialog, CDialog)
END_MESSAGE_MAP()


// CNewDirDialog 消息处理程序

BOOL CNewDirDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	this->SetWindowText(L"创建目录");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
