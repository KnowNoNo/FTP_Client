
// HH_FTPClientDoc.h : CHH_FTPClientDoc ��Ľӿ�
//


#pragma once


class CHH_FTPClientDoc : public CDocument
{
protected: // �������л�����
	CHH_FTPClientDoc();
	DECLARE_DYNCREATE(CHH_FTPClientDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CHH_FTPClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
};


