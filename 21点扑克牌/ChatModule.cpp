#include "ChatModule.h"
#include<iostream>
#include"Alpha.h"
#define TEXT_BOX_H 40//用户输入框的高度，显示在游戏屏幕最下方

#define MAX_INPUT_CHAR_NUM_ONCE 128//输入框单此最多可输入字符

#define MAX_ALL_TEXT_CHAR_NUM 10//聊天栏最多显示行

#define MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM 16//聊天栏单行最多显示文字

#define CHAT_COLUMN_W 300//聊天栏宽度

//聊天栏文本颜色
COLORREF textColors[10] = { WHITE,BLUE,GREEN,RGB(255, 192, 203),RGB(255, 105, 180),RGB(238, 130, 238),RGB(0, 255, 255),RGB(255, 255, 0),RGB(253, 245, 230),RGB(255, 69, 0) };

ChatModule::ChatModule(SOCKET sock, const wchar_t* userNmae)
{
	
	m_inputText = new wchar_t[MAX_INPUT_CHAR_NUM_ONCE];

	m_inputTextBox = Button(0, HEIGHT - TEXT_BOX_H, WIDTH, TEXT_BOX_H);

	m_allText = new wchar_t*[MAX_ALL_TEXT_CHAR_NUM];
	
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		m_allText[i] = new wchar_t[MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM];
		wmemset(m_allText[i], _T('\0'), MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM);
		
	}
	m_sock = sock;

	m_isInputing = false;

	wcscpy(m_userName, userNmae);

	m_colorId = new int[MAX_INPUT_CHAR_NUM_ONCE];
	memset(m_colorId, 0, MAX_INPUT_CHAR_NUM_ONCE);

	m_preColorId = -1;//初始化为-1，则在第一条消息时不会有任何颜色限制
}

ChatModule::ChatModule()
{
	m_inputText = new wchar_t[MAX_INPUT_CHAR_NUM_ONCE];

	m_inputTextBox = Button(0, HEIGHT - TEXT_BOX_H, WIDTH, TEXT_BOX_H);

	m_allText = new wchar_t* [MAX_ALL_TEXT_CHAR_NUM];
	
	for (int i = 0; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		m_allText[i] = new wchar_t[MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM];
		wmemset(m_allText[i], _T('\0'), MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM);
	}
	

	m_isInputing = false;

	m_colorId = new int[MAX_INPUT_CHAR_NUM_ONCE];
	memset(m_colorId, 0, MAX_INPUT_CHAR_NUM_ONCE);
}

//释放内存
ChatModule::~ChatModule()
{
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
		if (m_isInputing) {//用户正在输入聊天消息
			if (msg->ch == _T('\r')) {//输入回车
				sendMessageToServer();
				
				m_isInputing = false;
			}
			else if (msg->ch == _T('\b')) {//输入backspace
				int len = wcslen(m_inputText);
				if (len > 0) {
					m_inputText[len - 1] = _T('\0');
				}
			}
			else {//其他字符按键                                   //留给用户名和冒号的
				if(wcslen(m_inputText)< MAX_INPUT_CHAR_NUM_ONCE-1-wcslen(m_userName)-1)
				swprintf(m_inputText, _T("%s%c"), m_inputText, msg->ch);
				
			}
		}
		else {//用户不是正在输入聊天消息
			//按下t可以进行一次输入聊天消息
			if (msg->ch == 't') {
				m_isInputing = true;
				//模拟清空输入文字
				wmemset(m_inputText, _T('\0'), MAX_INPUT_CHAR_NUM_ONCE);
			}
		}
		
		
	}
}

void ChatModule::draw()
{
	
	settextstyle(25, 0, _T("微软雅黑"));
	
	//输入状态
	if (m_isInputing) {
		//输入文字区
		m_inputTextBox.draw(m_inputText, BTN_TEXT_STYLE_LEFT);
	}
	
	//聊天框背景
	rec(WIDTH- CHAT_COLUMN_W,HEIGHT-CHAT_COLUMN_W , WIDTH, HEIGHT, RGB(0, 0, 0), 128);

	//聊天框文字
	int x = WIDTH-CHAT_COLUMN_W;
	int y = HEIGHT - 20 * MAX_ALL_TEXT_CHAR_NUM;
	
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
			
			//错误写法::::int n = recv(m_sock, (char*)text, pdu->msgLen / sizeof(wchar_t), 0);
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

//往存储聊天栏信息的allText添加新消息
void ChatModule::addTextToAllText(wchar_t* text)
{
	
	//判断新加入文字显示时分为几行
	int newTextLen = wcslen(text);
	//std::cout << "newTextLen:" << newTextLen << std::endl;
	int row = newTextLen / (MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM-1);
	if (newTextLen % (MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM-1) != 0) {
		row++;
	}

	//将旧数据往前移动
	for (int i = 0, j = row; i < (MAX_ALL_TEXT_CHAR_NUM - row); i++,j++) {
		wchar_t* tmp = m_allText[i];
		m_allText[i] = m_allText[j];
		m_allText[j] = tmp;

		int temp = m_colorId[i];
		m_colorId[i]= m_colorId[j];
		m_colorId[j] = m_colorId[i];
	}

	//将新数据拷贝到m_allText
	wchar_t* t = text;
	int textcolor;
	while ((textcolor = rand() % 10) == m_preColorId);
	m_preColorId = textcolor;
	for (int i = MAX_ALL_TEXT_CHAR_NUM - row; i < MAX_ALL_TEXT_CHAR_NUM; i++) {
		wcsncpy(m_allText[i], t, MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM - 1);
		m_colorId[i] = textcolor;
		t += MAX_CHAT_COLUMN_INDIVIDUAL_LINE_CHAR_NUM;
	}


}

//发送用户发送的消息给服务器
void ChatModule::sendMessageToServer()
{
	//往消息前附加上用户名
	int inputTextLen = wcslen(m_inputText);
	wchar_t* tmp = new wchar_t[inputTextLen + 1];
	//wmemset(tmp, _T('\0'), inputTextLen + 1);
	wmemcpy(tmp, m_inputText, inputTextLen + 1);
	//wmemset(m_inputText, _T('\0'), MAX_INPUT_CHAR_NUM_ONCE);
	swprintf(m_inputText, _T("%s:%s"), m_userName, tmp);

	//发送
	PDU* pdu = mkPDU((wcslen(m_inputText) + 1) * sizeof(wchar_t));
	pdu->msgType = ENUM_MSG_SEND_MESSAGE_REQUEST;
	memcpy((void*)pdu->mainMsg, m_inputText, pdu->msgLen);
	send(m_sock, (char*)pdu, sizeof(PDU)+pdu->msgLen, 0);
	free(pdu);
	delete tmp;
}
