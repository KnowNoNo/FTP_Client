// NewDirDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "NewDirDialog.h"


// CNewDirDialog �Ի���

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


// CNewDirDialog ��Ϣ�������

BOOL CNewDirDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	this->SetWindowText(L"����Ŀ¼");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
