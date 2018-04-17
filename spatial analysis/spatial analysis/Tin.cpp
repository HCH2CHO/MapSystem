#include "stdafx.h"
#include "Tin.h"
#include "spatial analysisView.h"
#include "spatial analysis.h"

CTin::CTin()
{
	m_pView = NULL;
	m_pNum = NULL;
	Thiessen=NULL;
	calThiessen=FALSE;
}

CTin::~CTin()
{
	if(m_pNum)
	{
		delete[] m_pNum;
		m_pNum = NULL;
	}

}

CTin::CTin(CspatialanalysisView* pView)
{
	SetView(pView);
}

void CTin::SetView(CspatialanalysisView* pView)
{
	m_pView = pView;
}

BOOL CTin::Read(char* szFile)
{	
	m_triPointArray.RemoveAll();
	m_triArray.RemoveAll();

	int nPoint=0;
	int nTri;

	int i;
	FILE* f = fopen(szFile,"rb");
	if(!f)
	{
		AfxMessageBox(_T("打开文件错误！"));
		return FALSE;
	}

	VERINFO verinfo;
	fread(&verinfo,sizeof(VERINFO),1,f);

	double z1,z2;//x,y,z的最大最小值
	
	fread(&m_rcTin.left,sizeof(double),1,f);
	fread(&m_rcTin.right,sizeof(double),1,f);
	fread(&m_rcTin.top,sizeof(double),1,f);
	fread(&m_rcTin.bottom,sizeof(double),1,f);

	fread(&z1,sizeof(double),1,f);
	fread(&z2,sizeof(double),1,f);

	fread(&nPoint,sizeof(int),1,f);

	for(i=0;i<nPoint;i++)
	{		
		DOUBLEPOINT3D pt3d;		
		fread(&pt3d,sizeof(DOUBLEPOINT3D),1,f);		
		m_triPointArray.Add(pt3d);

		DOUBLEPOINT p2d;
		p2d.x = pt3d.x;
		p2d.y = pt3d.y;
		m_triPointArray2d.Add(p2d);
	}	

	int nEdge;
	fread(&nEdge,sizeof(int),1,f);

	/*DCT_EDGE2* pEdge = NULL;
	pEdge = new DCT_EDGE2[nEdge];
	fread(pEdge,sizeof(DCT_EDGE2),nEdge,f);

	if(pEdge)
	{
		delete[] pEdge;
		pEdge = NULL;
	}*/

	for(i=0;i<nEdge;i++)
	{
		DCT_EDGE2 edge;
		fread(&edge,sizeof(DCT_EDGE2),1,f);
	}	

	fread(&nTri,sizeof(int),1,f);
// 	m_triArray.SetSize(nTri);
// 	fread(m_triArray.GetData(),sizeof(dct_tri),nTri,f);

	for(i=0;i<nTri;i++)
	{
		dct_tri tri;		
		fread(&tri,sizeof(dct_tri),1,f);
		m_triArray.Add(tri);
	}

	fclose(f);

	memmove(&m_pView->m_rcRealMap,&m_rcTin,sizeof(DOUBLERECT));	

	m_pView->GetCoorArray(m_triPointArray2d,m_triPointArray2d_draw);

	return TRUE;
}

void CTin::DrawTin(CDC* pDC)
{
	int i;
	int nTri = m_triArray.GetSize();
	for (i=0;i<nTri;i++)
	{
		dct_tri tri = m_triArray.GetAt(i);
		POINT p1 = m_triPointArray2d_draw[tri.p1];
		POINT p2 = m_triPointArray2d_draw[tri.p2];
		POINT p3 = m_triPointArray2d_draw[tri.p3];

		pDC->MoveTo(p1);
		pDC->LineTo(p2);
		pDC->LineTo(p3);
		pDC->LineTo(p1);
	}

	if(calThiessen==TRUE)
	{
		int nThi=m_triPointArray2d_draw.GetSize();
		CPen pen;
		pen.CreatePen(PS_SOLID,0,RGB(255,0,0));
		pDC->SelectObject(&pen);

		for (i=0;i<nThi;i++)
		{
			if(Thiessen[i].IsEmpty()==0)
			{
				int nEdge=Thiessen[i].GetSize();
				if (nEdge!=0)
				{
					for(int j=0;j<nEdge-1;j++)
					{
						pDC->MoveTo(Thiessen[i][j]);
						pDC->LineTo(Thiessen[i][j+1]);
					}
					pDC->MoveTo(Thiessen[i][nEdge-1]);
					pDC->LineTo(Thiessen[i][0]);
				}
			}
		}
	}
}

void CTin::FindTris()
{
	if(m_pNum)
	{
		delete[] m_pNum;
		m_pNum = NULL;
	}
	int nPoint = m_triPointArray2d_draw.GetSize();
	m_pNum = new INTARRAY[nPoint];

	int nTri = m_triArray.GetSize();
	int i;
	for (i=0;i<nTri;i++)
	{
		dct_tri tri = m_triArray[i];
		m_pNum[tri.p1].Add(i);
		m_pNum[tri.p2].Add(i);
		m_pNum[tri.p3].Add(i);
	}
}


void CTin::PointRange()
{
	int midx = (m_rcTin.left+m_rcTin.right)/2.;
	int midy = (m_rcTin.top+m_rcTin.bottom)/2.;
	maxx=m_rcTin.right-midx;
	minx=m_rcTin.left-midx;
	maxy=m_rcTin.bottom-midy;
	miny=m_rcTin.top-midy;
}

//计算外接圆圆心
//由于坐标转换可能存在精度损失！部分坐标点位置相同
BOOL CTin::Cal_Centre(POINT &po,int n1,int n2,int n3) 
{ 
    double x1,x2,x3,y1,y2,y3; 
    x1=m_triPointArray2d_draw[n1].x/1000.; 
    y1=m_triPointArray2d_draw[n1].y/1000.; 
    x2=m_triPointArray2d_draw[n2].x/1000.; 
    y2=m_triPointArray2d_draw[n2].y/1000.; 
    x3=m_triPointArray2d_draw[n3].x/1000.; 
    y3=m_triPointArray2d_draw[n3].y/1000.; 
	double a,b;
	a=(x3-x1)*(y2-y1)-(x2-x1)*(y3-y1);
	b=(y3-y1)*(x2-x1)-(y2-y1)*(x3-x1);
	if( (a==0) || (b==0) )
		return FALSE;


    double x_centre=((y2-y1)*(y3*y3-y1*y1+x3*x3-x1*x1)-(y3-y1)*(y2*y2-y1*y1+x2*x2-x1*x1))/(2*(x3-x1)*(y2-y1)-2*((x2-x1)*(y3-y1)));//计算外接圆圆心的x坐标 
    double y_centre=((x2-x1)*(x3*x3-x1*x1+y3*y3-y1*y1)-(x3-x1)*(x2*x2-x1*x1+y2*y2-y1*y1))/(2*(y3-y1)*(x2-x1)-2*((y2-y1)*(x3-x1)));//计算外接圆圆心的y坐标 
    //radius= sqrt((x1 - x_centre)*(x1 - x_centre) + (y1 - y_centre)*(y1 - y_centre));//计算外接圆的半径 

	if(x_centre>maxx||x_centre<minx||y_centre>maxy||y_centre<miny)
		return FALSE;

	po.x=int(1000*x_centre);po.y=int(1000*y_centre);
	return TRUE;
}

//getnextPoint和Findpoint有问题
void CTin::getnextPoint(int pcenter,int &pfirst,int &pnext,int ppp[3])
{
	for(int k=0;k<3;k++)
	{
		if(pcenter==ppp[k])//中心点
		{
			ppp[k]=-1;
			break;
		}
	}

	if(pfirst==-1)
	{
		for(int k=0;k<3;k++)
		{
			if(ppp[k]!=-1)
			{
				pfirst=ppp[k];
				ppp[k]=-1;
				break;
			}
		}
	}

	if(pnext!=-1)
	{
		for(int k=0;k<3;k++)
		{
			if(pnext==ppp[k])//上一点
			{
				ppp[k]=-1;
				break;
			}
		}
	 }

	for(int k=0;k<3;k++)
	{
		if(ppp[k]!=-1)
		{
			pnext=ppp[k];
			break;
		}
	}
}

void CTin::Findpoint(int num)
{
	//num为中心点 点号
	//POINT pCenter=m_triPointArray2d_draw[num];
	int firstp=-1;
	int nextp=-1;
	int triNum=m_pNum[num].GetSize();//三角形数量
	
	dct_tri tri = m_triArray[m_pNum[num][0]];
	int plist[3]={tri.p1,tri.p2,tri.p3};

	int nexttri=0;
	int currenttri=0;

	bool test0;

	for (int j=0;j<triNum;j++)
	{
		POINT point;
		test0=Cal_Centre(point,m_triArray[m_pNum[num][ nexttri]].p1,m_triArray[m_pNum[num][ nexttri]].p2,m_triArray[m_pNum[num][ nexttri]].p3);

		if(test0)
		{
			Thiessen[num].Add(point);
		}
		//plist应用形参
		getnextPoint(num,firstp,nextp,plist);
		currenttri=nexttri;

		//检索下一个三角形,如何检索不重复
		for (int i=1;i<triNum;i++)
		{
			if(i!=currenttri)
			{
				if(m_triArray[m_pNum[num][i]].p1==nextp)
				{
					nexttri=i;				
					break;
				}
				if(m_triArray[m_pNum[num][i]].p2==nextp)
				{
					nexttri=i;
					break;
				}
				if(m_triArray[m_pNum[num][i]].p3==nextp)
				{
					nexttri=i;
					break;
				}
			}
		}
		plist[0]=m_triArray[m_pNum[num][nexttri]].p1;
		plist[1]=m_triArray[m_pNum[num][nexttri]].p2;
		plist[2]=m_triArray[m_pNum[num][nexttri]].p3;
	}
}

void CTin::Voronoi()
{
	if(Thiessen)
	{
		delete[] Thiessen;
		Thiessen = NULL;
	}
	int nPoint = m_triPointArray2d_draw.GetSize();
	Thiessen = new POINTARRAY[nPoint];
	PointRange();//获取泰森多边形坐标范围
	FindTris();
	for(int i=0;i< nPoint;i++)
	{
		Findpoint(i);		
	}

	calThiessen=TRUE;

}