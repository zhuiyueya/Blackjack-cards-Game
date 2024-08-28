#pragma once
#include"protocol.h"
#include"OptionBtn.h"
#include"MyMessageBox.h"
#include"MyInputBox.h"
#include<winsock2.h>
#include<vector>
#pragma comment(lib,"ws2_32.lib")
//���ý���
class Setting
{
public:
	Setting();

	Setting(SOCKET sock, wchar_t* account, wchar_t* userName);

	//���ý�����ѭ��
	void play();
private:
	//����
	void draw();

	//�û������¼�
	void inputEvent();

	//����������ظ�
	void handleRecv();

	//�����޸��ǳ�����
	void sendChangeUserName();

private:
	wchar_t* m_account;
	wchar_t* m_userName;
	SOCKET m_sock;
	std::vector<OptionBtn>m_mainOptions;//�����ѡ��
	int m_curMainWindowLabel;//��ǰ��ѡ���Ӧ����
	IMAGE m_bgImg;
	//�޸��û�����ť
	Button m_changeNameBtn;
	
	//�޸����밴ť
	Button m_changePwdBtn;

	bool m_isQuit;//�Ƿ��˳����ý���

	Button m_returnBtn;//����������İ�ť
};

