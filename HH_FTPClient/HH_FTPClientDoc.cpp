
// HH_FTPClientDoc.cpp : CHH_FTPClientDoc 类的实现
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


// CHH_FTPClientDoc 构造/析构

CHH_FTPClientDoc::CHH_FTPClientDoc()
{
	// TODO: 在此添加一次性构造代码

}

CHH_FTPClientDoc::~CHH_FTPClientDoc()
{
}

BOOL CHH_FTPClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CHH_FTPClientDoc 序列化

void CHH_FTPClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CHH_FTPClientDoc 诊断

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


// CHH_FTPClientDoc 命令

BOOL CHH_FTPClientDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	// TODO: 在此添加专用代码和/或调用基类
	return TRUE;
	//return CDocument::CanCloseFrame(pFrame);
}
