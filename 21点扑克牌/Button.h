#pragma once
#include"protocol.h"
enum BTNTEXTSTYLE {
	BTN_TEXT_STYLE_LEFT,//���ֿ���
	BTN_TEXT_STYLE_MIDDLE,//���־���
};
class Button
{
public:
	Button();
	Button(int x,int y, int width,int height);
	int beginX;//��ť��ʼX
	int beginY;//��ť��ʼY
	int endX;//��ť��ֹX
	int endY;//��ť��ֹY
	//���ư�ť
	void draw(LPCTSTR text,enum BTNTEXTSTYLE type= BTN_TEXT_STYLE_MIDDLE);
};

