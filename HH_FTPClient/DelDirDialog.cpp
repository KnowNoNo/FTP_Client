// DelDirDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "DelDirDialog.h"


// CDelDirDialog 对话框

IMPLEMENT_DYNAMIC(CDelDirDialog, CDialog)

CDelDirDialog::CDelDirDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDelDirDialog::IDD, pParent)
	, m_csName(_T(""))
	, m_bKeep(FALSE)
{
	m_bDel  = FALSE;
}

CDelDirDialog::~CDelDirDialog()
{
}

void CDelDirDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_NAME, m_csName);
	DDX_Check(pDX, IDC_CHECK2, m_bKeep);
}


BEGIN_MESSAGE_MAP(CDelDirDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDelDirDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDelDirDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK2, &CDelDirDialog::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CDelDirDialog 消息处理程序

void CDelDirDialog::SetFileText(CString str)
{
	m_csName = str;
}

void CDelDirDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bDel = TRUE;
	OnOK();
}

void CDelDirDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bDel = FALSE;
	OnCancel();
}

BOOL CDelDirDialog::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialog::PreCreateWindow(cs);
}

BOOL CDelDirDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	this->SetWindowText(L"Check");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDelDirDialog::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bKeep ^= 1;
}
