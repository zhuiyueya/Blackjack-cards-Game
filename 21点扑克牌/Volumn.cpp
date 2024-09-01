#include "Volumn.h"
#include<iostream>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define VOLUMN_ADD_BTN_SIZE 20//增加音量加号按钮大小
#define VOLUMN_SUB_BTN_SIZE 20//减少音量加号按钮大小

#define VOLUMN_SlIDER_H 10//音量滑条是高度
#define VOLUMN_BTN_SLIDER_GAP 10//音量按钮和滑条之间是间隙
#define MAX_VOLUMN 100//最大音量
#define MIN_VOLUMN 0//最小音量
#define VOLUMN_SLIDER_PER_OFFSET_NUM 3//增加/减少音量时像素点偏移量
#define VOLUMN_SLIDER_W (MAX_VOLUMN-MIN_VOLUMN+1)*VOLUMN_SLIDER_PER_OFFSET_NUM //音量滑条的宽度


Volumn::Volumn(int *volumn)
{
	m_volumn = *volumn;
	int totalW = VOLUMN_ADD_BTN_SIZE + VOLUMN_SUB_BTN_SIZE + VOLUMN_SLIDER_W + VOLUMN_BTN_SLIDER_GAP;
	int beginX = BTN_SIZE_MAIN_OPTION_W + (WIDTH - BTN_SIZE_MAIN_OPTION_W - totalW)/2;
	int beginY = (HEIGHT - VOLUMN_ADD_BTN_SIZE) / 2;
	m_volumnSubBtn = Button(beginX, beginY, VOLUMN_SUB_BTN_SIZE, VOLUMN_SUB_BTN_SIZE);
	beginX += VOLUMN_BTN_SLIDER_GAP+ VOLUMN_SUB_BTN_SIZE;
	m_volumnSlider = Slider(beginX, (HEIGHT - VOLUMN_SlIDER_H) / 2, VOLUMN_SLIDER_W, VOLUMN_SlIDER_H, MAX_VOLUMN, MIN_VOLUMN, VOLUMN_SLIDER_PER_OFFSET_NUM);
	beginX += VOLUMN_BTN_SLIDER_GAP + VOLUMN_SLIDER_W;
	m_volumnAddBtn = Button(beginX, beginY, VOLUMN_ADD_BTN_SIZE, VOLUMN_ADD_BTN_SIZE);
	
}

Volumn::Volumn()
{
	m_volumn = MIN_VOLUMN;
	int totalW = VOLUMN_ADD_BTN_SIZE + VOLUMN_SUB_BTN_SIZE + VOLUMN_SLIDER_W + VOLUMN_BTN_SLIDER_GAP;
	int beginX = BTN_SIZE_MAIN_OPTION_W + (WIDTH - BTN_SIZE_MAIN_OPTION_W - totalW) / 2;
	int beginY = (HEIGHT - VOLUMN_ADD_BTN_SIZE) / 2;
	m_volumnSubBtn = Button(beginX, beginY, VOLUMN_SUB_BTN_SIZE, VOLUMN_SUB_BTN_SIZE);
	beginX += VOLUMN_BTN_SLIDER_GAP + VOLUMN_SUB_BTN_SIZE;
	m_volumnSlider = Slider(beginX, (HEIGHT - VOLUMN_SlIDER_H) / 2, VOLUMN_SLIDER_W, VOLUMN_SlIDER_H, MAX_VOLUMN, MIN_VOLUMN, VOLUMN_SLIDER_PER_OFFSET_NUM);
	beginX += VOLUMN_BTN_SLIDER_GAP + VOLUMN_SLIDER_W;
	m_volumnAddBtn = Button(beginX, beginY, VOLUMN_ADD_BTN_SIZE, VOLUMN_ADD_BTN_SIZE);
}

void Volumn::inputEvent(ExMessage* msg)
{
	int tmpVolumn = m_volumn;
	if (msg->message == WM_LBUTTONDOWN) {
		if (msg->x > m_volumnSubBtn.beginX && msg->x<m_volumnSubBtn.endX && msg->y>m_volumnSubBtn.beginY && msg->y < m_volumnSubBtn.endY) {
			m_volumnSlider.setCurOffsetNum(-1);
		}
		else if (msg->x > m_volumnAddBtn.beginX && msg->x<m_volumnAddBtn.endX && msg->y>m_volumnAddBtn.beginY && msg->y < m_volumnAddBtn.endY) {
			m_volumnSlider.setCurOffsetNum(1);
		}
		
	}
	m_volumnSlider.inputEvent(msg);
	m_volumn = m_volumnSlider.getCurOffsetNum();
	if(m_volumn!=tmpVolumn)
	setVolumn();
	//std::cout << m_volumn << std::endl;
	
}

void Volumn::draw()
{
	m_volumnAddBtn.draw(_T("+"));
	m_volumnSubBtn.draw(_T("-"));
	m_volumnSlider.draw();
}

Volumn& Volumn::getInstance()
{
	// TODO: 在此处插入 return 语句
	static Volumn instance;
	return instance;
}

void Volumn::setVolumn()
{
	// 查询当前音量
	/*wchar_t a[100];
	mciSendString(L"status mymusic volume", a, 100, 0);*/
	wchar_t cc[256];
	//int currentVolume = _wtoi(a);
	//std::cout << currentVolume << std::endl;

	// 设置新的音量值（例如设置为 50）1~1000
	int newVolume = m_volumn*10;
	swprintf(cc, L"setaudio mymusic volume to %d", newVolume);
	mciSendString(cc, 0, 0, 0);
}
