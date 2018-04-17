#pragma once
class  CspatialanalysisView;
class CTin
{
public:
	void FindTris();//�ҵ���ɢ����Χ�������α��
	CTin();
	CTin(CspatialanalysisView* pView);
	virtual ~CTin();
	
	BOOL Read(char* szFile);

	void DrawTin(CDC* pDC);

	DOUBLERECT m_rcTin;	

	void SetView(CspatialanalysisView* pView);

	BOOL Cal_Centre(POINT &po,int n1,int n2,int n3);//�������ԲԲ��

	void getnextPoint(int pcenter,int &pfirst,int &pnext,int ppp[3]);

	void Findpoint(int num);

	void Voronoi();

	void PointRange();
private:
	int maxx,minx,maxy,miny;

	CspatialanalysisView* m_pView;
	

	DOUBLEPOINTARRAY m_triPointArray2d; //�洢�����xy
	POINTARRAY m_triPointArray2d_draw; //����ת��������
	DOUBLEPOINT3DARRAY m_triPointArray;//�洢�����xyz������
	TRIARRAY m_triArray; //���ڻ��Ƶ���������������

	INTARRAY* m_pNum;//�洢��ɢ����Χ�����α��

	POINTARRAY *Thiessen;//�洢̩ɭ����εĶ��㣬��������Բ��
	BOOL calThiessen;
};

