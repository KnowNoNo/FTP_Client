
// HH_FTPClientDoc.cpp : CHH_FTPClientDoc ���ʵ��
//

#include "stdafx.h"
#include "HH_FTPClient.h"

#include "HH_FTPClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHH_FTPClientDoc

IMPLEMENT_DYNCREATE(CHH_FTPClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CHH_FTPClientDoc, CDocument)
END_MESSAGE_MAP()


// CHH_FTPClientDoc ����/����

CHH_FTPClientDoc::CHH_FTPClientDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CHH_FTPClientDoc::~CHH_FTPClientDoc()
{
}

BOOL CHH_FTPClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CHH_FTPClientDoc ���л�

void CHH_FTPClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CHH_FTPClientDoc ���

#ifdef _DEBUG
void CHH_FTPClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHH_FTPClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHH_FTPClientDoc ����

BOOL CHH_FTPClientDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	// TODO: �ڴ����ר�ô����/����û���
	return TRUE;
	//return CDocument::CanCloseFrame(pFrame);
}
