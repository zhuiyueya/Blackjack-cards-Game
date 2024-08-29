#include "ChatModule.h"
#include<iostream>
#define TEXT_BOX_H 40//�û������ĸ߶ȣ���ʾ����Ϸ��Ļ���·�

#define MAX_INPUT_CHAR_NUM_ONCE 128//����򵥴����������ַ�

#define MAX_ALL_TEXT_CHAR_NUM 10//�����������ʾ��

#define MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM 16//���������������ʾ����

#define CHAT_COLUMN_W 300

ChatModule::ChatModule(SOCKET sock)
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
}

ChatModule::ChatModule()
{
	/*m_inputText = new wchar_t[MAX_INPUT_CHAR_NUM_ONCE];

	m_inputTextBox = Button(0, HEIGHT - TEXT_BOX_H, WIDTH, TEXT_BOX_H);

	m_allText = new wchar_t* [MAX_ALL_TEXT_CHAR_NUM];
	if (!m_allText) {
		std::cout << "�յ�\n";
	}
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		m_allText[i] = new wchar_t[MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM];
		wcscpy(m_allText[i], _T("\0"));
	}

	m_isInputing = false;*/
	//std::cout << "Ĭ�Ϲ���\n";
}

ChatModule::~ChatModule()
{
	//std::cout << "����\n";
	delete[]m_inputText;
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		delete[]m_allText[i];
	}
	delete []m_allText;
}

void ChatModule::inputEvent(ExMessage* msg)
{
	
	if (msg->message == WM_CHAR) {
		if (m_isInputing) {//�û���������������Ϣ
			if (msg->ch == _T('\r')) {//����س�
				sendMessageToServer();
				addTextToAllText(m_inputText);
				m_isInputing = false;
			}
			else if (msg->ch == _T('\b')) {//����backspace
				int len = wcslen(m_inputText);
				if (len > 0) {
					m_inputText[len - 1] = _T('\0');
				}
			}
			else {//�����ַ�����
				if(wcslen(m_inputText)< MAX_INPUT_CHAR_NUM_ONCE-1)
				swprintf(m_inputText, _T("%s%c"), m_inputText, msg->ch);
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
	
	settextstyle(20, 0, _T("΢���ź�"));
	
	//����״̬
	if (m_isInputing) {
		//����������
		//setfillcolor(WHITE);
		m_inputTextBox.draw(m_inputText, BTN_TEXT_STYLE_LEFT);
	}
	
	//�����
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
	}

	//�������ݿ�����m_allText
	wchar_t* t = text;
	for (int i = MAX_ALL_TEXT_CHAR_NUM - row; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		wcsncpy(m_allText[i], t, MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM - 1);
		t += MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM;
	}


}

//�����û����͵���Ϣ��������
void ChatModule::sendMessageToServer()
{

	PDU* pdu = mkPDU((wcslen(m_inputText) + 1) * sizeof(wchar_t));
	pdu->msgType = ENUM_MSG_SEND_MESSAGE_REQUEST;
	memcpy((void*)pdu->mainMsg, m_inputText, pdu->msgLen);
	send(m_sock, (char*)pdu, sizeof(PDU)+pdu->msgLen, 0);
	free(pdu);
}
