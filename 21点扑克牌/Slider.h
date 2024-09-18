#pragma once
#include"protocol.h"
class Slider
{
public:    
	Slider();
	//��1����ʼλ��x����2����ʼλ��y����3��������ȣ���4�������߶ȣ���5��6��������ʾ���ݷ�Χ����7��ÿ���ƶ���ƫ����
	Slider(int x, int y, int w, int h, double maxNum, double minNum, double perOffsetNum, COLORREF sliderFillColor=WHITE, COLORREF sliderEdgeColor=BLACK, COLORREF sliderBlockFillColor_normal=RGB(220,220,220), COLORREF sliderBlockFillColor_press = RED,COLORREF sliderBlockEdgeColor=BLACK);
	//����
	void draw();
	
	//�����¼�
	void inputEvent(ExMessage *msg);

	//��ȡ��ǰ��
	double getCurNum();

	//���õ�ǰƫ���������밴ť��������
	void setCurOffsetNum(double curNum);



private:
	double m_curOffsetNum;//��ǰ��

	int m_maxNum;//�����

	int m_minNum;//��С��

	int m_perOffsetNum;//�����ƶ�һ���ƫ����

	int m_beginX;//������ʼx

	int m_beginY;//������ʼy

	int m_endX;//������ֹx

	int m_endY;//������ֹy

	int m_sliderBlockX;//����λ��
	int m_sliderBlockW;//������

	COLORREF m_sliderFillColor;//���������ɫ
	COLORREF m_sliderBlockFillColor_normal;//����ͨ������������ɫ
	COLORREF m_sliderBlockFillColor_press;//���鱻��ѹʱ������ɫ
	COLORREF m_sliderBlockEdgeColor;//����߿���ɫ
	COLORREF m_sliderEdgeColor;//�����߿���ɫ
	COLORREF m_sliderBlockFillColor_show;//������ʾ����ɫ

	bool m_isDraging;//�Ƿ����϶�״̬
};


