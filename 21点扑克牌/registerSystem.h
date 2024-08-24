#pragma once
#include"protocol.h"
#include"Button.h"
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
#include<thread>
#include<future>
#include <condition_variable>
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

	//���л�Ϊע�����ʱ�ĳ�ʼ��
	void initRegisterWindow();

	//Ϊ�������������ӹ��
	void addCursor(wchar_t *content);

	//Ϊ��������������Ƴ����
	void removeCursor(wchar_t* content);

	//��content��������ַ�
	void addNewChar(wchar_t* content,char newch);

	//��content���Ƴ����ַ�
	void delOldChar(wchar_t* content);

	//����ע������
	void sendRegisterRequest();

	//���ͻ�ȡ��֤������
	bool sendGetVerificationCodeRequest();


	//��ע����淵�ص�¼����ʱ����һЩ����
	void resetLoginWindowSet();

	//���ٴη�����֤��ĵ���ʱ
	int verifiCodeCountdown();

	wchar_t* getUserName();
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

	//��ǰ�������
	windowstate m_windowState;

	//ע�ᰴť
	Button m_registerBtn;

	//�����ʼ������
	Button m_emailInputBox;

	//�����ʼ��������ʾ
	Button m_emailInputTip;

	//��֤�������
	Button m_verificationCodeInputBox;

	//��֤���������ʾ
	Button m_verificationCodeInputTip;

	//������֤�밴ť
	Button m_sendVerificationCodeBtn;

	//�洢�����ַ
	wchar_t m_email[33] = {0};

	//�洢��֤��
	wchar_t m_verificationCode[33] = { 0 };

	//�Ƿ����������ʼ���ַ
	bool m_isInputEmail;

	//�Ƿ�����������֤��
	bool m_isInputVerificationCode;

	//ע����淵�ذ�ť
	Button m_RegisterWindowReturnBtn;

	//����ʱ
	int m_countdownTime;

	//�Ƿ��ڷ�����֤����ȴ��
	bool m_isVCodeCooldown;

	//������֤����ȴ��ʱ�̵߳�future
	std::future<int>m_future;

	//�洢�û���Ϸ��
	wchar_t m_userName[32];
};





