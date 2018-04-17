#pragma once
class  CspatialanalysisView;
class CTin
{
public:
	void FindTris();//找到离散点周围的三角形编号
	CTin();
	CTin(CspatialanalysisView* pView);
	virtual ~CTin();
	
	BOOL Read(char* szFile);

	void DrawTin(CDC* pDC);

	DOUBLERECT m_rcTin;	

	void SetView(CspatialanalysisView* pView);

	BOOL Cal_Centre(POINT &po,int n1,int n2,int n3);//计算外接圆圆心

	void getnextPoint(int pcenter,int &pfirst,int &pnext,int ppp[3]);

	void Findpoint(int num);

	void Voronoi();

	void PointRange();
private:
	int maxx,minx,maxy,miny;

	CspatialanalysisView* m_pView;
	

	DOUBLEPOINTARRAY m_triPointArray2d; //存储坐标点xy
	POINTARRAY m_triPointArray2d_draw; //坐标转换成整形
	DOUBLEPOINT3DARRAY m_triPointArray;//存储坐标点xyz，不用
	TRIARRAY m_triArray; //用于绘制的三角形三个点点号

	INTARRAY* m_pNum;//存储离散点周围三角形编号

	POINTARRAY *Thiessen;//存储泰森多边形的顶点，即三角形圆心
	BOOL calThiessen;
};

