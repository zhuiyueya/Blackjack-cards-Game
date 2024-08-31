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
	
	void inputEvent(ExMessage* msg);

	void draw();

	void recvEvent(PDU* pdu);
	void sendMessageToServer();
private:

	//���洢��������Ϣ��allText�������Ϣ
	void addTextToAllText(wchar_t* text);

	//�����û����͵���Ϣ��������
	

private:
	wchar_t** m_allText;//�洢������������
	wchar_t* m_inputText;//�洢����������

	bool m_isInputing;//�ж��Ƿ���������

	Button m_inputTextBox;//���������

	SOCKET m_sock;//���ӽ�����Ϸ������ͨ��

	wchar_t m_userName[32];//�û���

	int* m_colorId;//������ÿ����Ϣ����ɫ
};

