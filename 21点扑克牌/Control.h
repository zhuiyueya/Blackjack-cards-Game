#pragma once
#include"Card.h"
#include"registerSystem.h"
#include"Setting.h"
#include"ChatModule.h"
#include<Windows.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
class Control
{
private:
	IMAGE m_bgImg;

	std::vector<IMAGE>m_cardImg;

	std::vector<Card>m_card;

	bool m_usedCard[54];

	WSADATA wsadata;

	SOCKET csock;

	wchar_t m_account[32];//�û��˺�

	bool m_isQuitGame;

	std::vector<std::vector<Card>>m_otherPlayerCard;

	std::vector<std::string>m_otherPlayerName;

	int m_windowState;//��ǰ��������״̬

	SOCKET gameSock;//ִ����Ϸʱ�������ͨ��

	int gamePORT;//�������ӽ��̶˿ں�

	int m_nowTurn;//��ǰ�غ�

	IMAGE m_clockImg;
	tm beginP;
	wchar_t m_downcountTimeSecond[32];

	//��¼������
	registerSystem m_loginSys;

	wchar_t m_name[32];//�����Ϸ��

	//��������ʾ�û���
	Button m_userNameBox;

	//ƥ�䰴ť�����С
	int m_matchBtnCharSize;

	//����--��login�е�¼��ɺ��ʼ��
	Setting m_setting;

	//���ð�ť
	Button m_settingBtn;

	//����ͼ��
	IMAGE m_settingImg;

	//������
	ChatModule *m_chat;

public:

	//��¼ϵͳ
	void login();

	Control();

	//��Ϸ��ʼ��
	void init();
	
	//��Ϸ����
	void draw();

	//������������������
	void getRandomCardFromServer();

	//����¼�
	void mouseEvent();

	//���ӷ�����
	void connectToServer();

	~Control();

	//�ж��Ƿ��˳���Ϸ
	bool IsQuitGame();

	//��¼ʱ�����û�������
	void sendUserNameToServer();

	//������շ��������͵�����
	void handleMainWindowRecv();

	//����ƥ������
	void match();

	//ȡ��ƥ�䣬����ȡ��ƥ������
	void cancelMatch();

	//����������¼�
	void mainWindowMouseEvent();

	//��ʼ��Ϸ
	void startGame();

	//��ʼ��Ϸ����
	void drawMainWindowUI();

	//�����������ƥ������Ļظ�
	bool handleMatch(PDU* pdu);

	//����ƥ�����Ϸsocket��������¿����ӽ��̽�������
	bool sendGameLinkRequest();

	//��Ϸ����ʱ���շ������ظ�
	void gameRecvEvent();

	//����ʱ
	void downcountTime();

	//���͸��µ���ʱ֪ͨ�����������Ա������������һ��ȡ�Ʒ�������ͬ�����
	void sendUpdateDowncountTimeNotify();

	


};


