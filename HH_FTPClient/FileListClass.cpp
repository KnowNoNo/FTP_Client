#include "StdAfx.h"
#include "FileListClass.h"
#include "resource.h"
CFileListClass::CFileListClass(void)
{
	HIMAGELIST hImageList;
	SHFILEINFO shFi;
	// 读取系统文件信息
	hImageList=(HIMAGELIST)SHGetFileInfo(LOCAL_DEFAULT_DIRECTORYL,0,&shFi,sizeof(shFi),
		SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	if(!hImageList)
		return ;
	// 把图像列表加到列表视图空间中
	m_ctImageList.m_hImageList=hImageList;
	// 设定默认鼠标指针形状
	//m_hDragCursor=AfxGetApp()->LoadIcon(IDI_CURSOR);
	m_bDragging=FALSE;
	//
	if(!m_LocServMenu.LoadMenu(IDR_LOC_SERV_MENU))
		AfxThrowResourceException();
}

CFileListClass::~CFileListClass(void)
{
	if(m_ctImageList.m_hImageList)
		m_ctImageList.Detach();
}

void CFileListClass::SetFileColumns(CListCtrl* pListCtrl)
{
	static struct
	{
		LPCTSTR pszText;
		int uiFormat;
	}columns[]={
		_T("文件名"),LVCFMT_LEFT,
		_T("日期"),LVCFMT_CENTER,
		_T("大小   "),LVCFMT_RIGHT,
		_T("字节数"),LVCFMT_CENTER
	};
	for(int i=0,j=120;i<sizeof(columns)/sizeof(columns[0]);i++,j-=10)
		pListCtrl->InsertColumn(i,columns[i].pszText,columns[i].uiFormat,j);

}

BOOL CFileListClass::SetImageList(CListCtrl* pListCtrl,CComboBoxEx* pCombo)
{
	if(pListCtrl)pListCtrl->SetImageList(&m_ctImageList,LVSIL_SMALL);
	if(pCombo)pCombo->SetImageList(&m_ctImageList);
	return TRUE;

}

void CFileListClass::GetFileIcon(CString& fileName,int* iIcon,int* iIconSel/*NULL*/,int nCur/*=LOCFILE*/)
{
	CString str=fileName;
	if(str.Right(1)!=L"\\")
		str+=L"\\";
	SHFILEINFO shFi;
	// 读取图标

	if(!SHGetFileInfo(str,0,&shFi,sizeof(shFi),SHGFI_ICON|
		(nCur==LOCFILE? SHGFI_SMALLICON|SHGFI_DISPLAYNAME:SHGFI_USEFILEATTRIBUTES)))
	{
		TRACE(L"getfileinfo fail!");
		return;
	}

	
	*iIcon=shFi.iIcon;
	DestroyIcon(shFi.hIcon);
	if(iIconSel)
	{
		if(!SHGetFileInfo(str,0,&shFi,sizeof(shFi),SHGFI_ICON|SHGFI_SMALLICON|SHGFI_OPENICON|SHGFI_DISPLAYNAME))
		{
			AfxMessageBox(L"getfileicon fail!");
			return;
		}
		*iIconSel=shFi.iIcon;
		DestroyIcon(shFi.hIcon);
	}
	//	fileName=shFi.szDisplayName;
	return ;
}

void CFileListClass::SetPopMenu(int nIndex)
{
	CMenu* pPopMenu=m_LocServMenu.GetSubMenu(nIndex);
	ASSERT_VALID(pPopMenu);

	//CBitmap bitmapRefresh,bitmapUpload;
	//bitmapUpload.LoadBitmap(IDB_BITMAP1);
	//bitmapRefresh.LoadBitmap(IDB_BITMAP2);
	//if(nIndex == 0)
	//{
	//	pPopMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&bitmapUpload , &bitmapUpload);
	//	pPopMenu->SetMenuItemBitmaps(3,MF_BYPOSITION,&bitmapRefresh, &bitmapRefresh);
	//}
	//else
	//{
	//	pPopMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&bitmapRefresh, &bitmapRefresh);
	//}
	POINT pt;
	::GetCursorPos(&pt);	// 获得当前鼠标位置
	// 显示浮动菜单
	pPopMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,AfxGetMainWnd());

}

void CFileListClass::InsertFile(CListCtrl* pListCtrl,FILE_FTP_INFO* pInfo,int lParam,UINT& nDirNum)
{

	LVITEM	lvIt;
	int iIcon,nItem,iPos;
	CString fileName=pInfo->szFileName;

	if(pInfo->nType==DIRECTORYICON)
		nItem=nDirNum++;
	else
		nItem=pListCtrl->GetItemCount();

	if(lParam==LOCFILE || lParam==SERVFILE)
		GetFileIcon(fileName,&iIcon,NULL,lParam);
	else
		iIcon=pInfo->nType;

	lvIt.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	lvIt.iImage=iIcon;
	lvIt.lParam=pInfo->nType;
	lvIt.pszText=pInfo->szFileName;
	lvIt.iSubItem=0;
	lvIt.iItem=nItem;
	iPos = pListCtrl->InsertItem(&lvIt);
	ASSERT(iPos >= 0);

	lvIt.mask=LVIF_TEXT;
	lvIt.iItem=iPos;
	lvIt.pszText=pInfo->szFileDate;
	lvIt.iSubItem=1;
	pListCtrl->SetItem(&lvIt);
	lvIt.pszText=pInfo->szFileSize;
	lvIt.iSubItem=2;
	pListCtrl->SetItem(&lvIt);
	lvIt.pszText=pInfo->szFileByte;
	lvIt.iSubItem=3;
	pListCtrl->SetItem(&lvIt);

	CPublicData::IsFree=TRUE;

}


BOOL CFileListClass::TransformLine(CString strLine,FILE_FTP_INFO &serFileInfo)
{
	FILE_FTP_INFO2 temp;
	if(strLine.IsEmpty())
		return FALSE;

	TCHAR ch = strLine.GetAt(0);
	if(ch == L'd' || ch == L'D')
	{
		temp.m_bDirectory = TRUE;
	}
	else if(ch == L'-')
	{
		temp.m_bDirectory = FALSE;
	}
	else
	{
		if(strLine.Find(L"<DIR>") != -1 || strLine.Find(L"<dir>") != -1)
			temp.m_bDirectory = TRUE;
		else
			temp.m_bDirectory = FALSE;
		if(strLine.GetLength() < 40)
			return FALSE;
		temp.m_strName = strLine.Mid(39);
		temp.m_strDate = strLine.Left(18);
		temp.m_strName.TrimLeft();
		temp.m_strName.TrimRight();
		//
		goto end;
	}
	temp.m_strSec = strLine.Mid(0, 10);
	int ndx,ndx1,nMouth=0;
	// find mouth
	if((ndx1 = strLine.Find(L"Jan"))!=-1)nMouth = 1;
	else if((ndx1 = strLine.Find(L"Feb"))!=-1)nMouth = 2;
	else if((ndx1 = strLine.Find(L"Mar"))!=-1)nMouth = 3;
	else if((ndx1 = strLine.Find(L"Apr"))!=-1)nMouth = 4;
	else if((ndx1 = strLine.Find(L"May"))!=-1)nMouth = 5;
	else if((ndx1 = strLine.Find(L"Jun"))!=-1)nMouth = 6;
	else if((ndx1 = strLine.Find(L"Jul"))!=-1)nMouth = 7;
	else if((ndx1 = strLine.Find(L"Aug"))!=-1)nMouth = 8;
	else if((ndx1 = strLine.Find(L"Sep"))!=-1)nMouth = 9;
	else if((ndx1 = strLine.Find(L"Oct"))!=-1)nMouth = 10;
	else if((ndx1 = strLine.Find(L"Nov"))!=-1)nMouth = 11;
	else if((ndx1 = strLine.Find(L"Dec"))!=-1)nMouth = 12;
	temp.m_strDate.Format(L"%02d-%s",nMouth,strLine.Mid(ndx1+4 , 8));
	//
	ndx = strLine.Find(L':');
	if(ndx == -1){
		temp.m_strName = strLine.Mid(ndx1 + 12);
		temp.m_strName.TrimLeft();
		temp.m_strName.TrimRight();
		temp.m_strByte = strLine.Left(ndx1-1);
		temp.m_strByte=temp.m_strByte.Right(temp.m_strByte.GetLength()-temp.m_strByte.ReverseFind(L' '));

	}
	else
	{
		temp.m_index = ndx;
		temp.m_strName = strLine.Mid(ndx + 3);
		temp.m_strName.TrimLeft();
		temp.m_strName.TrimRight();
		temp.m_strDate.Format(L"%02d-%s",nMouth,strLine.Mid(ndx-5 , 8));
		temp.m_strByte = strLine.Left(ndx-10);
		temp.m_strByte=temp.m_strByte.Right(temp.m_strByte.GetLength()-temp.m_strByte.ReverseFind(' '));
	}
	//
	if(!temp.m_bDirectory)
	{
		ULONGLONG h=CStringToLong(temp.m_strByte);
		temp.m_strSize = ByteConnvertType(h);
	}
	else
	{
		temp.m_strSize = L"";
		temp.m_strByte = L"";
	}
end:
	serFileInfo.nType = temp.m_bDirectory?DIRECTORYICON:FILEICON;
	CStringToTchar(temp.m_strName,serFileInfo.szFileName);
	CStringToTchar(temp.m_strDate,serFileInfo.szFileDate);
	CStringToTchar(temp.m_strSize,serFileInfo.szFileSize);
	CStringToTchar(temp.m_strByte,serFileInfo.szFileByte);
		
	return true;
}

CString CFileListClass::ByteConnvertType(ULONGLONG uByte)
{
	ULONGLONG uType=1024;
	CString   csAnswer;
	if(uByte < uType)
	{
		csAnswer.Format(L"%4llu   B",uByte);
	}
	else if(uByte < uType*uType)
	{
		csAnswer.Format(L"%4llu  KB",uByte/1024);
	}
	else if(uByte < uType*uType*uType)
	{
		csAnswer.Format(L"%4llu  MB",uByte/1024/1024);
	}
	else if(uByte < uType*uType*uType*uType)
	{
		csAnswer.Format(L"%4llu  GB",uByte/1024/1024/1024);
	}
	//csAnswer.Format(L"%s (%9llu)",csAnswer,uByte);
	return csAnswer;
}

