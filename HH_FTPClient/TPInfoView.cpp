// TPInfoView.cpp : 实现文件
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "MainFrame.h"
#include "TPInfoView.h"

// CTPInfoView

IMPLEMENT_DYNCREATE(CTPInfoView, CFormView)

CTPInfoView::CTPInfoView()
	: CFormView(CTPInfoView::IDD)
{
	m_pUploadCount = NULL;
	m_tFailQueueCount.nThreadCount = 0;
	m_tFailQueueCount.mpFileInfo.clear();
}

CTPInfoView::~CTPInfoView()
{
}

void CTPInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctTPInfo);
}

BEGIN_MESSAGE_MAP(CTPInfoView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTPInfoView::OnTcnSelchangeTab1)
	//
	ON_MESSAGE(WM_SHOW_TPFILE_STATUS, OnSHOW_TPFILE_STATUS)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CTPInfoView::OnNMRClickList1)
	ON_COMMAND(ID_FAILQUEUE_REUPLOAD, &CTPInfoView::OnFailqueueReupload)
END_MESSAGE_MAP()


// CTPInfoView 诊断

#ifdef _DEBUG
void CTPInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTPInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTPInfoView 消息处理程序

void CTPInfoView::OnSize(UINT nType, int cx, int cy)
{
	//CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if(m_ctTPInfo.m_hWnd)
	{
		m_ctTPInfo.SetWindowPos(this,0,0,cx,cy,SWP_NOZORDER|SWP_NOMOVE);
		if(m_wndQueueDlg.m_hWnd && m_wndSuccessDlg.m_hWnd && m_wndFailureDlg.m_hWnd)
		{
			m_wndQueueDlg.SetWindowPos(this,0,0,cx,cy-20,SWP_NOZORDER|SWP_NOMOVE);
			m_wndSuccessDlg.SetWindowPos(this,0,0,cx,cy-20,SWP_NOZORDER|SWP_NOMOVE);
			m_wndFailureDlg.SetWindowPos(this,0,0,cx,cy-20,SWP_NOZORDER|SWP_NOMOVE);
		}
	}
	//
}

void CTPInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	InitTabCtrl();
	
	startTime = GetTickCount();
	SetTimer(TIMER_ID_TO_UPDATETPINFO,2000,NULL);
}


void CTPInfoView::InitTabCtrl()
{
	CRect rect,rcTabItem;
	//m_ctTPInfo.GetClientRect(&rect);
	m_ctTPInfo.InsertItem(0,L"传输队列文件");
	m_ctTPInfo.InsertItem(1,L"传输失败");
	m_ctTPInfo.InsertItem(2,L"传输成功");
	m_ctTPInfo.HighlightItem(0,TRUE);
	m_ctTPInfo.HighlightItem(1,TRUE);
	m_ctTPInfo.HighlightItem(2,TRUE);
	//
	m_wndQueueDlg.Create(IDD_TAB_DIALOG1,&m_ctTPInfo);
	m_wndFailureDlg.Create(IDD_TAB_DIALOG1,&m_ctTPInfo);
	m_wndSuccessDlg.Create(IDD_TAB_DIALOG1,&m_ctTPInfo);

	m_wndQueueDlg.ShowWindow(TRUE);
	m_ctTPInfo.SetCurFocus(0);

}

void CTPInfoView::DeleteTPList(CListCtrl* pListCtrl,FILE_UPLOAD_INFO* pInfo,UINT uMark)
{
	if(pInfo == NULL || m_pUploadCount == NULL) return;

	int nIndex;
	LVFINDINFO info;
	TCITEM  tcItem;
	CString csName,csText;
	TCHAR     pch[MAX_DIR_SIZE];
	
	switch(uMark)
	{
	case FILE_TP_JOIN:
		csName.Format(L"传输队列文件");
		break;
	case FILE_TP_FAIL:
		csName.Format(L"传输失败");
		AddUploadFailFileInfo(pInfo);
		break;
	case FILE_TP_SUCCESS:
		csName.Format(L"传输成功");
		break;
	}

	CStringToTchar(pInfo->csTPFileName,pch);
	info.flags = LVFI_STRING|LVFI_PARTIAL;
	info.psz   = pch;
	if((nIndex = pListCtrl->FindItem(&info)) == -1)
		return;

	//tab控件的标题名
	if(pListCtrl->GetItemCount() > 1)
		csText.Format(L"%s(%d)",csName,pListCtrl->GetItemCount()-1);
	else
		csText = csName;

	tcItem.pszText = csText.LockBuffer();
	tcItem.mask    = TCIF_TEXT;
	m_ctTPInfo.SetItem(uMark , &tcItem);
	csText.UnlockBuffer();
	//
	pListCtrl->DeleteItem(nIndex);
}

void CTPInfoView::InsertTPList(CListCtrl* pListCtrl,FILE_UPLOAD_INFO* pInfo,UINT uMark)
{
	LVITEM	lvIt;
	int iIcon,nItem,iPos;
	BOOL bIsUpload = pInfo->bTPMode;
	CString csSize,csStatus,csTime,csName,csText;
	TCHAR*	pCh = new TCHAR[256]; 
	SYSTEMTIME st;
	TCITEM  tcItem;
	//
	if(pInfo->ullTPFileByte <= 0 || (!pInfo->bIsForce && pInfo->ullTPFileByte <= pInfo->ullTPDestnFileSize))
		csSize = L"100%";
	else
		csSize.Format(L"%d%%",(int)(100.0*(pInfo->ullTpFileSize+(pInfo->bIsForce?0:pInfo->ullTPDestnFileSize))/(pInfo->ullTPFileByte)));
	//
	switch(uMark)
	{
	case FILE_TP_JOIN:
		if(bIsUpload)
			csStatus = L"正在下载";
		else 
			csStatus = L"正在上传";
		csName.Format(L"传输队列文件");
		break;
	case FILE_TP_FAIL:
		if(bIsUpload)
			csStatus = L"下载失败";
		else
			csStatus = L"上传失败";
		csName.Format(L"传输失败");
		AddUploadFailFileInfo(pInfo);
		break;
	case FILE_TP_SUCCESS:
		if(bIsUpload)
			csStatus = L"下载成功";
		else
			csStatus = L"上传成功";
		csName.Format(L"传输成功");
		break;
	}
	//当前时间
	GetLocalTime(&st);
	csTime.Format(L"[%04d-%02d-%02d %02d:%02d:%02d] ",st.wYear,
		st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

	nItem = pListCtrl->GetItemCount();
	CString fileName=pInfo->csTPFileName;
	GetFileIcon(fileName,&iIcon,NULL,LOCFILE);

	//tab控件的标题名
	csText.Format(L"%s(%d)",csName,nItem+1);
	tcItem.pszText = csText.LockBuffer();
	tcItem.mask    = TCIF_TEXT;
	m_ctTPInfo.SetItem(uMark , &tcItem);
	csText.UnlockBuffer();

	//list控件信息
	lvIt.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	lvIt.iImage=iIcon;
	lvIt.lParam=FILEICON;
	CStringToTchar(pInfo->csTPFileName,pCh);
	lvIt.pszText=pCh;
	lvIt.iSubItem=0;
	lvIt.iItem=nItem;
	iPos = pListCtrl->InsertItem(&lvIt);
	ASSERT(iPos >= 0);

	lvIt.mask=LVIF_TEXT;
	lvIt.iItem=iPos;
	//file name
	CStringToTchar(pInfo->csLocFileDir,pCh);
	lvIt.pszText=pCh;
	lvIt.iSubItem=1;
	pListCtrl->SetItem(&lvIt);
	//tp 方式
	lvIt.pszText=bIsUpload?L"下载":L"上传";
	lvIt.iSubItem=2;
	pListCtrl->SetItem(&lvIt);
	//server file dir
	CStringToTchar(pInfo->csServIp+L":"+pInfo->csServFileDir,pCh);
	lvIt.pszText=pCh;
	lvIt.iSubItem=3;
	pListCtrl->SetItem(&lvIt);
	//size
	CStringToTchar(csSize,pCh);
	lvIt.pszText=pCh;
	lvIt.iSubItem=4;
	pListCtrl->SetItem(&lvIt);
	//status
	CStringToTchar(csStatus,pCh);
	lvIt.pszText=pCh;
	lvIt.iSubItem=5;
	pListCtrl->SetItem(&lvIt);
	//time
	CStringToTchar(csTime,pCh);
	lvIt.pszText=pCh;
	lvIt.iSubItem=6;
	pListCtrl->SetItem(&lvIt);
	delete[] pCh;

}

void CTPInfoView::UpdateQueueInfo()
{
	CString strPrec(L"0");
	int nIter = 0;
	int nCount = m_wndQueueDlg.m_ctList.GetItemCount();
	map<CString,FILE_UPLOAD_INFO>::iterator iter;
	ULONGLONG ullSum = 0 , ullSize;
	for(iter = m_pUploadCount->mpFileInfo.begin() ; iter != m_pUploadCount->mpFileInfo.end() && nIter < nCount; iter++,nIter++)
	{
		if(iter->first == m_wndQueueDlg.m_ctList.GetItemText(nIter,0))
		{
			ullSize = iter->second.ullTpFileSize;
			if((iter->second.ullTPFileByte) <= 0)
				strPrec = L"100%";
			else	
				strPrec.Format(L"%d%%",(int)(100.0*(ullSize + (iter->second.bIsForce?0:iter->second.ullTPDestnFileSize))/(iter->second.ullTPFileByte)));
			ullSum += ullSize - iter->second.ullTPFileLastSize;
			iter->second.ullTPFileLastSize = ullSize;
			m_wndQueueDlg.m_ctList.SetItemText(nIter,4,strPrec);
		}
	}	
	
	//更新状态栏信息:网络速度
	DWORD	time = (GetTickCount() - startTime)/1000;
	if(time <= 0 )
		time = 1;
	strPrec.Format(L"当前网络速度:%s/s", ByteConnvertType(ullSum/time));
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.SetPaneText(1,strPrec);
}

BOOL CTPInfoView::AddUploadFailFileInfo(FILE_UPLOAD_INFO* pFileInfo)
{
	if( m_tFailQueueCount.mpFileInfo.count(pFileInfo->csTPFileName) <= 0)
		return FALSE;

	VERIFY(m_tFailQueueCount.mpFileInfo.insert( pair<CString,FILE_UPLOAD_INFO>(pFileInfo->csTPFileName,*pFileInfo)).second);
	//初始化
	m_tFailQueueCount.mpFileInfo[pFileInfo->csTPFileName].ullTpFileSize = 0;
	m_tFailQueueCount.mpFileInfo[pFileInfo->csTPFileName].ullTPDestnFileSize = 0;
	m_tFailQueueCount.mpFileInfo[pFileInfo->csTPFileName].ullTPFileLastSize = 0;
	m_tFailQueueCount.mpFileInfo[pFileInfo->csTPFileName].uIsTpOk = FILE_TP_JOIN;
	m_tFailQueueCount.mpFileInfo[pFileInfo->csTPFileName].hEvtEndModule = ::CreateEvent ( NULL, TRUE, FALSE, NULL );

	m_tFailQueueCount.nThreadCount++;

	return TRUE;
}

void CTPInfoView::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nCurSel = m_ctTPInfo.GetCurSel();
	switch(nCurSel)
	{
	case FILE_TP_JOIN :
		m_wndQueueDlg.ShowWindow(TRUE);
		m_wndFailureDlg.ShowWindow(FALSE);
		m_wndSuccessDlg.ShowWindow(FALSE);
		break;
	case FILE_TP_FAIL:
		m_wndQueueDlg.ShowWindow(FALSE);
		m_wndFailureDlg.ShowWindow(TRUE);
		m_wndSuccessDlg.ShowWindow(FALSE);
		break;
	case FILE_TP_SUCCESS:
		m_wndQueueDlg.ShowWindow(FALSE);
		m_wndFailureDlg.ShowWindow(FALSE);
		m_wndSuccessDlg.ShowWindow(TRUE);
		break;
	}
}

LRESULT CTPInfoView::OnSHOW_TPFILE_STATUS(WPARAM wParam,LPARAM lParam)
{
	//将SERVEVIEW中的传输文件列表保存
	if(m_pUploadCount == NULL)
		m_pUploadCount = (FILE_UPLOAD_COUNT_INFO*)wParam;
	FILE_UPLOAD_INFO* pInfo = (FILE_UPLOAD_INFO*)lParam;

	UINT  uSign = pInfo->uIsTpOk;
	switch(uSign)
	{
	case FILE_TP_JOIN:
		InsertTPList(&(m_wndQueueDlg.m_ctList),pInfo,FILE_TP_JOIN);
		break;
	case FILE_TP_FAIL:
		//UpdateQueueInfo();
		InsertTPList(&(m_wndFailureDlg.m_ctList),pInfo,FILE_TP_FAIL);
		DeleteTPList(&(m_wndQueueDlg.m_ctList),pInfo,FILE_TP_JOIN);
		break;
	case FILE_TP_SUCCESS:
		//UpdateQueueInfo();
		InsertTPList(&(m_wndSuccessDlg.m_ctList),pInfo,FILE_TP_SUCCESS);
		DeleteTPList(&(m_wndQueueDlg.m_ctList),pInfo,FILE_TP_JOIN);
		break;
	default:
		TRACE(L"error sign");
	}
	m_ctTPInfo.Invalidate();
	return 0L;
}

void CTPInfoView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case TIMER_ID_TO_UPDATETPINFO:
		if(m_pUploadCount == NULL || m_wndQueueDlg.m_hWnd == NULL)
		{
			startTime = GetTickCount();
			return;
		}
		CPublicData::CSFor_UploadCount.Lock();
		UpdateQueueInfo();
		CPublicData::CSFor_UploadCount.Unlock();
		startTime = GetTickCount();
		return;
	default:;
	}
	startTime = GetTickCount();
	__super::OnTimer(nIDEvent);
}

void CTPInfoView::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	return;
	//
	if(m_ctTPInfo.GetCurSel() != FILE_TP_FAIL) 
		return;
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	if(nItem==-1)return;
	SetPopMenu(2);
}

void CTPInfoView::OnFailqueueReupload()
{
	// TODO: 在此添加命令处理程序代码
	if(CPublicData::szFtpDirectory == L"")
	{
		MessageBox(L"未连接服务器",L"NOTE",MB_OK);
		return;
	}

	int nCount = 0;
	POSITION iPos;
	CString  str,csFileName;
	map<CString,int> mp;

	nCount = m_wndFailureDlg.m_ctList.GetSelectedCount();
	if(nCount <= 0)
		return;
	//确认信息
	str.Format(L"	确定重新上传文件?\n选中文件:\n");
	mp.clear();
	iPos=m_wndFailureDlg.m_ctList.GetFirstSelectedItemPosition();
	for(int j=0;j<nCount;j++)
	{
		int nItem=m_wndFailureDlg.m_ctList.GetNextSelectedItem(iPos);
		csFileName = m_wndFailureDlg.m_ctList.GetItemText(nItem,0);
		if(mp.count(csFileName) <= 0)
		{
			mp.insert( pair<CString,int> (csFileName , 1));
			str += csFileName + L"\n";
		}
	}
	if(MessageBox(str,L"CHECK",MB_YESNO|MB_DEFBUTTON2) == IDNO)	
		return;
	//
	//CPublicData::CSFor_UploadCount.Lock();

	map<CString,int>::iterator nIter;
	for(nIter = mp.begin() ; nIter != mp.end() ; nIter++)
	{
		if(m_tFailQueueCount.mpFileInfo.count(nIter->first) > 0)
		{
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pServeView->SendMessage(WM_UPLOAD_FILE,NULL,(LPARAM)(&(m_tFailQueueCount.mpFileInfo[nIter->first])));
		}
	}

	//CPublicData::CSFor_UploadCount.Unlock();
}
