#pragma once
#include"protocol.h"
#include"OptionBtn.h"
#include<vector>
//信息盒子的不同类型-主要体现在按钮上
enum MyMessageBoxType {
	MMBT_OK,
	MMBT_TRY,
};
int myMessageBox(int x, int y, const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType = MMBT_OK);
int myMessageBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType = MMBT_OK);

class MyMessageBox
{
public:
	MyMessageBox(int x, int y, const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType = MMBT_OK);
	MyMessageBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType = MMBT_OK);
	~MyMessageBox();
	int play();
protected:
	void draw();
	void inputEvent();
	
	int beginX;
	int beginY;
	int endX;
	int endY;
	wchar_t* title;//信息盒子标题
	wchar_t* content;//主要内容
	std::vector<OptionBtn>buttons;//存储按钮
	std::vector<int>contentSplitPoint;//存储不同行的在content成员变量中的起始位置的偏移量
	bool isQuit;//是否关闭窗口
	int clickedBtnType;//被点击的按钮类型，对应于MyMessageBoxType
	IMAGE bgImg;
};

