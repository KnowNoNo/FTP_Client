
// HH_FTPClientView.cpp : CHH_FTPClientView 类的实现
//

#include "stdafx.h"
#include "HH_FTPClient.h"

#include "HH_FTPClientDoc.h"
#include "HH_FTPClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHH_FTPClientView

IMPLEMENT_DYNCREATE(CHH_FTPClientView, CView)

BEGIN_MESSAGE_MAP(CHH_FTPClientView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHH_FTPClientView 构造/析构

CHH_FTPClientView::CHH_FTPClientView()
{
	// TODO: 在此处添加构造代码

}

CHH_FTPClientView::~CHH_FTPClientView()
{
}

BOOL CHH_FTPClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHH_FTPClientView 绘制

void CHH_FTPClientView::OnDraw(CDC* /*pDC*/)
{
	CHH_FTPClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CHH_FTPClientView 打印

BOOL CHH_FTPClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHH_FTPClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHH_FTPClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CHH_FTPClientView 诊断

#ifdef _DEBUG
void CHH_FTPClientView::AssertValid() const
{
	CView::AssertValid();
}

void CHH_FTPClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHH_FTPClientDoc* CHH_FTPClientView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHH_FTPClientDoc)));
	return (CHH_FTPClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CHH_FTPClientView 消息处理程序
