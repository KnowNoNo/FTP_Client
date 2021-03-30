#pragma once
#include "afxcmn.h"
#include "TPQueueDialog.h"
#include "FileListClass.h"

// CTPInfoView 窗体视图

class CTPInfoView : public CFormView,public CFileListClass
{
	DECLARE_DYNCREATE(CTPInfoView)

protected:
	CTPInfoView();           // 动态创建所使用的受保护的构造函数
	virtual ~CTPInfoView();

public:
	enum { IDD = IDD_TPInfoView };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	virtual void OnInitialUpdate();
private:
	DWORD startTime;

public:
	CTabCtrl m_ctTPInfo;

	FILE_UPLOAD_COUNT_INFO*	m_pUploadCount;
	FILE_UPLOAD_COUNT_INFO  m_tFailQueueCount;

	CTPQueueDialog	m_wndQueueDlg;
	CTPQueueDialog  m_wndFailureDlg;
	CTPQueueDialog  m_wndSuccessDlg;

	//CCriticalSection* m_CSFor_UploadCount;
private:
	void InitTabCtrl();
	void UpdateQueueInfo();
	BOOL AddUploadFailFileInfo(FILE_UPLOAD_INFO* pFileInfo);
	void DeleteTPList(CListCtrl* pListCtrl,FILE_UPLOAD_INFO* pInfo,UINT uMark);
	void InsertTPList(CListCtrl* pListCtrl,FILE_UPLOAD_INFO* pInfo,UINT uMark);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	//自定义消息
	
	afx_msg LRESULT OnSHOW_TPFILE_STATUS(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFailqueueReupload();
};


