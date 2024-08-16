#pragma once
#include"protocol.h"
#include"Button.h"
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
class registerSystem
{

public:
	//���캯��
	registerSystem();
	registerSystem(SOCKET sock);
	
	//���Ƶ�¼����
	void draw();
	//������¼��������¼�
	void mouseEvent();

	//�ж��Ƿ�ȷ���ύ
	bool isConfirmSubmitJudge();

	//ִ��ѭ��--��¼����ѭ��
	wchar_t* play();
	
	//��ȡ�˺�
	wchar_t* getAccount();

	//��ȡ�������Ϣ
	void recvEvent();

	//���͵�¼����
	void sendLoginRequest();

	//��������ʱ���|��˸
	void setInputFlashFlag(bool isInput, wchar_t* content);

	//��ȡ����
	wchar_t* getPwd();

private:
	IMAGE m_bg;//����ͼ
	wchar_t m_account[33] = {0};//�洢�˺�
	wchar_t m_pwd[33] = { 0 };//�洢����
	//�˺������
	Button m_accountInputBox;
	//�˺��������ʾ
	Button m_accountInputTip;
	//ȷ���ύ��ť
	Button m_confirmSubmitBtn;
	//�Ƿ��������˺�״̬
	bool m_isInputAccount;
	//�Ƿ�ȷ���ύ�˺�����
	bool m_isConfirmSubmit;

	//�������ͨ��
	SOCKET m_csock;

	//���������
	Button m_pwdInputBox;
	//�����������ʾ
	Button m_pwdInputTip;
	
	//�Ƿ�����������״̬
	bool m_isInputPwd;
};

