#include "Slider.h"
#include<iostream>
Slider::Slider()
{
}

Slider::Slider(int x, int y, int w, int h, double maxNum, double minNum, double perOffsetNum, COLORREF sliderFillColor, COLORREF sliderEdgeColor, COLORREF sliderBlockFillColor_normal, COLORREF sliderBlockFillColor_press, COLORREF sliderBlockEdgeColor)
{
	//初始化位置
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
	//绘制滑条
	setfillcolor(m_sliderFillColor);
	setlinecolor(m_sliderEdgeColor);
	fillrectangle(m_beginX, m_beginY, m_endX, m_endY);

	//绘制滑块
	setfillcolor(m_sliderBlockFillColor_show);
	setlinecolor(m_sliderBlockEdgeColor);
	fillrectangle(m_sliderBlockX, m_beginY-(m_endY-m_beginY)/2, m_sliderBlockX+m_sliderBlockW, m_endY+(m_endY-m_beginY)/2);

}

void Slider::inputEvent(ExMessage* msg)
{
	
		if (msg->message == WM_LBUTTONDOWN) {
			if (msg->x >= m_sliderBlockX && msg->x <= m_sliderBlockX + m_sliderBlockW &&
				msg->y >= m_beginY + (m_endY - m_beginY) / 2 && msg->y <= m_endY + (m_endY - m_beginY) / 2) {

				m_isDraging = true;//鼠标按下滑块后切换为正在拖拽状态
				m_sliderBlockFillColor_show = m_sliderBlockFillColor_press;//鼠标按下滑块后滑块显示颜色设置为按压时的填充颜色
				setcapture();//设置监听窗口外的事件
				
			}
		}
		else if (msg->message == WM_LBUTTONUP) {

			m_isDraging = false;
			releasecapture();//释放监听窗口外的事件
			m_sliderBlockFillColor_show = m_sliderBlockFillColor_normal;
		}
		else if (msg->message == WM_MOUSEMOVE) {
			if (m_isDraging) {

				if (msg->x > m_sliderBlockX + m_sliderBlockW) {
					m_sliderBlockX= msg->x;
					if (m_sliderBlockX > m_endX-m_sliderBlockW) {//超过滑条范围
						m_sliderBlockX = m_endX - m_sliderBlockW;
					}
				}
				else if(msg->x<m_sliderBlockX){
					m_sliderBlockX= msg->x;
					if (m_sliderBlockX < m_beginX) {//超出滑条范围
						m_sliderBlockX = m_beginX;
					}
				}

			}
			//刷新当前偏移量
			m_curOffsetNum= (double)(m_sliderBlockX - m_beginX) / (double)(m_endX - m_beginX - m_sliderBlockW) * (m_maxNum - m_minNum) + m_minNum;
		
		}
		
	
}

//获取当前偏移量
double Slider::getCurNum()
{
	return m_curOffsetNum;
}

//设置当前偏移量
void Slider::setCurOffsetNum(double curNum)
{
	//更新滑块位置
	m_sliderBlockX += m_perOffsetNum*curNum;
	
	
	//将计算当前偏移量的公式倒过来
	//m_sliderBlockX = (double)(curNum - m_minNum) / (double)(m_maxNum - m_minNum) * (double)(m_endX - m_beginX - m_sliderBlockW)+ (double)m_beginX;
	
	//滑块超出范围时
	if (m_sliderBlockX < m_beginX)m_sliderBlockX = m_beginX;
	if (m_sliderBlockX > m_endX - m_sliderBlockW)m_sliderBlockX = m_endX - m_sliderBlockW;

	//更新当前偏移量
	m_curOffsetNum = (double)(m_sliderBlockX - m_beginX) / (double)(m_endX - m_beginX - m_sliderBlockW) * (m_maxNum - m_minNum) + m_minNum;

}


