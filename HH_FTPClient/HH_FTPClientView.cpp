
// HH_FTPClientView.cpp : CHH_FTPClientView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHH_FTPClientView ����/����

CHH_FTPClientView::CHH_FTPClientView()
{
	// TODO: �ڴ˴���ӹ������

}

CHH_FTPClientView::~CHH_FTPClientView()
{
}

BOOL CHH_FTPClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CHH_FTPClientView ����

void CHH_FTPClientView::OnDraw(CDC* /*pDC*/)
{
	CHH_FTPClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CHH_FTPClientView ��ӡ

BOOL CHH_FTPClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CHH_FTPClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CHH_FTPClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CHH_FTPClientView ���

#ifdef _DEBUG
void CHH_FTPClientView::AssertValid() const
{
	CView::AssertValid();
}

void CHH_FTPClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHH_FTPClientDoc* CHH_FTPClientView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHH_FTPClientDoc)));
	return (CHH_FTPClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CHH_FTPClientView ��Ϣ�������
