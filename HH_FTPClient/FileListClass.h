#pragma once
#include "PublicFunction.h"
#include <map>
using namespace std;

#define		LOCAL_DEFAULT_DIRECTORYL	L"C:\\"
typedef struct		// �ļ���Ϣ
{
	TCHAR szFileName[MAX_PATH];
	TCHAR szFileDate[MAX_PATH];
	TCHAR szFileSize[30];
	TCHAR szFileByte[30];
	int nType;
}FILE_FTP_INFO;

typedef struct		//
{
	BOOL	m_bDirectory;
	WORD	m_bSec;
	//CString length;
	CString m_strSec;
	CString m_strName;
	CString m_strDate;
	CString m_strSize;
	CString m_strByte;
	int m_index;
}FILE_FTP_INFO2;

typedef struct
{
	CString		csTPFileName;						// �����ļ���   Ҳ��MAP_ALL_UPLOADFILE_INFO map�ṹ���е�KEYֵ
	ULONGLONG	ullTPFileByte;						// �����ļ���С

	CString		csLocFileDir;
	CString		csServFileDir;
	CString		csServIp;
	int			nServPort;
	CString		csServUser;
	CString		csServPassword;
	bool		bIsPort;
	bool		bIsForce;
	UINT        uIsTpOk;
	bool		bTPMode;							// 0 Ϊ�ϴ�  1 Ϊ����
	CWinThread* pUploadThread;						// �����߳�
	HANDLE		hEvtEndModule;						// ģ������¼�
	ULONGLONG   ullTPDestnFileSize ;				// ����Ŀ�ĵ�����ͬ���ļ��ֽ�
	volatile ULONGLONG	ullTpFileSize;				// ʵʱ��¼�����ֽ���
	volatile ULONGLONG  ullTPFileLastSize;			// ��¼ǰһ��Ĵ����ֽ���������ͳ�������ٶ�
}FILE_UPLOAD_INFO;

typedef map<CString,FILE_UPLOAD_INFO>  MAP_ALL_UPLOADFILE_INFO;

typedef struct
{
	MAP_ALL_UPLOADFILE_INFO mpFileInfo;
	int nThreadCount;
}FILE_UPLOAD_COUNT_INFO;


typedef struct  
{
	CString csLocDir;
	CString csUpOrDownLoad;
	CString csServDir;
	CString csFileName;
	CString csFileSize;
	CString csStatus;
	//UINT    uDlgSign;
}FILE_FTP_TP_STATUS;

enum
{
	FILE_TP_JOIN,
	FILE_TP_FAIL,
	FILE_TP_SUCCESS
};

class CFileListClass
{
public:
	CFileListClass(void);
	virtual ~CFileListClass(void);

protected:
	HCURSOR m_hDragCursor;			// ��ҷ�б����ĳ��ʱ��ʾ��ͼ��
	BOOL    m_bDragging;
	CImageList* m_pDragImageList;	// ͼ���б����
	CImageList  m_ctImageList;
	CMenu		m_LocServMenu;
	
public:
	void SetFileColumns(CListCtrl* pListCtrl);
	BOOL SetImageList(CListCtrl* pListCtrl,CComboBoxEx* pCombo);
	void GetFileIcon(CString& fileName,int* iIcon,int* iIconSel=NULL,int nCur = LOCFILE);
	void SetPopMenu(int nIndex);
	void InsertFile(CListCtrl* pListCtrl,FILE_FTP_INFO* pInfo,int lParam,UINT& nDirNum);
	BOOL TransformLine(CString strLine,FILE_FTP_INFO &serFileInfo);
	CString ByteConnvertType(ULONGLONG uByte);
};
