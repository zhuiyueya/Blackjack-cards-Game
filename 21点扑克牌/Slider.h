#pragma once
#include"protocol.h"
class Slider
{
public:    
	Slider();
	//参1：起始位置x，参2：起始位置y，参3：滑条宽度，参4：滑条高度，参5、6：滑条表示数据范围，参7：每次移动的偏移量
	Slider(int x, int y, int w, int h, double maxNum, double minNum, double perOffsetNum, COLORREF sliderFillColor=WHITE, COLORREF sliderEdgeColor=BLACK, COLORREF sliderBlockFillColor_normal=RGB(220,220,220), COLORREF sliderBlockFillColor_press = RED,COLORREF sliderBlockEdgeColor=BLACK);
	//绘制
	void draw();
	
	//输入事件
	void inputEvent(ExMessage *msg);

	//获取当前量
	double getCurNum();

	//设置当前偏移量，可与按钮进行联动
	void setCurOffsetNum(double curNum);



private:
	double m_curOffsetNum;//当前量

	int m_maxNum;//最大量

	int m_minNum;//最小量

	int m_perOffsetNum;//滑块移动一格的偏移量

	int m_beginX;//滑块起始x

	int m_beginY;//滑块起始y

	int m_endX;//滑块终止x

	int m_endY;//滑块终止y

	int m_sliderBlockX;//滑块位置
	int m_sliderBlockW;//滑块宽度

	COLORREF m_sliderFillColor;//滑条填充颜色
	COLORREF m_sliderBlockFillColor_normal;//滑块通常情况下填充颜色
	COLORREF m_sliderBlockFillColor_press;//滑块被按压时填充的颜色
	COLORREF m_sliderBlockEdgeColor;//滑块边框颜色
	COLORREF m_sliderEdgeColor;//滑条边框颜色
	COLORREF m_sliderBlockFillColor_show;//滑块显示的颜色

	bool m_isDraging;//是否处于拖动状态
};


