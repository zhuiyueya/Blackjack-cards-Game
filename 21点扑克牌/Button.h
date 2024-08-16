#pragma once
#include"protocol.h"
enum BTNTEXTSTYLE {
	BTN_TEXT_STYLE_LEFT,//文字靠左
	BTN_TEXT_STYLE_MIDDLE,//文字居中
};
class Button
{
public:
	Button();
	Button(int x,int y, int width,int height);
	int beginX;//按钮起始X
	int beginY;//按钮起始Y
	int endX;//按钮终止X
	int endY;//按钮终止Y
	//绘制按钮
	void draw(LPCTSTR text,enum BTNTEXTSTYLE type= BTN_TEXT_STYLE_MIDDLE);
};

