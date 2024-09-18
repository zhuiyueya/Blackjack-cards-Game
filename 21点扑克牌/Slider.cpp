#include "Slider.h"
#include<iostream>
Slider::Slider()
{
}

Slider::Slider(int x, int y, int w, int h, double maxNum, double minNum, double perOffsetNum, COLORREF sliderFillColor, COLORREF sliderEdgeColor, COLORREF sliderBlockFillColor_normal, COLORREF sliderBlockFillColor_press, COLORREF sliderBlockEdgeColor)
{
	//��ʼ��λ��
	m_beginX = x;
	m_beginY = y;
	m_endX = x + w;
	m_endY = y + h;

	
	m_maxNum = maxNum;
	m_minNum = minNum;
	
	m_perOffsetNum = perOffsetNum;
	m_curOffsetNum = minNum;

	m_sliderBlockX = m_beginX;
	m_sliderBlockW = w / (maxNum - minNum+1);

	m_sliderFillColor = sliderFillColor;
	m_sliderEdgeColor = sliderEdgeColor;
	m_sliderBlockFillColor_normal = sliderBlockFillColor_normal;
	m_sliderBlockFillColor_press = sliderBlockFillColor_press;
	m_sliderBlockEdgeColor = sliderBlockEdgeColor;
	m_sliderBlockFillColor_show = m_sliderBlockFillColor_normal;
	
	m_isDraging = false;
	
}

void Slider::draw()
{
	//���ƻ���
	setfillcolor(m_sliderFillColor);
	setlinecolor(m_sliderEdgeColor);
	fillrectangle(m_beginX, m_beginY, m_endX, m_endY);

	//���ƻ���
	setfillcolor(m_sliderBlockFillColor_show);
	setlinecolor(m_sliderBlockEdgeColor);
	fillrectangle(m_sliderBlockX, m_beginY-(m_endY-m_beginY)/2, m_sliderBlockX+m_sliderBlockW, m_endY+(m_endY-m_beginY)/2);

}

void Slider::inputEvent(ExMessage* msg)
{
	
		if (msg->message == WM_LBUTTONDOWN) {
			if (msg->x >= m_sliderBlockX && msg->x <= m_sliderBlockX + m_sliderBlockW &&
				msg->y >= m_beginY + (m_endY - m_beginY) / 2 && msg->y <= m_endY + (m_endY - m_beginY) / 2) {

				m_isDraging = true;//��갴�»�����л�Ϊ������ק״̬
				m_sliderBlockFillColor_show = m_sliderBlockFillColor_press;//��갴�»���󻬿���ʾ��ɫ����Ϊ��ѹʱ�������ɫ
				setcapture();//���ü�����������¼�
				
			}
		}
		else if (msg->message == WM_LBUTTONUP) {

			m_isDraging = false;
			releasecapture();//�ͷż�����������¼�
			m_sliderBlockFillColor_show = m_sliderBlockFillColor_normal;
		}
		else if (msg->message == WM_MOUSEMOVE) {
			if (m_isDraging) {

				if (msg->x > m_sliderBlockX + m_sliderBlockW) {
					m_sliderBlockX= msg->x;
					if (m_sliderBlockX > m_endX-m_sliderBlockW) {//����������Χ
						m_sliderBlockX = m_endX - m_sliderBlockW;
					}
				}
				else if(msg->x<m_sliderBlockX){
					m_sliderBlockX= msg->x;
					if (m_sliderBlockX < m_beginX) {//����������Χ
						m_sliderBlockX = m_beginX;
					}
				}

			}
			//ˢ�µ�ǰƫ����
			m_curOffsetNum= (double)(m_sliderBlockX - m_beginX) / (double)(m_endX - m_beginX - m_sliderBlockW) * (m_maxNum - m_minNum) + m_minNum;
		
		}
		
	
}

//��ȡ��ǰƫ����
double Slider::getCurNum()
{
	return m_curOffsetNum;
}

//���õ�ǰƫ����
void Slider::setCurOffsetNum(double curNum)
{
	//���»���λ��
	m_sliderBlockX += m_perOffsetNum*curNum;
	
	
	//�����㵱ǰƫ�����Ĺ�ʽ������
	//m_sliderBlockX = (double)(curNum - m_minNum) / (double)(m_maxNum - m_minNum) * (double)(m_endX - m_beginX - m_sliderBlockW)+ (double)m_beginX;
	
	//���鳬����Χʱ
	if (m_sliderBlockX < m_beginX)m_sliderBlockX = m_beginX;
	if (m_sliderBlockX > m_endX - m_sliderBlockW)m_sliderBlockX = m_endX - m_sliderBlockW;

	//���µ�ǰƫ����
	m_curOffsetNum = (double)(m_sliderBlockX - m_beginX) / (double)(m_endX - m_beginX - m_sliderBlockW) * (m_maxNum - m_minNum) + m_minNum;

}


