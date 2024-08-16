#pragma once
#include"Card.h"
#include"registerSystem.h"
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
	wchar_t m_userName[32];
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
public:

	//��¼ϵͳ
	void login();

	Control();

	//��ʼ��
	void init();
	
	void draw();

	//���������������
	void randomCardDeal();

	void getRandomCardFromServer();

	//����¼�
	void mouseEvent();

	void connectToServer();

	~Control();

	//��������û���
	void getUserNameInput();

	//�ж��Ƿ��˳���Ϸ
	bool IsQuitGame();

	//��¼ʱ�����û�������
	void sendUserNameToServer();

	//������շ��������͵�����
	void handleRecv();

	//����ƥ������
	void match();

	//ȡ��ƥ�䣬����ȡ��ƥ������
	void cancelMatch();

	//����������¼�
	void mainWindowMouseEvent();

	//��ʼ��Ϸ
	void startGame();

	//��ʼ��Ϸ����
	void drawStartGameUI();

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


