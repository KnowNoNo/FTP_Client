#pragma once
#include "afxwin.h"


// CNewDirDialog �Ի���

class CNewDirDialog : public CDialog
{
	DECLARE_DYNAMIC(CNewDirDialog)

public:
	CNewDirDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewDirDialog();

// �Ի�������
	enum { IDD = IDD_NEWDIR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	virtual BOOL OnInitDialog();
	CString m_csEdit;
};
