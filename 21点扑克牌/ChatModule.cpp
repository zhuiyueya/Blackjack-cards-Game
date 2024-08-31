#include "ChatModule.h"
#include<iostream>
#include"Alpha.h"
#define TEXT_BOX_H 40//�û������ĸ߶ȣ���ʾ����Ϸ��Ļ���·�

#define MAX_INPUT_CHAR_NUM_ONCE 128//����򵥴����������ַ�

#define MAX_ALL_TEXT_CHAR_NUM 10//�����������ʾ��

#define MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM 16//���������������ʾ����

#define CHAT_COLUMN_W 300

COLORREF textColors[10] = { WHITE,BLUE,GREEN,RGB(255, 192, 203),RGB(255, 105, 180),RGB(238, 130, 238),RGB(0, 255, 255),RGB(255, 255, 0),RGB(253, 245, 230),RGB(255, 69, 0) };

ChatModule::ChatModule(SOCKET sock, const wchar_t* userNmae)
{
	//std::cout << "�вι���\n";
	m_inputText = new wchar_t[MAX_INPUT_CHAR_NUM_ONCE];

	m_inputTextBox = Button(0, HEIGHT - TEXT_BOX_H, WIDTH, TEXT_BOX_H);

	m_allText = new wchar_t*[MAX_ALL_TEXT_CHAR_NUM];
	//std::cout << m_allText << std::endl;
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		m_allText[i] = new wchar_t[MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM];
		wmemset(m_allText[i], _T('\0'), MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM);
		//wcscpy(m_allText[i], _T("\0"));
	}
	m_sock = sock;

	m_isInputing = false;

	wcscpy(m_userName, userNmae);

	m_colorId = new int[MAX_INPUT_CHAR_NUM_ONCE];
	memset(m_colorId, 0, MAX_INPUT_CHAR_NUM_ONCE);
}

ChatModule::ChatModule()
{
	m_inputText = new wchar_t[MAX_INPUT_CHAR_NUM_ONCE];

	m_inputTextBox = Button(0, HEIGHT - TEXT_BOX_H, WIDTH, TEXT_BOX_H);

	m_allText = new wchar_t* [MAX_ALL_TEXT_CHAR_NUM];
	//std::cout << m_allText << std::endl;
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		m_allText[i] = new wchar_t[MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM];
		wmemset(m_allText[i], _T('\0'), MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM);
		//wcscpy(m_allText[i], _T("\0"));
	}
	

	m_isInputing = false;

	m_colorId = new int[MAX_INPUT_CHAR_NUM_ONCE];
	memset(m_colorId, 0, MAX_INPUT_CHAR_NUM_ONCE);
}

ChatModule::~ChatModule()
{
	//std::cout << "����\n";
	delete[]m_inputText;
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		delete[]m_allText[i];
	}
	delete []m_allText;
	delete[]m_colorId;
}

void ChatModule::inputEvent(ExMessage* msg)
{
	
	if (msg->message == WM_CHAR) {
		if (m_isInputing) {//�û���������������Ϣ
			if (msg->ch == _T('\r')) {//����س�
				sendMessageToServer();
				//addTextToAllText(m_inputText);
				m_isInputing = false;
			}
			else if (msg->ch == _T('\b')) {//����backspace
				int len = wcslen(m_inputText);
				if (len > 0) {
					m_inputText[len - 1] = _T('\0');
				}
			}
			else {//�����ַ�����                                   //�����û�����ð�ŵ�
				if(wcslen(m_inputText)< MAX_INPUT_CHAR_NUM_ONCE-1-wcslen(m_userName)-1)
				swprintf(m_inputText, _T("%s%c"), m_inputText, msg->ch);
				//std::cout << wcslen(m_inputText) << std::endl;
			}
		}
		else {//�û�������������������Ϣ
			//����t���Խ���һ������������Ϣ
			if (msg->ch == 't') {
				m_isInputing = true;
				//ģ�������������
				wmemset(m_inputText, _T('\0'), MAX_INPUT_CHAR_NUM_ONCE);
				//wcscpy(m_inputText, _T("\0"));
			}
		}
		
		
	}
}

void ChatModule::draw()
{
	
	settextstyle(25, 0, _T("΢���ź�"));
	
	//����״̬
	if (m_isInputing) {
		//����������
		//setfillcolor(WHITE);
		m_inputTextBox.draw(m_inputText, BTN_TEXT_STYLE_LEFT);
	}
	
	//����򱳾�
	rec(WIDTH- CHAT_COLUMN_W,HEIGHT-CHAT_COLUMN_W , WIDTH, HEIGHT, RGB(0, 0, 0), 128);

	//���������
	int x = WIDTH-CHAT_COLUMN_W;
	int y = HEIGHT - 20 * MAX_ALL_TEXT_CHAR_NUM;
	//int randomTextColor = rand() % 10;
	//settextcolor(textColors[randomTextColor]);
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		settextcolor(textColors[m_colorId[i]]);
		outtextxy(x, y + i * 20, m_allText[i]);
	}
	settextcolor(BLACK);

}

void ChatModule::recvEvent(PDU* pdu)
{
	switch (pdu->msgType) {
		case ENUM_MSG_SEND_MESSAGE_RESPOND:
		{
			//std::cout << "msgLen:"<<pdu->msgLen <<":"<< sizeof(wchar_t)<< std::endl;
			wchar_t* text = new wchar_t[(pdu->msgLen) / sizeof(wchar_t)];
			wmemset(text, _T('\0'), (pdu->msgLen) / sizeof(wchar_t));
			int n=recv(m_sock, (char*)text, pdu->msgLen, 0);
			
			//����д��::::int n = recv(m_sock, (char*)text, pdu->msgLen / sizeof(wchar_t), 0);
			//std::wcout << text << std::endl;
			//std::cout <<"n="<< n << std::endl;
			addTextToAllText(text);
			delete []text;
			break;
		}
		default:
			break;
	}
}

void ChatModule::addTextToAllText(wchar_t* text)
{
	
	//�ж��¼���������ʾʱ��Ϊ����
	int newTextLen = wcslen(text);
	//std::cout << "newTextLen:" << newTextLen << std::endl;
	int row = newTextLen / (MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM-1);
	if (newTextLen % (MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM-1) != 0) {
		row++;
	}

	//����������ǰ�ƶ�
	for (int i = 0, j = row; i < (MAX_ALL_TEXT_CHAR_NUM - row); i++,j++) {
		wchar_t* tmp = m_allText[i];
		m_allText[i] = m_allText[j];
		m_allText[j] = tmp;

		int temp = m_colorId[i];
		m_colorId[i]= m_colorId[j];
		m_colorId[j] = m_colorId[i];
	}

	//�������ݿ�����m_allText
	wchar_t* t = text;
	int textcolor = rand() % 10;
	for (int i = MAX_ALL_TEXT_CHAR_NUM - row; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		wcsncpy(m_allText[i], t, MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM - 1);
		m_colorId[i] = textcolor;
		t += MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM;
	}


}

//�����û����͵���Ϣ��������
void ChatModule::sendMessageToServer()
{
	int inputTextLen = wcslen(m_inputText);

	wchar_t* tmp = new wchar_t[inputTextLen + 1];

	//wmemset(tmp, _T('\0'), inputTextLen + 1);
	
	wmemcpy(tmp, m_inputText, inputTextLen + 1);

	//wmemset(m_inputText, _T('\0'), MAX_INPUT_CHAR_NUM_ONCE);

	swprintf(m_inputText, _T("%s:%s"), m_userName, tmp);
	PDU* pdu = mkPDU((wcslen(m_inputText) + 1) * sizeof(wchar_t));
	pdu->msgType = ENUM_MSG_SEND_MESSAGE_REQUEST;
	memcpy((void*)pdu->mainMsg, m_inputText, pdu->msgLen);
	send(m_sock, (char*)pdu, sizeof(PDU)+pdu->msgLen, 0);
	free(pdu);
	delete tmp;
}
