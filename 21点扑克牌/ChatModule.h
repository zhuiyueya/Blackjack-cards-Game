#pragma once
#include"protocol.h"
#include"Button.h"
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")


class ChatModule
{
public:
	ChatModule(SOCKET sock,const wchar_t *userNmae);
	ChatModule();
	~ChatModule();
	
	//用户输入事件
	void inputEvent(ExMessage* msg);

	//绘制
	void draw();

	//处理服务器发送的数据
	void recvEvent(PDU* pdu);
	
private:

	//往存储聊天栏信息的allText添加新消息
	void addTextToAllText(wchar_t* text);

	//发送用户发送的消息给服务器
	void sendMessageToServer();

private:
	wchar_t** m_allText;//存储聊天栏的文字

	wchar_t* m_inputText;//存储输入框的文字

	bool m_isInputing;//判断是否正在输入

	Button m_inputTextBox;//文字输入框

	SOCKET m_sock;//与子进程游戏服务器通信

	wchar_t m_userName[32];//用户名

	int* m_colorId;//聊天栏每条信息的颜色

	int m_preColorId;//聊天栏中上一条消息的颜色
};

