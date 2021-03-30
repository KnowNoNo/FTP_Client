#pragma once


// CDelDirDialog �Ի���

class CDelDirDialog : public CDialog
{
	DECLARE_DYNAMIC(CDelDirDialog)

public:
	CDelDirDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDelDirDialog();

// �Ի�������
	enum { IDD = IDD_DELDIR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csName;

	BOOL m_bKeep;
	BOOL m_bDel;

	void SetFileText(CString str);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck2();

};
