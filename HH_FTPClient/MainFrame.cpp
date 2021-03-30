
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND_EX(ID_VIEW_DIALOGBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DIALOGBAR, OnUpdateControlBarMenu)
	ON_BN_CLICKED(IDC_QUICKCONNECT, OnQuickconnect)
	ON_BN_CLICKED(IDC_DISCONNECT, &CMainFrame::OnBnClickedDisconnect)

	ON_WM_CREATE()
	ON_WM_SIZE()

	//
	ON_UPDATE_COMMAND_UI(IDC_QUICKCONNECT,OnUpdateButtonQuickConnect)
	ON_UPDATE_COMMAND_UI(IDC_DISCONNECT,OnUpdateButtonDisconnect)
	ON_UPDATE_COMMAND_UI(IDC_FTPNAME,OnUpdateButtonFtpName)
	ON_UPDATE_COMMAND_UI(IDC_FTPPORT,OnUpdateButtonFtpPort)
	ON_UPDATE_COMMAND_UI(IDC_FTPPASSWORD,OnUpdateButtonFtpPassword)
	ON_UPDATE_COMMAND_UI(IDC_FTPUSER,OnUpdateButtonFtpUser)
	ON_UPDATE_COMMAND_UI(IDC_ISPORT,OnUpdateButtonFtpIsPort)
	ON_UPDATE_COMMAND_UI(IDC_ISFORCE,OnUpdateButtonFtpIsForce)

	ON_COMMAND(ID_CONNECT, &CMainFrame::OnConnect)
	ON_COMMAND(ID_DISCONNECT, &CMainFrame::OnDisconnect)
	ON_COMMAND(ID_REFRESH, &CMainFrame::OnRefresh)
	ON_COMMAND(ID_INFODESK_SHOW, &CMainFrame::OnInfodeskShow)
	ON_COMMAND(ID_TPQUEUE_SHOW, &CMainFrame::OnTpqueueShow)
	ON_COMMAND(ID_UPLOAD, &CMainFrame::OnUpload)
	ON_COMMAND(ID_CREATE_SERVER_DIR, &CMainFrame::OnCreateServerDir)
	ON_COMMAND(ID_DELETE_SERVER_DIR, &CMainFrame::OnDeleteServerDir)
	ON_COMMAND(ID_SERVER_SHOW, &CMainFrame::OnServerShow)
	ON_COMMAND(ID_DOWNLOAD, &CMainFrame::OnDownload)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	IDS_STRING_QUEUEFILE,
	IDS_STRING_RATE
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	CPublicData::bButtonEnable = TRUE;
	CPublicData::nPort         = DEFAULT_FTP_PORT;
	
	m_pInfoDeskView			   = NULL;
	m_pLocView				   = NULL;
	m_pServeView			   = NULL;
	m_pTPInfoView			   = NULL;

	m_bAutoMenuEnable		   = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("未能创建对话栏\n");
		return -1;		// 未能创建
	}

	if (!m_wndReBar.Create(this) || !m_wndReBar.AddBar(&m_wndToolBar) || !m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("未能创建 Rebar\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要工具提示，则将此移除
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	
	m_wndStatusBar.SetPaneInfo(0,IDS_STRING_QUEUEFILE,SBPS_STRETCH,0);
	m_wndStatusBar.SetPaneInfo(1,IDS_STRING_RATE,SBPS_POPOUT,200);

	if (!m_wndMyDialogBar.Create(this, IDD_DIALOGBAR,
		CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		ID_VIEW_DIALOGBAR))
	{
		TRACE0("Failed to create dialog bar m_wndMyDialogBar\n");
		return -1;		// fail to create
	}
	
	m_wndMyDialogBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMyDialogBar);
	//m_wndMyDialogBar.SetDlgItemText(IDC_FTPPORT,L"21");
	//m_wndMyDialogBar.SetDlgItemText(IDC_FTPNAME,L"172.16.18.182");
	//m_wndMyDialogBar.SetDlgItemText(IDC_FTPUSER,L"Anonymous");
	//m_wndMyDialogBar.SetDlgItemText(IDC_FTPPASSWORD,L"Anonymous");

	//初始化控件信息
	InitMyDialogBarDate();

	HandleCommandLine();
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~FWS_ADDTOTITLE ;
	m_strTitle = L"HH-FTP文件传输客户端";
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


CString CMainFrame::GetCtrlValueAndSaveSettingInfo(UINT nCtrlID)
{
	CString csEntry, csValue;
	csEntry.Format ( L"%d", nCtrlID );
	m_wndMyDialogBar.GetDlgItemText ( nCtrlID, csValue );
	csValue.TrimLeft(); csValue.TrimRight ();
	AfxGetApp()->WriteProfileString ( L"Setting", csEntry, csValue );
	return csValue;
}

void CMainFrame::ReadSettingInfoAndSetCtrlValue(UINT nCtrlID, LPCTSTR lpszDefaultValue)
{
	CString csEntry;
	csEntry.Format ( L"%d", nCtrlID );
	m_wndMyDialogBar.SetDlgItemText ( nCtrlID, AfxGetApp()->GetProfileString ( L"Setting", csEntry, lpszDefaultValue ) );
}

void CMainFrame::HandleCommandLine()
{
	CString strCmd =  AfxGetApp()->m_lpCmdLine;

	if(strCmd == L"/CMDFormMediaPlay")
	{
		OnInfodeskShow();
		OnServerShow();
		OnTpqueueShow();
	}
}

void CMainFrame::InitMyDialogBarDate()
{
	ReadSettingInfoAndSetCtrlValue(IDC_FTPNAME,L"");
	ReadSettingInfoAndSetCtrlValue(IDC_FTPUSER,L"");
	ReadSettingInfoAndSetCtrlValue(IDC_FTPPASSWORD,L"");
	ReadSettingInfoAndSetCtrlValue(IDC_FTPPORT,L"");
	((CButton*)(m_wndMyDialogBar.GetDlgItem(IDC_ISPORT)) )->SetCheck ( AfxGetApp()->GetProfileInt ( L"Setting", L"IDC_ISPORT", 0 ) );
	((CButton*)(m_wndMyDialogBar.GetDlgItem(IDC_ISFORCE)) )->SetCheck ( AfxGetApp()->GetProfileInt ( L"Setting", L"IDC_ISFORCE", 0 ) );
}

void CMainFrame::GetCtrlValue()
{
	CPublicData::szFtpName = GetCtrlValueAndSaveSettingInfo ( IDC_FTPNAME );
	CPublicData::szFtpUser = GetCtrlValueAndSaveSettingInfo ( IDC_FTPUSER );
	CPublicData::szFtpPassword = GetCtrlValueAndSaveSettingInfo ( IDC_FTPPASSWORD );
	CPublicData::nPort = CStringToInt(GetCtrlValueAndSaveSettingInfo ( IDC_FTPPORT ));
	CPublicData::bIsPort = ((CButton*)(m_wndMyDialogBar.GetDlgItem(IDC_ISPORT)) )->GetCheck()==1?true:false;
	AfxGetApp()->WriteProfileInt ( L"Setting", L"IDC_ISPORT", CPublicData::bIsPort );
	CPublicData::bIsForce  = ((CButton*)(m_wndMyDialogBar.GetDlgItem(IDC_ISFORCE)) )->GetCheck()==1?true:false;
	AfxGetApp()->WriteProfileInt ( L"Setting", L"IDC_ISFORCE", CPublicData::bIsForce);
}

// CMainFrame 消息处理程序

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	CRect rect;
	GetClientRect(&rect);

	if(!m_wndSplitter1.CreateStatic(this,3,1))
		return FALSE;
	if(!m_wndSplitter1.CreateView(0,0,RUNTIME_CLASS(CInfoDeskView),CSize(rect.Width(),rect.Height()/6),pContext))
		return FALSE;
	if(m_wndSplitter2.CreateStatic(&m_wndSplitter1,1,2,WS_CHILD|WS_VISIBLE,m_wndSplitter1.IdFromRowCol(1,0))==NULL)
		return FALSE;
	if(!m_wndSplitter2.CreateView(0,0,RUNTIME_CLASS(CLocView),CSize(rect.Width()/2,rect.Height()/6*3),pContext)||
		!m_wndSplitter2.CreateView(0,1,RUNTIME_CLASS(CServeView),CSize(rect.Width()/2,rect.Height()/6*3),pContext))
		return FALSE;
	if(!m_wndSplitter1.CreateView(2,0,RUNTIME_CLASS(CTPInfoView),CSize(rect.Width(),rect.Height()/6*2),pContext))
		return FALSE;
	
	m_wndSplitter1.SetRowInfo(0,rect.Height()/6*1,20);
	m_wndSplitter1.SetRowInfo(1,rect.Height()/6*4,20);
	m_wndSplitter1.SetRowInfo(2,rect.Height()/6*1,20);

	m_pInfoDeskView = (CInfoDeskView*)m_wndSplitter1.GetPane(0,0);
	m_pLocView      = (CLocView*)m_wndSplitter2.GetPane(0,0);
	m_pServeView    = (CServeView*)m_wndSplitter2.GetPane(0,1);
	m_pTPInfoView   = (CTPInfoView*)m_wndSplitter1.GetPane(2,0);
	
	// 根据分辨率自动调节窗口大小
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
	AdaptWindowSize(oMonitor.rcMonitor.right - oMonitor.rcMonitor.left);

	return TRUE;
	
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

void CMainFrame::OnUpdateButtonQuickConnect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CPublicData::bButtonEnable);
}

void CMainFrame::OnUpdateButtonDisconnect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!CPublicData::bButtonEnable);
}

void CMainFrame::OnUpdateButtonFtpName(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CPublicData::bButtonEnable);
}
void CMainFrame::OnUpdateButtonFtpUser(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CPublicData::bButtonEnable);
}
void CMainFrame::OnUpdateButtonFtpPassword(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CPublicData::bButtonEnable);
}
void CMainFrame::OnUpdateButtonFtpPort(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CPublicData::bButtonEnable);
}

void CMainFrame::OnUpdateButtonFtpIsPort(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CPublicData::bButtonEnable);
}

void CMainFrame::OnUpdateButtonFtpIsForce(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CPublicData::bButtonEnable);
}

void CMainFrame::OnQuickconnect() 
{
	//保存信息
	GetCtrlValue();
	m_pServeView->SendMessage(WM_CONNECTSERVE);
}

void CMainFrame::OnBnClickedDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pServeView->OnDisconnectServe(NULL,NULL);
}

void CMainFrame::OnConnect()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(m_hWnd,WM_COMMAND,IDC_QUICKCONNECT,NULL);
}

void CMainFrame::OnDisconnect()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(m_hWnd,WM_COMMAND,IDC_DISCONNECT,NULL);
}

void CMainFrame::OnRefresh()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(m_pLocView->m_hWnd,WM_COMMAND,ID_LOC_REFRESH,NULL);
	if(CPublicData::szFtpDirectory != L"")
		::PostMessage(m_pServeView->m_hWnd,WM_COMMAND,ID_SER_REFRESH,NULL);
	Invalidate();
}

void CMainFrame::OnInfodeskShow()
{
	// TODO: 在此添加命令处理程序代码
	CRect rectA,rectB;
	m_pInfoDeskView->GetClientRect(&rectA);
	m_pLocView->GetClientRect(&rectB);

	//调整INFODESK,LOC,SERV窗口大小。（窗口高度小于20，默认为0）
	if(rectA.Height() <= 20)//展开
	{
		m_wndSplitter1.SetRowInfo(0,rectB.Height()/4,20);
		m_wndSplitter1.SetRowInfo(1,rectB.Height()/4*3,20);
	}
	else//隐藏
	{
		m_wndSplitter1.SetRowInfo(0,0,20);
		m_wndSplitter1.SetRowInfo(1,rectA.Height()+rectB.Height(),20);
	}
	
	m_wndSplitter1.RecalcLayout();
}

void CMainFrame::OnServerShow()
{
	// TODO: 在此添加命令处理程序代码
	CRect rectA,rectB;
	m_pServeView->GetClientRect(&rectA);
	m_pLocView->GetClientRect(&rectB);

	//调整LOC,SERV窗口大小。（窗口宽度小于20，默认为0）
	if(rectA.Width() <= 20)//展开
	{
		m_wndSplitter2.SetColumnInfo(0,rectB.Width()/2,20);
		m_wndSplitter2.SetColumnInfo(1,rectB.Width()/2,20);
	}
	else//隐藏
	{
		m_wndSplitter2.SetColumnInfo(0,rectA.Width()+rectB.Width(),20);
		m_wndSplitter2.SetColumnInfo(1,0,20);
	}

	m_wndSplitter2.RecalcLayout();

}

void CMainFrame::OnTpqueueShow()
{
	// TODO: 在此添加命令处理程序代码
	//m_pTPInfoView->ShowWindow(SW_HIDE);
	CRect rectA,rectB;
	m_pLocView->GetClientRect(&rectA);
	m_pTPInfoView->GetClientRect(&rectB);
	
	//调整LOC,SERV,TPINFO窗口大小
	if(rectB.Height() <= 20)//展开
	{
		m_wndSplitter1.SetRowInfo(1,rectA.Height()/3*2,20);
		m_wndSplitter1.SetRowInfo(2,rectA.Height()/3,20);
	}
	else//隐藏
	{
		m_wndSplitter1.SetRowInfo(1,rectA.Height()+rectB.Height(),20);
		m_wndSplitter1.SetRowInfo(2,0,20);
	}
	
	m_wndSplitter1.RecalcLayout();
}

void CMainFrame::OnUpload()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(m_pLocView->m_hWnd,WM_COMMAND,ID_LOC_SEND,NULL);
}

void CMainFrame::OnCreateServerDir()
{
	// TODO: 在此添加命令处理程序代码
	if(CPublicData::bButtonEnable == TRUE)
	{
		MessageBox(L"服务器未连接");
		return;
	}
	m_pServeView->SendMessage(WM_COMMAND,ID_SER_CREAT_DIR,NULL);
}

void CMainFrame::OnDeleteServerDir()
{
	// TODO: 在此添加命令处理程序代码
	if(CPublicData::bButtonEnable == TRUE)
	{
		MessageBox(L"服务器未连接");
		return;
	}
	m_pServeView->SendMessage(WM_COMMAND,ID_SER_DELETE_DIR,NULL);
}



void CMainFrame::OnDownload()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(m_pServeView->m_hWnd,WM_COMMAND,ID_SER_DOWNLOAD,NULL);
}

void CMainFrame::AdaptWindowSize(UINT cxScreen)
{
	int iX = 968, iY = 600;
	int iWidthList = 225, iWidthSearchEdit = 193;
	SIZE szFixSearchBtn = {201, 0};

	if(cxScreen <= 1024)      // 800*600  1024*768  
	{
		iX = 775;
		iY = 470;
	} 
	else if(cxScreen <= 1280) // 1152*864  1280*800  1280*960  1280*1024
	{
		iX = 968;
		iY = 600;
	}
	else if(cxScreen <= 1366) // 1360*768 1366*768
	{
		iX = 1058;
		iY = 656;
		iWidthList        += 21;
		iWidthSearchEdit  += 21;
		szFixSearchBtn.cx += 21;
	}
	else                      // 1440*900
	{
		iX = 1224;
		iY = 760;
		iWidthList        += 66;
		iWidthSearchEdit  += 66;
		szFixSearchBtn.cx += 66;
	}

	::SetWindowPos(this->GetSafeHwnd(), NULL, 0, 0, iX, iY, SWP_FRAMECHANGED|SWP_NOZORDER|SWP_NOACTIVATE);
	CenterWindow();
}
