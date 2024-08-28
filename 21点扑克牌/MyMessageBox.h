#pragma once
#include"protocol.h"
#include"OptionBtn.h"
#include<vector>
//��Ϣ���ӵĲ�ͬ����-��Ҫ�����ڰ�ť��
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
	wchar_t* title;//��Ϣ���ӱ���
	wchar_t* content;//��Ҫ����
	std::vector<OptionBtn>buttons;//�洢��ť
	std::vector<int>contentSplitPoint;//�洢��ͬ�е���content��Ա�����е���ʼλ�õ�ƫ����
	bool isQuit;//�Ƿ�رմ���
	int clickedBtnType;//������İ�ť���ͣ���Ӧ��MyMessageBoxType
	IMAGE bgImg;
};

