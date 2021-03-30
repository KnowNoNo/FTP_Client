// TPQueueDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "TPQueueDialog.h"


// CTPQueueDialog �Ի���

IMPLEMENT_DYNAMIC(CTPQueueDialog, CDialog)

CTPQueueDialog::CTPQueueDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTPQueueDialog::IDD, pParent)
{

}

CTPQueueDialog::~CTPQueueDialog()
{
}

void CTPQueueDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctList);
}


BEGIN_MESSAGE_MAP(CTPQueueDialog, CDialog)
	ON_WM_SIZE()
	//ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CTPQueueDialog::OnNMRClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CTPQueueDialog::OnNMRClickList1)
END_MESSAGE_MAP()


// CTPQueueDialog ��Ϣ�������


BOOL CTPQueueDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//
	m_ctList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	static struct
	{
		LPCTSTR pszText;
		int uiFormat;
	}columns[]={
		_T("�ļ�"),LVCFMT_LEFT,
		_T("����"),LVCFMT_LEFT,
		_T("�ϴ�/����"),LVCFMT_CENTER,
		_T("������"),LVCFMT_LEFT,
		_T("����   "),LVCFMT_RIGHT,
		_T("״̬"),LVCFMT_RIGHT,
		_T("ʱ��"),LVCFMT_RIGHT
	};
	for(int i=0,j=120;i<sizeof(columns)/sizeof(columns[0]);i++,j-=10)
		m_ctList.InsertColumn(i,columns[i].pszText,columns[i].uiFormat,j);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CTPQueueDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_ctList.m_hWnd)
	{
		m_ctList.SetWindowPos(this,0,0,cx,cy,SWP_NOZORDER|SWP_NOMOVE);
	}
}


void CTPQueueDialog::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetParent()->SendMessage(WM_NOTIFY,IDC_LIST1,(WPARAM)pNMHDR);
	*pResult = 0;
}
