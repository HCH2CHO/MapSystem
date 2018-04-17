
// spatial analysisView.h : CspatialanalysisView ��Ľӿ�
//
#include  <string>
#include  <iostream>
#include <fstream>
#include<vector>
#include <math.h>
#include "Tin.h"
#include "spatial analysisDoc.h"
#pragma once
using namespace std;
class CspatialanalysisView : public CView
{
protected: // �������л�����
	CspatialanalysisView();
	DECLARE_DYNCREATE(CspatialanalysisView)

// ����
public:
	CspatialanalysisDoc* GetDocument() const;

	DOUBLERECT       m_rcRealMap;
// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	
// ʵ��
public:
	virtual ~CspatialanalysisView();

	void GetCoorArray(DOUBLEPOINTARRAY& doublePointArray,POINTARRAY& pointArray,BOOL bFlag = TRUE); 

	CTin m_tin;
	double           m_dResolution;//���ݷֱ���

	struct coord
	{
		double x;
		double y;
		bool record;
	};
	void Douglas(CDC* pDC);
	double Distance(coord *p1,coord *p2,coord *target);
	void Selectpoint(int start,int end,double thre);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

	BOOL init;
	BOOL m_YNLoaded;
	float m_scale;
	int m_WindowOrgX,m_WindowOrgY;
	bool m_bLeftButtonDown;
	bool m_bGlide;
	bool m_fileread;

	bool m_douglas;
	CPoint m_Pointup;
	CPoint m_Pointdown;

	CRect zoomRect; //���Ŵ���
	CPoint pts[3];
	vector<vector<double>> data;

	coord *douglas;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileOpen();

	afx_msg void OnDouglas();
	afx_msg void OnReadTin();
};

#ifndef _DEBUG  // spatial analysisView.cpp �еĵ��԰汾
inline CspatialanalysisDoc* CspatialanalysisView::GetDocument() const
   { return reinterpret_cast<CspatialanalysisDoc*>(m_pDocument); }
#endif

