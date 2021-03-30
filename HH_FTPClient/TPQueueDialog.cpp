// TPQueueDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "TPQueueDialog.h"


// CTPQueueDialog 对话框

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


// CTPQueueDialog 消息处理程序


BOOL CTPQueueDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//
	m_ctList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	static struct
	{
		LPCTSTR pszText;
		int uiFormat;
	}columns[]={
		_T("文件"),LVCFMT_LEFT,
		_T("本地"),LVCFMT_LEFT,
		_T("上传/下载"),LVCFMT_CENTER,
		_T("服务器"),LVCFMT_LEFT,
		_T("进度   "),LVCFMT_RIGHT,
		_T("状态"),LVCFMT_RIGHT,
		_T("时间"),LVCFMT_RIGHT
	};
	for(int i=0,j=120;i<sizeof(columns)/sizeof(columns[0]);i++,j-=10)
		m_ctList.InsertColumn(i,columns[i].pszText,columns[i].uiFormat,j);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTPQueueDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_ctList.m_hWnd)
	{
		m_ctList.SetWindowPos(this,0,0,cx,cy,SWP_NOZORDER|SWP_NOMOVE);
	}
}


void CTPQueueDialog::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	GetParent()->SendMessage(WM_NOTIFY,IDC_LIST1,(WPARAM)pNMHDR);
	*pResult = 0;
}
