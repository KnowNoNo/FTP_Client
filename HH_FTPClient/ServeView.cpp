// ServeView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "ServeView.h"
#include "NewDirDialog.h"
#include "TPInfoView.h"
#include "MainFrame.h"
#include "DelDirDialog.h"
#include "PublicFunction.h"
// CServeView

IMPLEMENT_DYNCREATE(CServeView, CFormView)

CServeView::CServeView()
	: CFormView(CServeView::IDD)
{
	m_bHaveDotFlag        = FALSE;
	//m_nTPStatusTimer      = NULL;
	m_pFindServFileThread = NULL;
	m_pFtpCmd             = NULL;
	m_pDragImageList      = NULL;
	m_pSendFileThread     = NULL;
	m_tUploadCount.nThreadCount = 0;
	m_tUploadCount.mpFileInfo.clear();
}

CServeView::~CServeView()
{
	if(m_ctServImageList.m_hImageList)
		m_ctServImageList.Detach();
	if(m_pFindServFileThread)
	{
		::WaitForSingleObject(m_pFindServFileThread->m_hThread,100);
		delete m_pFindServFileThread;
	}
	if(m_pSendFileThread)
	{
		::WaitForSingleObject(m_pSendFileThread->m_hThread,100);
		delete m_pSendFileThread;
	}
	DisConnect();
}

void CServeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BACK, m_ctBack);
	DDX_Control(pDX, IDC_SERV_LIST, m_ctServFile);
	DDX_Control(pDX, IDC_SERV_DIR, m_ctServDir);
	DDX_Control(pDX, IDC_STATIC_SERVSTATUS, m_ctServStatus);
}

BEGIN_MESSAGE_MAP(CServeView, CFormView)
	ON_WM_SIZE()
	
	ON_MESSAGE(WM_CONNECTSERVE,OnConnectServe)
	ON_MESSAGE(WM_UPLOAD_FILE,OnUploadFile)
	ON_NOTIFY(NM_DBLCLK, IDC_SERV_LIST, &CServeView::OnNMDblclkServList)
	ON_BN_CLICKED(IDC_BACK, &CServeView::OnBnClickedBack)
	ON_COMMAND(ID_SER_REFRESH, &CServeView::OnSerRefresh)
	ON_NOTIFY(NM_RCLICK, IDC_SERV_LIST, &CServeView::OnNMRClickServList)
	//ON_WM_TIMER()
	ON_COMMAND(ID_SER_CREAT_DIR, &CServeView::OnSerCreatDir)
	ON_COMMAND(ID_SER_DELETE_DIR, &CServeView::OnSerDeleteDir)
	ON_COMMAND(ID_SER_DOWNLOAD, &CServeView::OnSerDownload)
END_MESSAGE_MAP()


// CServeView ���

#ifdef _DEBUG
void CServeView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CServeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CServeView ��Ϣ�������

void CServeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//�ػ�
	CRect rect;GetClientRect(&rect);
	OnSize(1,rect.Width(),rect.Height());
	//
	CEdit* pEdit = (CEdit*)m_ctServDir.GetEditCtrl();
	pEdit->SetReadOnly(TRUE);

	m_ctServFile.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//back��ť��ʼ��
	hBitmap=::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BACK_BITMAP));
	m_ctBack.SetBitmap(hBitmap);
	//listctrl��ʼ��
	SetFileColumns(&m_ctServFile);	
	//SetServImageList(IDB_FILE_BITMAP);
	SetImageList(&m_ctServFile,NULL);
	//
	Log(L_NORMAL,INFODESK_BEGIN_SHOW);
}

BOOL CServeView::SetServImageList(UINT nBitmapID)
{
	if(!m_ctServImageList.Create(nBitmapID,16,1,RGB(0,255,0)))
	{
		TRACE(L"λͼ����ʧ�� ID = %d ",nBitmapID);
		return FALSE;
	}
		
	m_ctServFile.SetImageList(&m_ctServImageList,LVSIL_SMALL);
	return TRUE;
}

void CServeView::SetServerCtlDirectoryDisplay()
{
	CString str;
	str=CPublicData::szFtpDirectory;
	m_ctServDir.SetWindowText(str);
	m_ctServDir.Invalidate();
}

void CServeView::GetServerCurrentDirectory()
{
	CString str;
	m_ctServDir.GetWindowText(str);
	CPublicData::szFtpDirectory = str;
}

void CServeView::SetServerRedrawFlag()
{
	if(m_bHaveDotFlag)
	{
		CString str=L"..";
		int iIcon=DIRECTORYICON;
		m_ctServFile.InsertItem(0,str,iIcon);
		m_ctServFile.SetItemData(0,DIRECTORYICON);
		m_bHaveDotFlag=FALSE;
	}
	m_ctServFile.Invalidate();
	m_ctServFile.SetRedraw();
}


void CServeView::InsertServeFile(FILE_FTP_INFO* pInfo,int lParam,UINT& nDirNum)
{
	CString str=pInfo->szFileName;
	if(str=="."||str=="..")
	{
		m_bHaveDotFlag=TRUE;
		return ;
	}
	InsertFile(&m_ctServFile,pInfo,lParam,nDirNum);
}

BOOL CServeView::AddUploadFileInfo(FILE_UPLOAD_INFO* pFileInfo)
{
	if(pFileInfo == NULL) return FALSE;
	bool bNet = FALSE;
	CString csName = pFileInfo->csTPFileName;
	
	CPublicData::CSFor_UploadCount.Lock();
	if(m_tUploadCount.nThreadCount >= MAX_UPLOAD_THREAD_SIZE)
	{
		MessageBox(L"�ϴ��ļ�������");
	}
	else if(m_tUploadCount.mpFileInfo.count(csName) < 1)
	{
		VERIFY(m_tUploadCount.mpFileInfo.insert( pair<CString,FILE_UPLOAD_INFO>(csName,*pFileInfo)).second);
		m_tUploadCount.nThreadCount++;
		bNet = TRUE;
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pTPInfoView->SendMessage(WM_SHOW_TPFILE_STATUS,
			(WPARAM)&m_tUploadCount,(LPARAM)pFileInfo);
	}
	else
	{
		csName.Format(L"%s�ļ����ڴ��������!",csName);
		MessageBox(csName,L"NOTE",MB_OK);
	}

	CPublicData::CSFor_UploadCount.Unlock();
	//
	
	return bNet;
}

BOOL CServeView::DelUploadFileInfo(FILE_UPLOAD_INFO* pFileInfo)
{
	if(pFileInfo == NULL) return FALSE;
	bool bNet = FALSE;
	CString csName = pFileInfo->csTPFileName;

	CPublicData::CSFor_UploadCount.Lock();
	if(m_tUploadCount.mpFileInfo.count(csName) > 0)
	{
		//delete m_tUploadCount.mpFileInfo[csName].pUploadThread;
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pTPInfoView->SendMessage(WM_SHOW_TPFILE_STATUS,
			(WPARAM)&m_tUploadCount,(LPARAM)pFileInfo);

		VERIFY(m_tUploadCount.mpFileInfo.erase(csName));
		m_tUploadCount.nThreadCount--;
		bNet = TRUE;
	}
	CPublicData::CSFor_UploadCount.Unlock();
	return bNet;
}

BOOL CServeView::UploadOrDownloadFileOnce(CFTPCmdClass* pCmd,FILE_UPLOAD_INFO* pUploadInfo)
{
	//��¼
	if(!pCmd->LogOn(pUploadInfo->csServIp,pUploadInfo->nServPort,
		pUploadInfo->csServUser,pUploadInfo->csServPassword))
	{
		Log(L_WARNING,L"��¼ʧ��!");
		return FALSE;
	}
	
	//�ϴ��ļ� 
	if(pUploadInfo->bTPMode == false)
	{
		if(!pCmd->UploadFile(pUploadInfo->csLocFileDir,pUploadInfo->csTPFileName,pUploadInfo->ullTPFileByte,pUploadInfo->csServFileDir,&(pUploadInfo->ullTPDestnFileSize),
			&(pUploadInfo->ullTpFileSize),pUploadInfo->hEvtEndModule,pUploadInfo->bIsPort,pUploadInfo->bIsForce))
		{
			return FALSE;
		}
	}
	else//�����ļ�
	{	
		if(!pCmd->DownloadFile(pUploadInfo->csLocFileDir,pUploadInfo->csTPFileName,pUploadInfo->ullTPFileByte,pUploadInfo->csServFileDir,&(pUploadInfo->ullTPDestnFileSize),
			&(pUploadInfo->ullTpFileSize),pUploadInfo->hEvtEndModule,pUploadInfo->bIsPort,pUploadInfo->bIsForce))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

void CServeView::SetFindServFileDir(CString csStr/*=CPublicData::szFtpDirectory*/)
{
	m_csPreFindDir = csStr;
}

BOOL CServeView::BeginFindServFile(CString csPreServFileDir)
{
	m_CSFor_FindServFile.Lock();
	if(m_pFindServFileThread)
	{
		::WaitForSingleObject(m_pFindServFileThread->m_hThread,INFINITE);
		delete m_pFindServFileThread;
		m_pFindServFileThread=NULL;
	}
	SetFindServFileDir(csPreServFileDir);
	if(m_pFindServFileThread=AfxBeginThread(CMyAppThread::ThreadProc_FindServFile,(LPVOID)this,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pFindServFileThread->m_bAutoDelete=FALSE;
		m_ctServFile.SetRedraw(FALSE);
		m_ctServFile.DeleteAllItems();
		m_pFindServFileThread->ResumeThread();
		m_CSFor_FindServFile.Unlock();
		return TRUE;
	}
	m_CSFor_FindServFile.Unlock();
	return FALSE;
}

BOOL CServeView::BeginUploadOrDownloadFile(CString csFileName)
{
	
	if(m_tUploadCount.mpFileInfo[csFileName].pUploadThread = AfxBeginThread(CMyAppThread::ThreadProc_UploadOrDownloadFile,
		(LPVOID)&(m_tUploadCount.mpFileInfo[csFileName]),THREAD_PRIORITY_ABOVE_NORMAL,0,CREATE_SUSPENDED,NULL))
	{
		//m_tUploadCount.mpFileInfo[csFileName].pUploadThread->m_bAutoDelete = FALSE;
		m_tUploadCount.mpFileInfo[csFileName].pUploadThread->ResumeThread();

		return TRUE;
	}
	else
	{
		Log(L_WARNING,L"%s �ļ�����������ʧ��",csFileName);
	}
	return FALSE;

}

BOOL CServeView::BeginCreateDir()
{
	m_CSFor_FindServFile.Lock();
	if(m_pFindServFileThread)
	{
		::WaitForSingleObject(m_pFindServFileThread->m_hThread,INFINITE);
		delete m_pFindServFileThread;
		m_pFindServFileThread=NULL;
	}
	if(m_pFindServFileThread=AfxBeginThread(CMyAppThread::ThreadProc_CreateServDir,(LPVOID)this,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pFindServFileThread->m_bAutoDelete=FALSE;
		m_pFindServFileThread->ResumeThread();
		m_CSFor_FindServFile.Unlock();
		return TRUE;
	}
	m_CSFor_FindServFile.Unlock();
	return FALSE;
}

BOOL CServeView::BeginDeleteDir()
{
	m_CSFor_FindServFile.Lock();
	if(m_pFindServFileThread)
	{
		::WaitForSingleObject(m_pFindServFileThread->m_hThread,INFINITE);
		delete m_pFindServFileThread;
		m_pFindServFileThread=NULL;
	}
	if(m_pFindServFileThread=AfxBeginThread(CMyAppThread::ThreadProc_DeleteServDir,(LPVOID)this,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pFindServFileThread->m_bAutoDelete=FALSE;;
		m_pFindServFileThread->ResumeThread();
		m_CSFor_FindServFile.Unlock();
		return TRUE;
	}
	m_CSFor_FindServFile.Unlock();
	return FALSE;
}

void CServeView::ThreadProc_FindServFile()
{
	if(m_pFtpCmd == NULL || m_pFtpCmd->m_bIsConnect == FALSE)
	{
		DisConnect();
		m_pFtpCmd = new CFTPCmdClass;
		if(!m_pFtpCmd->LogOn(CPublicData::szFtpName,CPublicData::nPort,
			CPublicData::szFtpUser,CPublicData::szFtpPassword))
		{
			CPublicData::bButtonEnable = TRUE;
			DisConnect();
			Log(L_WARNING,L"��¼ʧ��!");
			//goto end;
			SetServerRedrawFlag();
			return;
		}
	}
	//��½�ɹ�
	CPublicData::bButtonEnable = FALSE;

	//��ʾserver�ļ�
	if(!m_pFtpCmd->List(m_csPreFindDir))
	{
		Log(L_DEBUG,L"�ļ��б���ʾʧ��!");
		//goto end;
		SetServerRedrawFlag();
		return;
	}

	//edit��ʾ
	CPublicData::szFtpDirectory = m_csPreFindDir;
	SetServerCtlDirectoryDisplay();

	FILE_FTP_INFO info;
	int nIndex = 0;
	UINT nDirNum = 0;
	CString str;
	ULONGLONG ullSum = 0;
	while(m_pFtpCmd->GetFileItem(nIndex))
	{
		TransformLine(m_pFtpCmd->m_csLine,info);
		if(info.nType == DIRECTORYICON)
		{
			InsertServeFile(&info,DIRECTORYICON,nDirNum);
		}
		else
		{
			str.Format(L"%s",info.szFileByte);
			ullSum += CStringToLong(str);
			InsertServeFile(&info,SERVFILE,nDirNum);
		}
	}
//end:	
	//
	str.Format(L"�� %d ���ļ� �� %d ��Ŀ¼.��С��%s",m_ctServFile.GetItemCount()-nDirNum,nDirNum,ByteConnvertType(ullSum));
	m_ctServStatus.SetWindowText(str);
	//��ʾList�ļ�
	SetServerRedrawFlag();
	return ;
}

void CServeView::ThreadProc_UploadOrDownloadFile(FILE_UPLOAD_INFO* pUploadInfo)
{
	CFTPCmdClass *pUploadCmd = new CFTPCmdClass;
	BOOL	bIsUpload = pUploadInfo->bTPMode;
	CString str;
	ASSERT(pUploadCmd);

	BOOL bRet = FALSE;
	for(int i=0 ; i<DEFAULT_FTP_UPLOAD_TIMES ; i++)
	{
		if(UploadOrDownloadFileOnce(pUploadCmd,pUploadInfo))
		{	
			bRet = TRUE;
			//ˢ��
			if(bIsUpload)//PostMessage ��Ϣ����ֳ�����
				//((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pLocView->OnLocRefresh();
				::SendMessage(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pLocView->m_hWnd,WM_COMMAND,ID_LOC_REFRESH,NULL);
			else
				OnSerRefresh();
			TRACE( L"%s�ļ� ��%d�ν���%s\n", pUploadInfo->csTPFileName, i,bIsUpload?L"����":L"�ϴ�");
			break;
		}
		else if(::WaitForSingleObject ( pUploadInfo->hEvtEndModule, 0 ) == WAIT_OBJECT_0 )
			break;
	}
	//send 
	pUploadInfo->uIsTpOk = bRet?FILE_TP_SUCCESS:FILE_TP_FAIL;

	Log(L_NORMAL,L"%s�ļ�%s���� �����%s!", pUploadInfo->csTPFileName, bIsUpload?L"����":L"�ϴ�",bRet?L"�ɹ�":L"ʧ��" );
	
	//delete����
	DelUploadFileInfo(pUploadInfo);

	delete pUploadCmd;
	pUploadCmd = NULL;
	return ;//
}

void CServeView::ThreadProc_CreateServDir()
{
	if(m_pFtpCmd == NULL || m_pFtpCmd->m_bIsConnect == FALSE)
	{
		DisConnect();
		CPublicData::bButtonEnable = TRUE;
		Log(L_WARNING,L"�������Ͽ�����!���������ӷ�����");
		return ;
	}

	if( ! m_pFtpCmd->CreateDir(m_csPreCreatDir) )
	{	
		Log(L_WARNING,L"�ļ���%s ����ʧ��",m_csPreCreatDir);
		return;
	}
	//����ˢ����Ϣ
	::PostMessage(m_hWnd,WM_COMMAND,ID_SER_REFRESH,NULL);
}

void CServeView::ThreadProc_DeleteServDir()
{
	int      nCount = 0;
	bool     bFile  = true;
	POSITION iPos;
	CString  str;
	CDelDirDialog dlg;
	nCount=m_ctServFile.GetSelectedCount();
	if(nCount <= 0)
	{
		MessageBox(L"��ѡ���ļ�",L"NOTE",MB_OK);
		return;
	}
	//
	if(m_pFtpCmd == NULL || m_pFtpCmd->m_bIsConnect == FALSE)
	{
		DisConnect();
		CPublicData::bButtonEnable = TRUE;
		Log(L_WARNING,L"�������Ͽ�����!���������ӷ�����");
		return ;
	}
	//
	iPos=m_ctServFile.GetFirstSelectedItemPosition();
	for(int j=0;j<nCount;j++)
	{
		int nItem=m_ctServFile.GetNextSelectedItem(iPos);
		//Ŀ¼
		if(m_ctServFile.GetItemText(nItem,3) == L"")
			bFile = FALSE;
		else bFile = TRUE;
		//���������ļ������趨
		if(!dlg.m_bKeep)
		{
			dlg.m_csName.Format(L"ȷ��ɾ��%s%s",bFile?L"�ļ�: ":L"Ŀ¼: ",m_ctServFile.GetItemText(nItem,0));
			if(dlg.DoModal() == IDCANCEL)	
				continue;
		}
		else if(dlg.m_bDel == false)
			break;
		//
		if(! m_pFtpCmd->DeleteDirOrFile(m_ctServFile.GetItemText(nItem,0),
			bFile?FILEICON:DIRECTORYICON) )
		{
			CString s;
			s.Format(L"%s ɾ��ʧ��(��Ŀ¼�º����ļ����޷�ɾ��)",m_ctServFile.GetItemText(nItem,0));
			MessageBox(s,L"Warning",MB_OK);
		}
	}
	//����ˢ����Ϣ
	::PostMessage(m_hWnd,WM_COMMAND,ID_SER_REFRESH,NULL);
}

void CServeView::DisConnect()
{
	if(m_pFtpCmd)
	{
		delete m_pFtpCmd;
		m_pFtpCmd = NULL;
	}
	
}

void CServeView::OnSize(UINT nType, int cx, int cy)
{
	//CFormView::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	if(m_ctServDir.m_hWnd&&m_ctBack.m_hWnd)
	{
		m_ctServDir.SetWindowPos(this,0,0,cx-32,0,SWP_NOZORDER|SWP_NOMOVE);
		m_ctBack.SetWindowPos(this,cx-30,0,0,0,SWP_NOZORDER|SWP_NOSIZE);
	}
	if(m_ctServFile.m_hWnd)
	{
		m_ctServFile.ShowWindow(SW_HIDE);
		m_ctServFile.SetWindowPos(this,0,0,cx,cy-50,SWP_NOZORDER|SWP_NOMOVE);
		m_ctServFile.ShowWindow(SW_SHOW);
	}
	if(m_ctServStatus.m_hWnd)
		m_ctServStatus.MoveWindow(0,cy-18,cx,20);
}

LRESULT CServeView::OnConnectServe(WPARAM wParam,LPARAM lParam)
{
	if(m_pFindServFileThread)
	{
		if(::WaitForSingleObject(m_pFindServFileThread->m_hThread,0) != WAIT_OBJECT_0)
		{
			Log(L_NORMAL,L"����������......");
			return 0L;
		}
	}
	BeginFindServFile(DEFAULT_FTP_INITDIRECTORY);
	return 0L;
}


LRESULT CServeView::OnDisconnectServe(WPARAM wParam,LPARAM lParam)
{
	//�Ͽ�SOCKETͨ��ͨ��
	DisConnect();
	//�Ͽ����ػ����ϴ��ļ�����ͨ��
	CPublicData::CSFor_UploadCount.Lock();

	map<CString,FILE_UPLOAD_INFO>::iterator iter;
	for(iter = m_tUploadCount.mpFileInfo.begin() ; iter != m_tUploadCount.mpFileInfo.end() ; iter++)
	{
		//����������
		if(iter->second.pUploadThread && HANDLE_IS_VALID(iter->second.hEvtEndModule))
		{
			::SetEvent ( iter->second.hEvtEndModule );
		}
	}

	CPublicData::CSFor_UploadCount.Unlock();
	
	//��ʼ����Ϣ
	m_ctServDir.SetWindowText(L"");
	m_ctServFile.DeleteAllItems();
	m_ctServStatus.SetWindowText(L"");
	CPublicData::szFtpDirectory = L"";
	CPublicData::bButtonEnable = true;
	return 0L;
}

LRESULT CServeView::OnUploadFile(WPARAM wParam,LPARAM lParam)
{
	
	if(!AddUploadFileInfo((FILE_UPLOAD_INFO*)lParam))	
		return 0L;
	BeginUploadOrDownloadFile(((FILE_UPLOAD_INFO*)lParam)->csTPFileName);

	return 0L;
}

void CServeView::OnNMDblclkServList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NMLVDISPINFO* pServInfo=(NMLVDISPINFO*)pNMHDR;
	CString str,csFileName;
	int nItem;
	DWORD dwFlag;
	//TCHAR curPath[MAX_PATH];

	str.Format(L"%d",pServInfo->item);
	if(str==L"-1")
	{
		TRACE(L"select item error,str = %s",str);
		return;
	}
	nItem=_ttoi(str);
	dwFlag=m_ctServFile.GetItemData(nItem);
	// ��Ϊ�ļ�
	if(dwFlag==FILEICON)	return;
	//	
	m_ctServDir.GetWindowText(str);
	csFileName = m_ctServFile.GetItemText(nItem,0);
	if(str.Right(1)!=L"/")
		str+=L"/";
	str += csFileName;

	//�߳�����
	BeginFindServFile(str);
	*pResult = 0;
}

void CServeView::OnBnClickedBack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetServerCurrentDirectory();
	if(CPublicData::szFtpDirectory == L"")return;
	str = CPublicData::szFtpDirectory;
	if(str.Right(1)!=L"/")
		str+=L"/";
	str+=L"..";

	BeginFindServFile(str);
}

void CServeView::OnSerRefresh()
{
	// TODO: �ڴ���������������
	BeginFindServFile(CPublicData::szFtpDirectory);
}

void CServeView::OnNMRClickServList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//
	if(CPublicData::szFtpDirectory == L"")return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	CMenu* pPopMenu=m_LocServMenu.GetSubMenu(1);

	if(nItem==-1)
	{
		pPopMenu->EnableMenuItem(ID_SER_REFRESH,MF_BYCOMMAND | MF_ENABLED);
		pPopMenu->EnableMenuItem(ID_SER_CREAT_DIR,MF_BYCOMMAND | MF_ENABLED);
		pPopMenu->EnableMenuItem(ID_SER_DELETE_DIR, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	else
	{
		pPopMenu->EnableMenuItem(ID_SER_REFRESH , MF_ENABLED);
		pPopMenu->EnableMenuItem(ID_SER_CREAT_DIR , MF_ENABLED);
		pPopMenu->EnableMenuItem(ID_SER_DELETE_DIR , MF_ENABLED);

	}
	SetPopMenu(1);
}

void CServeView::OnSerCreatDir()
{
	// TODO: �ڴ���������������

	CNewDirDialog dlg;
	dlg.m_csEdit = CPublicData::szFtpDirectory;
	if(dlg.m_csEdit.Right(1) != L"/") 
		dlg.m_csEdit += L"/";
	if(dlg.DoModal() == IDOK)
	{
		m_csPreCreatDir = dlg.m_csEdit;
		if(m_csPreCreatDir.IsEmpty()) return;
		BeginCreateDir();
	}
}

void CServeView::OnSerDeleteDir()
{
	// TODO: �ڴ���������������
	BeginDeleteDir();
}

void CServeView::OnSerDownload()
{
	//CLocView* pLocView = ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pLocView;
	int nCount = 0;
	POSITION iPos;
	CString  str;
	nCount=m_ctServFile.GetSelectedCount();
	if(nCount <= 0)
	{
		MessageBox(L"��ѡ���ļ�",L"NOTE",MB_OK);
		return;
	}
	//
	str.Format(L"	ȷ�������ļ�?\nѡ���ļ�:\n");
	iPos=m_ctServFile.GetFirstSelectedItemPosition();
	for(int j=0;j<nCount;j++)
	{
		int nItem=m_ctServFile.GetNextSelectedItem(iPos);
		if(m_ctServFile.GetItemData(nItem) == FILEICON)
		{
			str += m_ctServFile.GetItemText(nItem,0) + L"\n";
		}
	}
	if(MessageBox(str,L"CHECK",MB_YESNO|MB_DEFBUTTON2) == IDNO)	
		return;

	FILE_UPLOAD_INFO pFileCount;
	pFileCount.csServFileDir	 = CPublicData::szFtpDirectory;
	pFileCount.csServIp			 = CPublicData::szFtpName;
	pFileCount.nServPort		 = CPublicData::nPort;
	pFileCount.csServUser		 = CPublicData::szFtpUser;
	pFileCount.csServPassword	 = CPublicData::szFtpPassword;
	pFileCount.csLocFileDir		 = CPublicData::szLocDirectory;
	pFileCount.bIsPort			 = CPublicData::bIsPort;
	pFileCount.bIsForce			 = CPublicData::bIsForce;
	pFileCount.bTPMode			 = true; //����
	pFileCount.uIsTpOk			 = FILE_TP_JOIN;	
	pFileCount.ullTPDestnFileSize= 0;
	pFileCount.ullTpFileSize	 = 0;
	pFileCount.ullTPFileLastSize = 0;
	pFileCount.hEvtEndModule     = ::CreateEvent ( NULL, TRUE, FALSE, NULL );
	
	iPos=m_ctServFile.GetFirstSelectedItemPosition();
	for(int j=0;j<nCount;j++)
	{
		int nItem=m_ctServFile.GetNextSelectedItem(iPos);
		if(m_ctServFile.GetItemData(nItem) == FILEICON)
		{	
			pFileCount.csTPFileName  = m_ctServFile.GetItemText(nItem,0);
			pFileCount.ullTPFileByte  = CStringToLong(m_ctServFile.GetItemText(nItem,3));
			//pFileCount.ullServFileSize= pLocView->FindCurDirFileSize(pFileCount.csLocFileName);
			//���봫������
			if(AddUploadFileInfo((&pFileCount)))
				BeginUploadOrDownloadFile(pFileCount.csTPFileName);
		}
	}
}
