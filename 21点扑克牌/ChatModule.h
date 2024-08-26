#pragma once
#include"protocol.h"
#include"Button.h"
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")


class ChatModule
{
public:
	ChatModule(SOCKET sock);
	ChatModule();
	~ChatModule();
	
	void inputEvent(ExMessage* msg);

	void draw();

	void recvEvent(PDU* pdu);
	void sendMessageToServer();
private:

	//往存储聊天栏信息的allText添加新消息
	void addTextToAllText(wchar_t* text);

	//发送用户发送的消息给服务器
	

private:
	wchar_t** m_allText;//存储聊天栏的文字
	wchar_t* m_inputText;//存储输入框的文字

	bool m_isInputing;//判断是否正在输入

	Button m_inputTextBox;//文字输入框

	SOCKET m_sock;
};

