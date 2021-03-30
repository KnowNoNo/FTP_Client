
// MainFrame.h : CMainFrame 类的接口
//

#pragma once
#include "InfoDeskView.h"
#include "LocView.h"
#include "ServeView.h"
#include "TPInfoView.h"
#include "resource.h"
//class CLocView;	

class CMainFrame : public CFrameWnd
{
	
public: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	// 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CReBar            m_wndReBar;
	CDialogBar        m_wndDlgBar;
public:
	CStatusBar        m_wndStatusBar;
protected:
	// 窗口
	CSplitterWnd	  m_wndSplitter1;
	CSplitterWnd      m_wndSplitter2;
	CDialogBar		  m_wndMyDialogBar;
	//
public: //窗口类
	CLocView*         m_pLocView;
	CServeView*		  m_pServeView;
	CInfoDeskView*    m_pInfoDeskView;
	CTPInfoView*      m_pTPInfoView;
	

protected:	//date

public:	
	CString GetCtrlValueAndSaveSettingInfo(UINT nCtrlID);//保存联通信息
	void ReadSettingInfoAndSetCtrlValue(UINT nCtrlID, LPCTSTR lpszDefaultValue);
private:
	void HandleCommandLine();
	void InitMyDialogBarDate();
	void GetCtrlValue();
	void AdaptWindowSize( UINT cxScreen );

private:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);


// 生成的消息映射函数
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnQuickconnect();

	afx_msg void OnUpdateButtonQuickConnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonFtpName(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonFtpUser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonFtpPassword(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonFtpPort(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonFtpIsPort(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonFtpIsForce(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	afx_msg void OnRefresh();
	afx_msg void OnInfodeskShow();
	afx_msg void OnTpqueueShow();
	afx_msg void OnUpload();
	afx_msg void OnCreateServerDir();
	afx_msg void OnDeleteServerDir();
	afx_msg void OnServerShow();
	afx_msg void OnDownload();
};


