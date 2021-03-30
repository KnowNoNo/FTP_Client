#pragma once

#include "FileListClass.h"
#include "MyComboEx.h"
#include "afxwin.h"


// CLocView ������ͼ
class CLocView : public CFormView,public CFileListClass
{
	DECLARE_DYNCREATE(CLocView)

public:
	CLocView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLocView();

public:
	enum { IDD = IDD_LOC_FORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:
	virtual void OnInitialUpdate();

public:
	CListCtrl	m_ctLocFile;
	CMyComboEx	m_ctLocDir;
	CStatic     m_ctLocStatus;
	CString     m_csPreFindDir;
	int m_iDirCount;
protected:
	//CMainFrame* m_pMainFrame;
	BOOL m_bHaveDotFlag;
	CWinThread* m_pSendFileThread;
	CWinThread* m_pFindLocFileThread;
	//CCriticalSection	m_CSFor_ctLocFile;
	CCriticalSection	m_CSFor_FindLocFile;
public:
	void GetLocCurrentDirectory();
	void SetFindLocFileDir(CString csStr=CPublicData::szLocDirectory);
	void ThreadProc_FindLocFile();
protected:
	void InitDirCur();
	void GetLocDir();
	int  InsertLocDir(CString& str,int iIcon,int iIconSel);

	//����CPubLicData::szLocDirectory·������m_ctLocFile�ؼ�����
	BOOL BeginFindLocFile();
	void SetLocCtlDirectoryDisplay();
	void InsertLocFile(FILE_FTP_INFO* pInfo,int lParam,UINT &nDirNum);
	void SetLocRedrawFlag();
	//m_ctLocFile��ѡ���ݴ�
	void LocFileOpen();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkLocList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeLocDir();
	afx_msg void OnNMRClickLocList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLocOpen();
	afx_msg void OnLocDelete();
	afx_msg void OnLocSend();
	afx_msg void OnLocRefresh();
	DECLARE_MESSAGE_MAP()

};


