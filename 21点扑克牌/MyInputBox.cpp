#include "MyInputBox.h"
#include <stdexcept>
#include<iostream>
#define INPUTBOX_SIZE_W 200
#define INPUTBOX_SIZE_H 30
#define INPUTBOX_MIDDLE_MARGIN 10
#define BTN_WIDTH 60//��Ϣ���ӵĿ��
#define BTN_HEIGHT 30//��Ϣ���ӵĸ߶�
#define TOP_MARGIN 5//�����հ�
#define MIDDLE_LINE_MARGIN 20//����������֮�������հ׵Ŀ��
#define TEXT_MIDDLE_MARGIN 10//�����в�ͬ�м�ļ��
#define BOTTOM_MARGIN 20//�ײ��հ�
#define LEFT_MARGIN 20//��߿հ�
#define RIGHT_MARGIN 20//�ұ߿հ�
#define BTN_CONTENT_MIDDLE_MARGIN 20//��ť������֮��ļ��

//��һ��������ʾ��Ϣ���ڶ�����:����,��������ť���ͣ����ģ����������û��������ݵ����������壺����������������ÿ�������Ĵ�С
MyInputBox::MyInputBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, wchar_t **label,
	wchar_t** output, int num, std::initializer_list<int> lenList)
{
	//std::cout << "x\n";
	//try{
	//	if (num != lenList.size()) {
	//		
	//		throw std::runtime_error("address error occured");
	//	}
	//}
	//catch (const std::runtime_error& e) {
	//	
	//	std::cerr << "catch exception:" << e.what() << std::endl;
	//	//throw;
	//}
	//std::cout << 1;

	

	setfillcolor(WHITE);

	settextcolor(BLACK);

	setlinecolor(BLACK);

	settextstyle(20, 0, _T("΢���ź�"));

	//������ʾ����
	m_content = new wchar_t[wcslen(pcontent) + 1];
	wcscpy(m_content, pcontent);
	//��������
	m_title = new wchar_t[wcslen(ptitle) + 1];
	wcscpy(m_title, ptitle);

	m_isQuit = false;

	int rfNum = 0;//��¼\n���з��ĸ���
	int maxTextW = 0;//�����в�ͬ��֮��������
	int contentLen = wcslen(pcontent);
	wchar_t* preP = m_content;

	m_contentSplitPoint.push_back(0);
	//�ָ�content��ȷ����ͬ��
	for (int i = 0; i < contentLen; i++) {
		if (m_content[i] == _T('\n')) {
			rfNum++;
			m_content[i] = _T('\0');
			maxTextW = max(maxTextW, textwidth(preP));
			preP = m_content + i + 1;
			m_contentSplitPoint.push_back(i + 1);
		}
	}
	maxTextW = max(maxTextW, textwidth(preP));
	//��û�л��з�����Ϊ����ȼ��������ݵĿ��
	if (rfNum == 0)maxTextW = textwidth(pcontent);

	//�洢�û�������ǩ���������п������
	int maxInputLabelW = 0;
	if (label != NULL) {

		m_inputBoxLabel.resize(num);
		m_label.resize(num);
		for (int i = 0; i < num; i++) {
			//���û��ṩ��ÿ�������ı�ǩ��������
			m_label[i] = new wchar_t[wcslen(label[i]) + 1];
			wcscpy(m_label[i], label[i]);

			maxInputLabelW = max(maxInputLabelW,textwidth(m_label[i]));
		}

	}
	
	//���봰���ܿ�� ��ȡ ��ʾ�������+��ť���+ǰ����֮��հ�  ��  ������� ֮��Ľϴ�ֵ��Ϊ�������봰�ڿ��
	int maxWindowWidth = max(maxTextW , INPUTBOX_SIZE_W+ maxInputLabelW) + LEFT_MARGIN + RIGHT_MARGIN;
	//ʹ�øպû������м�
	m_beginX = (getwidth() - (maxWindowWidth)) / 2;

	//��ʾ�ĵ���height��
	int contentH = textheight(pcontent) * (rfNum + 1) + TEXT_MIDDLE_MARGIN * (rfNum);
	
	//���봰���ܸ߶� �� ����߶�+����ʾ�ĸ߶�+��������+��ʾ�������֮��������+�ײ�����+
	// num�������߶�+num�����������һ��Ϊ��ʾ����������,һ��Ϊ�����͵ײ���ť�ģ�
	int maxWindowHeight = textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN + BOTTOM_MARGIN +
		num * INPUTBOX_SIZE_H + (num+1)*INPUTBOX_MIDDLE_MARGIN + BTN_HEIGHT;
	//ʹ�øպû������м�
	m_beginY = (getheight() - maxWindowHeight)/ 2;

	m_endX = m_beginX + maxWindowWidth;
	//��ʱ���㵽��ʾ�ĵײ�
	m_endY = m_beginY + textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN;


	m_output.resize(num);
	m_inputBox.resize(num);
	m_isInputing.resize(num);

	int i = 0;
	for (int len : lenList) {
		if (i > num) {

		}
		
		//ȷ�������λ��
		m_endY += INPUTBOX_MIDDLE_MARGIN;
		m_inputBox[i] = Button(m_endX - RIGHT_MARGIN- INPUTBOX_SIZE_W, m_endY, INPUTBOX_SIZE_W, INPUTBOX_SIZE_H);

		//ȷ��������ǩλ��
		if (!m_inputBoxLabel.empty())
			m_inputBoxLabel[i] = Button(m_beginX + LEFT_MARGIN, m_endY, maxInputLabelW, INPUTBOX_SIZE_H);

		m_endY += INPUTBOX_SIZE_H;

		m_output[i].first = output[i];
		wcscpy(m_output[i].first, _T(""));

		m_output[i].second = len;//��¼������

		m_isInputing[i] = false;//ȫ����ʼ��Ϊ����������״̬

		i++;
	}
	//������밴ť���
	m_endY += INPUTBOX_MIDDLE_MARGIN;

	m_cursorPos.first = -100;
	m_cursorPos.second = -100;


	switch (messageBoxType) {
	case MMBT_OK:
	{
		int btnNum = 1;
		int btnX = m_beginX + ((m_endX - m_beginX) - (2*btnNum-1) * BTN_WIDTH) / 2;
		OptionBtn btn(btnX, m_endY, BTN_WIDTH, BTN_HEIGHT, MMBT_OK, _T("ȷ��"));
		m_buttons.push_back(btn);
		break;
	}
	case MMBT_TRY:
	{
		int btnNum = 2;
		break;
	}
	}
	

	//�ָ�������ȷ
	m_endY = m_beginY + maxWindowHeight;
}

int MyInputBox::play()
{
	// ����һ����������
	HRGN rgn = CreateRectRgn(m_beginX,m_beginY,m_endX,m_endY);
	// ���þ�����������Ϊ�ü���
	setcliprgn(rgn);
	// ����ʹ�� rgn������ rgn ռ�õ�ϵͳ��Դ
	DeleteObject(rgn);

	
	while (!m_isQuit) {
		//cleardevice();
		draw();
		inputEvent();
		FlushBatchDraw();
		Sleep(60);
	}

	// ȡ��֮ǰ���õĲü���
	setcliprgn(NULL);

	return 0;
}

MyInputBox::~MyInputBox()
{
	//�ͷ��ڴ�
	for (int i = 0; i < m_label.size(); i++) {
		delete m_label[i];
	}
	delete []m_title;
	delete[]m_content;
	
}

void MyInputBox::inputEvent()
{

	ExMessage msg;
	while (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			int mx = msg.x;
			int my = msg.y;
			bool isClickEmptyArea = true;
			//�����ť�ж��Ƿ񱻵��
			for (OptionBtn& btn : m_buttons) {
				if (mx > btn.beginX && mx<btn.endX && my>btn.beginY && my < btn.endY) {
					isClickEmptyArea = false;
					switch (btn.m_label) {
					case MMBT_OK:
					{
						m_clickedBtnType = MMBT_OK;
						m_isQuit = true;
						break;
					}
					}
				}
			}
			//���������ж��Ƿ񱻵��
			int choosedBtn=0;
			for (Button& btn : m_inputBox) {
				if (mx > btn.beginX && mx<btn.endX && my>btn.beginY && my < btn.endY) {
					isClickEmptyArea = false;
					break;
				}
				choosedBtn++;
			}
			//˵���б������������
			if (choosedBtn != m_inputBox.size()) {

				for (int i = 0; i < m_isInputing.size(); i++) {
					if (m_isInputing[i]) {
						m_isInputing[i] = false;
						break;
					}
				}
				m_isInputing[choosedBtn] = true;
				//��������-��|������ӽ�����ʵ�ֹ��
				//for (int i = 0; i < m_inputBox.size(); i++) {
				//	if (m_isInputing[i]) {
				//		//�ܽ���if˵��������|���ţ������鳤������Ϊ1
				//		int contentLen= wcslen(m_output[i].first);
				//		m_output[i].first[contentLen - 1] = _T('\0');
				//		break;
				//	}
				//}
				//
				//if (!m_isInputing[choosedBtn]) {
				//	
				//	int contentLen = wcslen(m_output[choosedBtn].first);
				//	m_output[choosedBtn].first[contentLen] = _T('|');
				//	m_output[choosedBtn].first[contentLen+1] = _T('\0');
				//	std::wcout << m_output[choosedBtn].first;
				//}
				//

				settextstyle(16, 0, _T("΢���ź�"));
				//���¹��λ��-�������ַ�ʱҲ��Ҫ����x����
				m_cursorPos.first = m_inputBox[choosedBtn].beginX+textwidth(m_output[choosedBtn].first)+10;
				
				m_cursorPos.second= m_inputBox[choosedBtn].beginY+ 
					((m_inputBox[choosedBtn].endY - m_inputBox[choosedBtn].beginY) - 16) / 2;
			}

			//������˿հ������������������Ϊ������״̬
			if (isClickEmptyArea) {
				for (int i = 0; i < m_inputBox.size(); i++) {
					if (m_isInputing[i]) {
						m_isInputing[i] = false;
						break;
					}
				}
			}



		}
		else if (msg.message == WM_CHAR) {
			//����Ϊ����(backspace)ʱ
			if (msg.vkcode == VK_BACK) {
				for (int i = 0; i < m_isInputing.size(); i++) {
					if (m_isInputing[i]) {
						int len = wcslen(m_output[i].first);
						if(len>0)
						m_output[i].first[len - 1] = _T('\0');
						//���¹��λ��
						settextstyle(16, 0, _T("΢���ź�"));
						m_cursorPos.first = m_inputBox[i].beginX + textwidth(m_output[i].first) + 10;
						break;
					}
				}
			}//����Ϊ��ͨ�ַ�ʱ
			else {
				
				for (int i = 0; i < m_isInputing.size(); i++) {
					if (m_isInputing[i]) {
						if(wcslen(m_output[i].first)<m_output[i].second-1)
						swprintf(m_output[i].first, _T("%s%c"), m_output[i].first, msg.ch);
						//���¹��λ��
						settextstyle(16, 0, _T("΢���ź�"));
						m_cursorPos.first = m_inputBox[i].beginX + textwidth(m_output[i].first)+10;
						break;
					}
				}
			}
		}
	}

}

void MyInputBox::draw()
{
	
	//������
	fillroundrect(m_beginX, m_beginY, m_endX, m_endY, 10, 10);

	int x = m_beginX + LEFT_MARGIN;
	int y = TOP_MARGIN + m_beginY;

	settextstyle(16, 0, _T("΢���ź�"));
	//����
	outtextxy(x, y, m_title);

	//�ָ���
	y += textheight(m_title);
	line(m_beginX, y + MIDDLE_LINE_MARGIN / 4, m_endX, y + MIDDLE_LINE_MARGIN / 4);

	settextstyle(20, 0, _T("΢���ź�"));
	//��ʾ����
	y += MIDDLE_LINE_MARGIN;
	for (int i = 0; i < m_contentSplitPoint.size(); i++) {
		outtextxy(x, y + textheight(m_content) * i, m_content + m_contentSplitPoint[i]);
		y += TEXT_MIDDLE_MARGIN;
	}

	//��ť
	for (OptionBtn& btn : m_buttons) {
		btn.draw();
	}

	settextstyle(16, 0, _T("΢���ź�"));
	//���������
	for (int i = 0; i < m_inputBox.size(); i++) {
		m_inputBox[i].draw(m_output[i].first, BTN_TEXT_STYLE_LEFT);
	}

	settextstyle(20, 0, _T("΢���ź�"));
	//����������ǩ
	for (int i = 0; i < m_inputBoxLabel.size(); i++) {
		m_inputBoxLabel[i].draw(m_label[i], BTN_TEXT_STYLE_LEFT);
	}


	//���ƹ��,��ʵ����˸Ч��
	static int flash = 0;
	if(flash%5!=0&& flash % 6 != 0&& flash % 7 != 0&& flash % 8 != 0&& flash % 9 != 0)
	line(m_cursorPos.first, m_cursorPos.second, m_cursorPos.first, m_cursorPos.second + 16);
	flash++;
	if (flash == INT_MAX)flash = 0;
}

//��һ��������ʾ��Ϣ���ڶ�����:����,��������ť���ͣ����ģ����������û��������ݵ����������壺����������������ÿ�������Ĵ�С
int myInputBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, wchar_t** label, wchar_t** output, int num, std::initializer_list<int> lenList)
{
	MyInputBox box(pcontent, ptitle, messageBoxType, label, output, num, lenList);
	return box.play();
}
