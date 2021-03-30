// DelDirDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "DelDirDialog.h"


// CDelDirDialog �Ի���

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


// CDelDirDialog ��Ϣ�������

void CDelDirDialog::SetFileText(CString str)
{
	m_csName = str;
}

void CDelDirDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bDel = TRUE;
	OnOK();
}

void CDelDirDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bDel = FALSE;
	OnCancel();
}

BOOL CDelDirDialog::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CDialog::PreCreateWindow(cs);
}

BOOL CDelDirDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	this->SetWindowText(L"Check");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDelDirDialog::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bKeep ^= 1;
}
