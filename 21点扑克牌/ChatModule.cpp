#include "ChatModule.h"
#include<iostream>
#define TEXT_BOX_H 40//用户输入框的高度，显示在游戏屏幕最下方

#define MAX_INPUT_CHAR_NUM_ONCE 128//输入框单此最多可输入字符

#define MAX_ALL_TEXT_CHAR_NUM 10//聊天栏最多显示行

#define MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM 16//聊天栏单行最多显示文字

#define CHAT_COLUMN_W 300

ChatModule::ChatModule(SOCKET sock)
{
	
	m_inputText = new wchar_t[MAX_INPUT_CHAR_NUM_ONCE];

	m_inputTextBox = Button(0, HEIGHT - TEXT_BOX_H, WIDTH, TEXT_BOX_H);

	m_allText = new wchar_t*[MAX_ALL_TEXT_CHAR_NUM];
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		m_allText[i] = new wchar_t[MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM];
		wcscpy(m_allText[i], _T("\0"));
	}
	m_sock = sock;

	m_isInputing = false;
}

ChatModule::ChatModule()
{
	m_inputText = new wchar_t[MAX_INPUT_CHAR_NUM_ONCE];

	m_inputTextBox = Button(0, HEIGHT - TEXT_BOX_H, WIDTH, TEXT_BOX_H);

	m_allText = new wchar_t* [MAX_ALL_TEXT_CHAR_NUM];
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		m_allText[i] = new wchar_t[MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM];
		wcscpy(m_allText[i], _T("\0"));
	}

	m_isInputing = false;
}

ChatModule::~ChatModule()
{
	delete[]m_inputText;
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		delete[]m_allText[i];
	}
	delete []m_allText;
}

void ChatModule::inputEvent(ExMessage* msg)
{
	
	if (msg->message == WM_CHAR) {
		if (m_isInputing) {//用户正在输入聊天消息
			if (msg->ch == _T('\r')) {//输入回车
				sendMessageToServer();
				addTextToAllText(m_inputText);
				m_isInputing = false;
			}
			else if (msg->ch == _T('\b')) {//输入backspace
				int len = wcslen(m_inputText);
				if (len > 0) {
					m_inputText[len - 1] = _T('\0');
				}
			}
			else {//其他字符按键
				if(wcslen(m_inputText)< MAX_INPUT_CHAR_NUM_ONCE-1)
				swprintf(m_inputText, _T("%s%c"), m_inputText, msg->ch);
			}
		}
		else {//用户不是正在输入聊天消息
			//按下t可以进行一次输入聊天消息
			if (msg->ch == 't') {
				m_isInputing = true;
				//模拟清空输入文字
				wcscpy(m_inputText, _T("\0"));
			}
		}
		
		
	}
}

void ChatModule::draw()
{
	
	settextstyle(20, 0, _T("微软雅黑"));
	
	//输入状态
	if (m_isInputing) {
		//输入文字区
		//setfillcolor(WHITE);
		m_inputTextBox.draw(m_inputText, BTN_TEXT_STYLE_LEFT);
	}
	
	//聊天框
	int x = WIDTH-CHAT_COLUMN_W;
	int y = HEIGHT - 20 * MAX_ALL_TEXT_CHAR_NUM;
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		outtextxy(x, y + i * 20, m_allText[i]);
	}
	
}

void ChatModule::recvEvent(PDU* pdu)
{
	switch (pdu->msgType) {
		case ENUM_MSG_SEND_MESSAGE_RESPOND:
		{
			wchar_t* text = new wchar_t[(pdu->msgLen) / sizeof(wchar_t)];
			recv(m_sock, (char*)text, (pdu->msgLen) / sizeof(wchar_t), 0);
			addTextToAllText(text);
			delete []text;
			break;
		}
	}
}

void ChatModule::addTextToAllText(wchar_t* text)
{
	//判断新加入文字显示时分为几行
	int newTextLen = wcslen(text);
	int row = newTextLen / (MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM-1);
	if (newTextLen % (MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM-1) != 0) {
		row++;
	}

	//将旧数据往前移动
	for (int i = 0, j = row; i < (MAX_ALL_TEXT_CHAR_NUM - row); i++,j++) {
		wchar_t* tmp = m_allText[i];
		m_allText[i] = m_allText[j];
		m_allText[j] = tmp;
	}

	//将新数据拷贝到m_allText
	wchar_t* t = text;
	for (int i = MAX_ALL_TEXT_CHAR_NUM - row; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		wcsncpy(m_allText[i], t, MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM - 1);
		t += MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM;
	}


}

//发送用户发送的消息给服务器
void ChatModule::sendMessageToServer()
{

	PDU* pdu = mkPDU((wcslen(m_inputText) + 1) * sizeof(wchar_t));
	pdu->msgType = ENUM_MSG_SEND_MESSAGE_REQUEST;
	memcpy((void*)pdu->mainMsg, m_inputText, pdu->msgLen);
	send(m_sock, (char*)pdu, sizeof(*pdu), 0);
	free(pdu);
}
