#pragma once
#include "afxcmn.h"


// CTPQueueDialog 对话框

class CTPQueueDialog : public CDialog
{
	DECLARE_DYNAMIC(CTPQueueDialog)

public:
	CTPQueueDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTPQueueDialog();

// 对话框数据
	enum { IDD = IDD_TAB_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctList;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
