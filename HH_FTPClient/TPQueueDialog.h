#pragma once
#include "afxcmn.h"


// CTPQueueDialog �Ի���

class CTPQueueDialog : public CDialog
{
	DECLARE_DYNAMIC(CTPQueueDialog)

public:
	CTPQueueDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTPQueueDialog();

// �Ի�������
	enum { IDD = IDD_TAB_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctList;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
