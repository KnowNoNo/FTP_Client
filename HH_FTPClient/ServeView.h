#pragma once

#include "FileListClass.h"
#include "FTPCmdClass.h"
#include "resource.h"

// CServeView 窗体视图
class CServeView : public CFormView,CFileListClass
{
	DECLARE_DYNCREATE(CServeView)

protected:
	CServeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CServeView();

public:
	enum { IDD = IDD_SERV_FORMVIEW };
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
	CButton	    m_ctBack;
	CListCtrl	m_ctServFile;
	CComboBoxEx	m_ctServDir;
	CStatic     m_ctServStatus;

	HBITMAP hBitmap;
	BOOL    m_bHaveDotFlag;
	CString m_csPreFindDir;
	CString m_csPreCreatDir;
	CImageList m_ctServImageList;
	FILE_UPLOAD_COUNT_INFO m_tUploadCount;
	//CCriticalSection	m_CSFor_UploadCount;
	CCriticalSection	m_CSFor_FindServFile;
	//HANDLE m_hEvtEndModule;	// 模块结束事件

	CWinThread* m_pFindServFileThread;
	CWinThread* m_pSendFileThread;
	CFTPCmdClass* m_pFtpCmd;
	//UINT_PTR    m_nTPStatusTimer;
	
private://私有
	BOOL SetServImageList(UINT nBitmapID);
	void SetServerCtlDirectoryDisplay();
	void GetServerCurrentDirectory();
	void SetServerRedrawFlag();
	void InsertServeFile(FILE_FTP_INFO* pInfo,int lParam,UINT& nDirNum);
	//
	BOOL AddUploadFileInfo(FILE_UPLOAD_INFO* pFileInfo);
	BOOL DelUploadFileInfo(FILE_UPLOAD_INFO* pFileInfo);
	BOOL UploadOrDownloadFileOnce(CFTPCmdClass* pCmd,FILE_UPLOAD_INFO* pUploadInfo);

public://公共接口
	void SetFindServFileDir(CString csStr=CPublicData::szFtpDirectory);
	BOOL BeginFindServFile(CString csPreServFileDir);
	BOOL BeginUploadOrDownloadFile(CString csFileName);
	BOOL BeginCreateDir();
	BOOL BeginDeleteDir();
	void ThreadProc_FindServFile();
	void ThreadProc_UploadOrDownloadFile(FILE_UPLOAD_INFO* pUploadInfo);
	void ThreadProc_CreateServDir();
	void ThreadProc_DeleteServDir();
	void DisConnect();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnBnClickedBack();
	afx_msg void OnNMDblclkServList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickServList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSerRefresh();
	afx_msg void OnSerCreatDir();
	afx_msg void OnSerDeleteDir();
	afx_msg void OnSerDownload();

	//自定义
	afx_msg LRESULT OnConnectServe(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDisconnectServe(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUploadFile(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


