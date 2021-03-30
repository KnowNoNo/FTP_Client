// LocView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "LocView.h"
#include "DelDirDialog.h"
#include "MainFrame.h"
//#include "MyAppThread.h"

// CLocView

IMPLEMENT_DYNCREATE(CLocView, CFormView)

CLocView::CLocView()
	: CFormView(CLocView::IDD)
{
	m_bHaveDotFlag			=FALSE;
	m_pSendFileThread		=NULL;
	m_pFindLocFileThread	=NULL;
}

CLocView::~CLocView()
{
}

void CLocView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOC_LIST, m_ctLocFile);
	DDX_Control(pDX, IDC_LOC_DIR, m_ctLocDir);
	DDX_Control(pDX, IDC_STATIC_LOCSTATUS, m_ctLocStatus);
}

BEGIN_MESSAGE_MAP(CLocView, CFormView)
	ON_WM_SIZE()

	ON_NOTIFY(NM_DBLCLK, IDC_LOC_LIST, &CLocView::OnNMDblclkLocList)
	ON_CBN_SELCHANGE(IDC_LOC_DIR, &CLocView::OnCbnSelchangeLocDir)
	ON_NOTIFY(NM_RCLICK, IDC_LOC_LIST, &CLocView::OnNMRClickLocList)
	ON_COMMAND(ID_LOC_OPEN, &CLocView::OnLocOpen)
	ON_COMMAND(ID_LOC_DELETE, &CLocView::OnLocDelete)
	ON_COMMAND(ID_LOC_SEND, &CLocView::OnLocSend)
	ON_COMMAND(ID_LOC_REFRESH, &CLocView::OnLocRefresh)
END_MESSAGE_MAP()


// CLocView ���

#ifdef _DEBUG
void CLocView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLocView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLocView ��Ϣ�������

void CLocView::OnSize(UINT nType, int cx, int cy)
{
	//CFormView::OnSize(nType, cx, cy);

	if(m_ctLocDir.m_hWnd)
		m_ctLocDir.SetWindowPos(this,0,0,cx,0,SWP_NOZORDER|SWP_NOMOVE);
	if(m_ctLocFile.m_hWnd)
	{
		m_ctLocFile.ShowWindow(SW_HIDE);
		m_ctLocFile.SetWindowPos(this,0,0,cx,cy-50,SWP_NOZORDER|SWP_NOMOVE);
		m_ctLocFile.ShowWindow(SW_SHOW);
	}
	if(m_ctLocStatus.m_hWnd)
		m_ctLocStatus.MoveWindow(0,cy-18,cx,20);
	
	// TODO: �ڴ˴������Ϣ����������
}

void CLocView::InitDirCur()
{
	//��ʼ���ؼ�
	SetFileColumns(&m_ctLocFile);
	SetImageList(&m_ctLocFile,&m_ctLocDir);
	m_ctLocFile.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	// ��ȡ�����ļ�ϵͳĿ¼
	GetLocDir();
	int nCur=m_ctLocDir.FindStringExact(-1,LOCAL_DEFAULT_DIRECTORYL);
	if(nCur < 0)
	{
		MessageBox(L"��ʼ��ʧ�ܣ��Ҳ���C��",L"WARNING",MB_OK);
		return ;
	}
	m_ctLocDir.SetCurSel(nCur);
	::SetCurrentDirectory(LOCAL_DEFAULT_DIRECTORYL);
	GetLocCurrentDirectory();
}

void CLocView::GetLocDir()
{
	int iIcon,iIconSel;
	TCHAR lpBuffer[MAX_DIR_SIZE];
	TCHAR* ptr;
	CString str;
	m_iDirCount=0;
	::GetLogicalDriveStrings(MAX_DIR_SIZE,lpBuffer);
	ptr = lpBuffer;
	while(*ptr)
	{
		m_iDirCount++;
		str=ptr;
		GetFileIcon(str,&iIcon,&iIconSel);
		InsertLocDir(str,iIcon,iIconSel);
		ptr+=_tcslen(ptr)+1;
	}
}

int CLocView::InsertLocDir(CString& str,int iIcon,int iIconSel)
{
	int nItem=m_ctLocDir.GetCount();
	COMBOBOXEXITEM Combo;
	Combo.mask=CBEIF_IMAGE|CBEIF_TEXT|CBEIF_SELECTEDIMAGE;
	Combo.iImage=iIcon;
	Combo.iSelectedImage=iIconSel;
	Combo.pszText=(LPTSTR)(LPCTSTR)str;
	Combo.iItem=nItem;
	m_ctLocDir.InsertItem(&Combo);
	return nItem;
}

void CLocView::GetLocCurrentDirectory()
{
	CString str;
	m_ctLocDir.GetWindowText(str);
	CPublicData::szLocDirectory = str;
}

void CLocView::SetFindLocFileDir(CString csStr/*=CPublicData::szLocDirectory*/)
{
	m_csPreFindDir = csStr;
}

//����CPubLicData::szLocDirectory·������m_ctLocFile�ؼ�����
void CLocView::ThreadProc_FindLocFile()
{

	if(!::SetCurrentDirectory(m_csPreFindDir))
	{
		AfxMessageBox(L"·��������",MB_OK);
		SetLocRedrawFlag();
		return ;
	}
	FILE_FTP_INFO* pInfo=new (FILE_FTP_INFO);
	CFileFind	   fileFind;
	ULONGLONG	   i=0,ullSum=0;
	UINT	       nDirNum=0;
	CTime		   time;
	CString		   str;
	BOOL		   bContinue;
	
	//edit��ʾ
	CPublicData::szLocDirectory = m_csPreFindDir;
 	SetLocCtlDirectoryDisplay();

	bContinue=fileFind.FindFile(L"*.*");
	while(bContinue)
	{
		bContinue=fileFind.FindNextFile();
		//�����ļ�����ʾ
		if(fileFind.IsHidden())continue;
		str=fileFind.GetFileName();
		_tcscpy_s(pInfo->szFileName,str);
		fileFind.GetLastWriteTime(time);
		//str=time.Format(L"%x");
		str.Format(L"%04d-%02d-%02d %02d:%02d:%02d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
		_tcscpy_s(pInfo->szFileDate,str);
		if(fileFind.IsDirectory())
		{
			_tcscpy_s(pInfo->szFileSize,L"");
			_tcscpy_s(pInfo->szFileByte,L"");
			pInfo->nType=DIRECTORYICON;
			InsertLocFile(pInfo,LOCFILE,nDirNum);
		}
		else
		{
			i = fileFind.GetLength();
			ullSum += i;
			str = ByteConnvertType(i);
			_tcscpy_s(pInfo->szFileSize,str);
			str.Format(L"%llu",i);
			_tcscpy_s(pInfo->szFileByte,str);
			pInfo->nType=FILEICON;
			InsertLocFile(pInfo,LOCFILE,nDirNum);
		}
	}
	//
	str.Format(L"�� %d ���ļ� �� %d ��Ŀ¼. ��С��%s",m_ctLocFile.GetItemCount()-nDirNum,nDirNum,ByteConnvertType(ullSum));
	m_ctLocStatus.SetWindowText(str);
	//
	SetLocRedrawFlag();

	delete pInfo;
	fileFind.Close();
	return ;

}

BOOL CLocView::BeginFindLocFile()
{
	m_CSFor_FindLocFile.Lock();
	if(m_pFindLocFileThread)		// ���߳��Ѿ����У���ȴ�����
	{
		::WaitForSingleObject(m_pFindLocFileThread->m_hThread,INFINITE);
		delete m_pFindLocFileThread;
		m_pFindLocFileThread=NULL;
	}
	// ��ʼ���̣�����SUSPENDED��ʽ��ʼ�����ú���Ӧ����������߳�
	if(m_pFindLocFileThread=AfxBeginThread(CMyAppThread::ThreadProc_FindLocFile,(LPVOID)this,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pFindLocFileThread->m_bAutoDelete=FALSE;
		m_ctLocFile.SetRedraw(FALSE);
		m_ctLocFile.DeleteAllItems();
		m_pFindLocFileThread->ResumeThread();
		m_CSFor_FindLocFile.Unlock();
		return TRUE;
	}
	m_ctLocFile.SetRedraw(FALSE);
	m_CSFor_FindLocFile.Unlock();
	return FALSE;
}

void CLocView::SetLocCtlDirectoryDisplay()
{
	CString str;
	str=CPublicData::szLocDirectory;
	m_ctLocDir.SetWindowText(str);
	m_ctLocDir.Invalidate();
}

//����List�ؼ��� pInfo��ʽ lparamΪLOCFILE���ȡϵͳͼ�꣬����ΪpInfo
void CLocView::InsertLocFile(FILE_FTP_INFO* pInfo,int lParam,UINT& nDirNum)
{
	CString str=pInfo->szFileName;
	if(str=="."||str=="..")
	{
		m_bHaveDotFlag=TRUE;
		return ;
	}
	InsertFile(&m_ctLocFile,pInfo,lParam,nDirNum);
}

void CLocView::SetLocRedrawFlag()
{
	if(m_bHaveDotFlag)
	{
		CString str=L"..";
		int iIcon=DIRECTORYICON;
		m_ctLocFile.InsertItem(0,str,iIcon);
		m_ctLocFile.SetItemData(0,DIRECTORYICON);
		m_bHaveDotFlag=FALSE;
	}

	m_ctLocFile.Invalidate();
	m_ctLocFile.SetRedraw();
	return ;	


}
//
void CLocView::LocFileOpen()
{
	GetLocCurrentDirectory();
	CString str=CPublicData::szLocDirectory;
	POSITION iPos=m_ctLocFile.GetFirstSelectedItemPosition();
	int nItem=m_ctLocFile.GetNextSelectedItem(iPos);
	int i=m_ctLocFile.GetItemData(nItem);
	if(str.Right(1)==L"\\")
		str+=m_ctLocFile.GetItemText(nItem,0);
	else
		str+=L"\\"+m_ctLocFile.GetItemText(nItem,0);
	if(i==FILEICON)
	{
		if(str.Right(4) == L".exe" || str.Right(4) == L".EXE")
			::ShellExecute(m_hWnd,NULL,str,NULL,NULL,SW_SHOWDEFAULT);
		else
			::ShellExecute(m_hWnd,NULL,str,L"explore",CPublicData::szLocDirectory,SW_SHOWDEFAULT);
	}
	else
	{
		//CPublicData::szLocDirectory = str;
		SetFindLocFileDir(str);
		BeginFindLocFile();
	}
}

void CLocView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//// TODO: �ڴ����ר�ô����/����û���
	//�ػ�
	CRect rect;GetClientRect(&rect);
	OnSize(1,rect.Width(),rect.Height());

	//	�༭��ֻ��
	CEdit* pEdit = (CEdit*)m_ctLocDir.GetEditCtrl();
	pEdit->SetReadOnly(TRUE);

	//	dir�ؼ���ʾ
	InitDirCur();
	//	list�ؼ���ʾ
	SetFindLocFileDir(CPublicData::szLocDirectory);
	BeginFindLocFile();	
}


void CLocView::OnNMDblclkLocList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NMLVDISPINFO* pLocInfo=(NMLVDISPINFO*)pNMHDR;
	CString str,csFileName;
	int nItem/*,iIcon,iIconSel*/;
	DWORD dwFlag;
	TCHAR curPath[MAX_DIR_SIZE];

	str.Format(L"%d",pLocInfo->item);
	if(str==L"-1")
	{
		TRACE(L"select item error,str = %s",str);
		return;
	}
	nItem=_ttoi(str);
	dwFlag=m_ctLocFile.GetItemData(nItem);
	// ��Ϊ�ļ�
	if(dwFlag==FILEICON)
	{
		LocFileOpen();
		return;
	}
	// ���õ�ǰĿ¼
	::GetCurrentDirectory(MAX_DIR_SIZE,curPath);
	str=curPath;

	csFileName = m_ctLocFile.GetItemText(nItem,0);
	if(str.Right(1)!=L"\\")
		str+=L"\\";
	str += csFileName;

	//��3�У����������¼��combox �б���
	//GetFileIcon(csFileName,&iIcon,&iIconSel);
	//int i=InsertLocDir(str,iIcon,iIconSel);
	//m_ctLocDir.SetCurSel(i);
	
	//�߳�����
	//���� C:\a\..��C:\a\.  -> C:\ ��� 
	//
	if(::SetCurrentDirectory(str) == FALSE) return;
	if(::GetCurrentDirectory(MAX_DIR_SIZE,curPath) == FALSE) return;
	//CPublicData::szLocDirectory = curPath;
	SetFindLocFileDir(curPath);
	BeginFindLocFile();	
	*pResult = 0;
}

void CLocView::OnCbnSelchangeLocDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	TCHAR lpBuffer[MAX_DIR_SIZE];
	int	  nCur , nPos;

	::GetCurrentDirectory(MAX_DIR_SIZE,lpBuffer);
	//m_ctLocDir.GetWindowText(lpBuffer);
	nPos = m_ctLocDir.GetCurSel();
	m_ctLocDir.GetLBText(nPos,str);

	nCur = 0;
	while(1)
	{
		if(!SetCurrentDirectory(str))
		{
			nCur++;
			if(nCur < 2)//���ƴ����������δ򲻿�
			{
				MessageBox(L"��ѡ���̴򲻿�",L"WARNING",MB_OK);
				continue;
			}
			//��ԭ
			str=lpBuffer;
			if((nPos=m_ctLocDir.FindStringExact(-1,str))!=-1)
				m_ctLocDir.SetCurSel(nPos);
			else
				m_ctLocDir.SetWindowText(str);
		}
		else
		{
			::SetCurrentDirectory(str);
			GetLocCurrentDirectory();
			SetFindLocFileDir(CPublicData::szLocDirectory);
			BeginFindLocFile();
		}
		break;
	}	
}

void CLocView::OnNMRClickLocList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem=pNMListView->iItem;
	if(nItem==-1)return;
	SetPopMenu(0);
}

void CLocView::OnLocOpen()
{
	// TODO: �ڴ���������������
	LocFileOpen();
}
	

void CLocView::OnLocDelete()
{
	// TODO: �ڴ���������������
	POSITION iPos = m_ctLocFile.GetFirstSelectedItemPosition();;
	if(!iPos)return;

	int i;
	SHFILEOPSTRUCT lpsh;
	CString strName,str;
	TCHAR buf[MAX_DIR_SIZE];
	CDelDirDialog dlg;

	ZeroMemory(&lpsh,sizeof(lpsh));
	lpsh.hwnd = HWND_DESKTOP;
	//��Ĭ���������� ������վ
	lpsh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_ALLOWUNDO;
	lpsh.wFunc  = FO_DELETE;
	
	//m_CSFor_ctLocFile.Lock();

	i=m_ctLocFile.GetSelectedCount();
	for(int j=0;j<i;j++)
	{
		int nItem = m_ctLocFile.GetNextSelectedItem(iPos);
		strName = m_ctLocFile.GetItemText(nItem,0);
		//ȷ���Ƿ�ɾ��
		if(!dlg.m_bKeep)	
		{
			dlg.m_csName.Format(L"ȷ��ɾ��%s%s",m_ctLocFile.GetItemData(nItem) == FILEICON ?L"�ļ�: ":L"Ŀ¼: ",m_ctLocFile.GetItemText(nItem,0));
			if(dlg.DoModal() == IDCANCEL)	
				continue;
		}
		else if(dlg.m_bDel == false)
			break;
		//ɾ���ļ�
		CStringToTchar(strName,buf);
		buf[strName.GetLength()+1] = L'\0';
		lpsh.pFrom = buf;
		if( SHFileOperation(&lpsh) )
		{
			CString s;
			s.Format(L"%s ɾ��ʱʧ��",strName);
			MessageBox(s,L"Warning",MB_OK);
		}
	}
	//m_CSFor_ctLocFile.Unlock();

	::GetCurrentDirectory(MAX_DIR_SIZE,buf);
	//CPublicData::szFtpDirectory = buf;
	SetFindLocFileDir(buf);
	BeginFindLocFile();
}

void CLocView::OnLocSend()
{
	// TODO: �ڴ���������������
	if(CPublicData::szFtpDirectory == L"")
	{
		MessageBox(L"δ���ӷ�����",L"NOTE",MB_OK);
		return;
	}

	int nCount = 0;
	POSITION iPos;
	CString  str;
	nCount=m_ctLocFile.GetSelectedCount();
	if(nCount <= 0)
	{
		MessageBox(L"��ѡ���ļ�",L"NOTE",MB_OK);
		return;
	}
	//
	str.Format(L"	ȷ���ϴ��ļ�?\nѡ���ļ�:\n");
	iPos=m_ctLocFile.GetFirstSelectedItemPosition();
	for(int j=0;j<nCount;j++)
	{
		int nItem=m_ctLocFile.GetNextSelectedItem(iPos);
		if(m_ctLocFile.GetItemData(nItem) == FILEICON)
		{
			str += m_ctLocFile.GetItemText(nItem,0) + L"\n";
		}
	}
	if(MessageBox(str,L"CHECK",MB_YESNO|MB_DEFBUTTON2) == IDNO)	
		return;
	//��Ϣ��ʼ��
	FILE_UPLOAD_INFO pFileCount;
	pFileCount.csServFileDir	 = CPublicData::szFtpDirectory;
	pFileCount.csServIp			 = CPublicData::szFtpName;
	pFileCount.nServPort		 = CPublicData::nPort;
	pFileCount.csServUser		 = CPublicData::szFtpUser;
	pFileCount.csServPassword	 = CPublicData::szFtpPassword;
	pFileCount.csLocFileDir		 = CPublicData::szLocDirectory;
	pFileCount.bIsPort			 = CPublicData::bIsPort;
	pFileCount.bIsForce			 = CPublicData::bIsForce;
	pFileCount.bTPMode			 = false; //�ϴ�
	pFileCount.uIsTpOk			 = FILE_TP_JOIN;	
	pFileCount.ullTPDestnFileSize= 0;
	pFileCount.ullTpFileSize	 = 0;
	pFileCount.ullTPFileLastSize = 0;
	pFileCount.hEvtEndModule = ::CreateEvent ( NULL, TRUE, FALSE, NULL );
	//
	iPos=m_ctLocFile.GetFirstSelectedItemPosition();
	for(int j=0;j<nCount;j++)
	{
		int nItem=m_ctLocFile.GetNextSelectedItem(iPos);
		if(m_ctLocFile.GetItemData(nItem) == FILEICON)
		{
			pFileCount.csTPFileName  = m_ctLocFile.GetItemText(nItem,0);
			pFileCount.ullTPFileByte  = CStringToLong(m_ctLocFile.GetItemText(nItem,3));
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pServeView->SendMessage(WM_UPLOAD_FILE,NULL,(LPARAM)(&pFileCount));
		}
	}

}

void CLocView::OnLocRefresh()
{
	// TODO: �ڴ���������������
	BeginFindLocFile();
}
