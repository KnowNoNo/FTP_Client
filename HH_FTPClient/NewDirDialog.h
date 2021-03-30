#pragma once
#include "afxwin.h"


// CNewDirDialog 对话框

class CNewDirDialog : public CDialog
{
	DECLARE_DYNAMIC(CNewDirDialog)

public:
	CNewDirDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewDirDialog();

// 对话框数据
	enum { IDD = IDD_NEWDIR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	virtual BOOL OnInitDialog();
	CString m_csEdit;
};
