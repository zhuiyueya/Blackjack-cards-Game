#include "Control.h"
#include<iostream>
#include <Ws2tcpip.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
//��¼ϵͳ
void Control::login()
{
	m_loginSys = registerSystem(csock);
	m_loginSys.play();
	wcscpy(m_account, m_loginSys.getAccount());
	wcscpy(m_name, m_loginSys.getUserName());

	//��¼���ʼ������
	m_setting = Setting(csock, m_account, m_name);

	//��¼�겥������

	// ʹ��mciSendString�����������ý����ƽӿ�
	// ����򿪲�����ָ����mp3�ļ�
	wchar_t command[128];
	swprintf(command, _T("open %s alias mymusic"), MUSIC_PATH_MAIN_WINDOW);
	if (mciSendString(command, NULL, 0, NULL) != MMSYSERR_NOERROR)
	{
		printf("Error opening the file.\n");
	}

	wchar_t cc[256];
	swprintf(cc, L"setaudio mymusic volume to %d", 0);
	mciSendString(cc, 0, 0, 0);

	//����
	if (mciSendStringA("play mymusic repeat", NULL, 0, NULL) != MMSYSERR_NOERROR)
	{
		printf("Error playing the file.\n");
		mciSendStringA("close mymusic", NULL, 0, NULL);
	}
}

Control::Control()
{
	//���ض�Ӧ����ͼƬ
	m_cardImg.resize(CARD_NUM+1);//����һ��ͼΪ�˿��Ʊ���ͼ����������ʾ�Ŀ�Ƭ

	initgraph(1040, 525);//��ʱ�����������������ſ��Ƽ���ͼƬ�����и�

	loadimage(&m_bgImg, _T("./card.png"));
	putimage(0, 0, &m_bgImg);

	//�и�ͼƬ��ȡ�����Ƶ�ͼƬ������
	int k = 0;
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 4; j++) {
			getimage(&m_cardImg[k], i * 80, j * 105, 80, 105);
			k++;
		}
	}
	getimage(&m_cardImg[k], 0 * 80, 4 * 105, 80, 105);
	getimage(&m_cardImg[k + 1], 1 * 80, 4 * 105, 80, 105);
	getimage(&m_cardImg[k + 2], 2 * 80, 4 * 105, 80, 105);
	closegraph();
	
	//���ر���
	loadimage(&m_bgImg, _T("./background-1.png"));

	//��ʼ��������
	m_windowState = WINDOW_STATE_MAIN;

	m_otherPlayerCard.resize(PLAYER_NUM-1);

	loadimage(&m_clockImg, _T("./clock.png"),40,40);
	
	//��ʼ���û�����ʾ��
	m_userNameBox = Button(0, 0, BTN_SIZE_USER_NAME_BOX_W, BTN_SIZE_USER_NAME_BOX_H);

	m_matchBtnCharSize = 50;

	//��ʼ�����ð�ť
	m_settingBtn = Button(WIDTH - BTN_SIZE_SETTING_W,0, BTN_SIZE_SETTING_W, BTN_SIZE_SETTING_H);
		
	loadimage(&m_settingImg, _T("./����.png"), BTN_SIZE_SETTING_W, BTN_SIZE_SETTING_H);

	m_chat = NULL;


	////��¼�겥������

	//// ʹ��mciSendString�����������ý����ƽӿ�
	//// ����򿪲�����ָ����mp3�ļ�
	//wchar_t command[128];
	//swprintf(command, _T("open %s alias mymusic"), MUSIC_PATH_MAIN_WINDOW);
	//if (mciSendString(command, NULL, 0, NULL) != MMSYSERR_NOERROR)
	//{
	//	printf("Error opening the file.\n");
	//}

	////����
	//if (mciSendStringA("play mymusic", NULL, 0, NULL) != MMSYSERR_NOERROR)
	//{
	//	printf("Error playing the file.\n");
	//	mciSendStringA("close mymusic", NULL, 0, NULL);
	//}
}

//��Ϸ��ʼ��
void Control::init()
{
	//������¿���
	memset(m_usedCard, 0, CARD_NUM);

	m_isQuitGame = false;
	
	//���������ҿ���
	for (int i = 0; i < PLAYER_NUM - 1; i++) {
		m_otherPlayerCard[i].clear();
	}
	
	//����Լ��Ŀ���
	m_card.clear();

	//���Է�ֹ���������δ���ӳɹ�ʱ��һ˲��ҿ��ٵ����ɵ�ȡ��
	m_nowTurn = -1;

	//��ʼ������ģ��
	m_chat =new ChatModule(gameSock,m_name);
	
}

//��Ϸ�������
void Control::draw()
{
	//���Ʊ���
	putimage(0, 0, &m_bgImg);

	//��������Լ����˿���
	int allwidth = (m_card.size() + 1) * CARD_WIDTH / 2;
	int beginX = (WIDTH - allwidth) / 2;
	int beginY = 2 * HEIGHT / 3;
	for (int i = 0; i < m_card.size(); i++) {
		putimage(beginX + i * CARD_WIDTH / 2, beginY, &m_cardImg[m_card[i].getId()]);
	}

	//�������ĳ�����
	putimage((WIDTH - CARD_WIDTH) / 2, (HEIGHT - CARD_HEIGHT) / 2, &m_cardImg[CARD_NUM]);

	//���������������
	for (int i = 0; i < m_otherPlayerName.size(); i++) {
		switch (i) {
		case 0:
		{
			beginX = 0;
			beginY = HEIGHT / 2;
			break;
		}
		case 1:
		{
			beginX = WIDTH - CARD_WIDTH;
			beginY = CARD_HEIGHT;
			break;
		}
		default:
			break;
		}


		wchar_t w[32] = { 0 };
		mbstowcs(w, m_otherPlayerName[i].c_str(), 32);
		button(beginX, beginY, w);
	}
	
	//����������ҿ���
	for (int i = 0; i < m_otherPlayerCard.size(); i++) {
		allwidth = (m_otherPlayerCard[i].size() + 1) * CARD_WIDTH / 2;
		switch (i) {
		case 0:
		{
			beginX = CARD_WIDTH;
			beginY = HEIGHT / 2;
			break;
		}
		case 1:
		{
			beginX = WIDTH-allwidth-CARD_WIDTH;
			beginY = CARD_HEIGHT;
			break;
		}
		default:
			break;
		}
		for (int j = 0;j < m_otherPlayerCard[i].size(); j++) {
			if (m_isQuitGame) {//��Ϸ����ʱ��ʾ������ҵĿ���
				putimage(beginX + j * CARD_WIDTH / 2, beginY, &m_cardImg[m_otherPlayerCard[i][j].getId()]);
			}
			else {//������Ϸ������ʾΪ���Ƶı�ͼ
				putimage(beginX + j * CARD_WIDTH / 2, beginY, &m_cardImg[CARD_NUM]);
			}
		}
	}

	//�ж�����λ��
	int clockX = -100;
	int clockY = -100;
	switch (m_nowTurn) {
	case 0:
	{
		clockY = HEIGHT / 2 + CARD_HEIGHT;
		clockX = CARD_WIDTH;
		break;
	}
	case 1:
	{
		clockY = CARD_HEIGHT*2;
		clockX = WIDTH - CARD_WIDTH;
		break;
	}
	case PLAYER_NUM-1:
	{
		clockY = 2*HEIGHT / 3 - m_clockImg.getheight();
		clockX = (WIDTH - m_clockImg.getwidth()) / 2;

		//���ơ�������ť
		button(clockX + m_clockImg.getwidth() + BTN_SKIP_ROUND_SIZE_W / 2, clockY, _T("��"));

		break;
	}
	default:
		break;
	}
	//��������
	transparentImage(NULL, clockX, clockY, &m_clockImg, BLACK);

	//���������еĵ���ʱ
	settextstyle(20, 0, _T("΢���ź�"));
	int downcountTimeX = (m_clockImg.getwidth() - textwidth(m_downcountTimeSecond)) / 2 + clockX;
	int downcountTimeY= (m_clockImg.getheight() - textheight(m_downcountTimeSecond)) / 2 + clockY;
	outtextxy(downcountTimeX, downcountTimeY,m_downcountTimeSecond);

	//���Ʒ��ذ�ť
	button(0, 0, _T("����"));

	//��������ģ����ص�
	m_chat->draw();
}

//�������������
void Control::getRandomCardFromServer()
{
	//std::cout << "һ��\n";
	static int tmp = 0;
	PDU pdu;
	pdu.msgLen = tmp;
	pdu.msgType = ENUM_MSG_TAKECARD_REQUEST;
	send(gameSock, (char*)&pdu, sizeof(pdu), 0);
	tmp++;
}

//��Ϸ�����¼�
void Control::mouseEvent()
{
	ExMessage msg;
	while (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			
			int mx = msg.x;
			int my = msg.y;

			//���ӵĻ�����ʼλ��
			int clockY = 2 * HEIGHT / 3 - m_clockImg.getheight();
			int clockX = (WIDTH - m_clockImg.getwidth()) / 2;
				
			//�鿨��
			if (mx > (WIDTH - CARD_WIDTH) / 2 && mx< (WIDTH - CARD_WIDTH) / 2 + CARD_WIDTH && my>(HEIGHT - CARD_HEIGHT) / 2 && my < (HEIGHT - CARD_HEIGHT) / 2 + CARD_HEIGHT) {
				if (m_nowTurn == PLAYER_NUM - 1)//�ڷ��Է��غ�ʱ��ֹ��һ�ȡ�ƣ�Ҳ���Է�ֹ���������δ���ӳɹ�ʱ��һ˲��ҿ��ٵ����ɵ�ȡ��
					getRandomCardFromServer();
				//else
					//std::cout << m_nowTurn << std::endl;
			}
			else if (mx < 100 && my < 40) {//���ذ�ť
				closesocket(gameSock);
				m_isQuitGame = true;
			}
			else if (m_nowTurn == PLAYER_NUM - 1&&clockX + m_clockImg.getwidth() + BTN_SKIP_ROUND_SIZE_W / 2 < mx && mx < clockX + m_clockImg.getwidth() + 3 * BTN_SKIP_ROUND_SIZE_W / 2 &&
				clockY<my && clockY + BTN_SKIP_ROUND_SIZE_W>my) {
				m_nowTurn = -1;//��������ֹ�������Ļظ�δ������ٴν��뵼�¶�η���֪ͨ
				sendUpdateDowncountTimeNotify();
			}
			/*else if (mx > WIDTH - 100 && my < 40) {
				match();
			}*/
		}
		m_chat->inputEvent(&msg);
	}
}

//�������������������
void Control::connectToServer()
{

	WSAStartup(MAKEWORD(2, 2), &wsadata);
	csock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	sockaddr.sin_port = htons(SERVER_PORT);
	again:
	int ret=connect(csock, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
	
	//δ�ɹ�����
	if (ret != 0) {
		
		ret=MessageBox(NULL, _T("δ���ӵ�������,\n�Ƿ����ԣ�\n(ȡ�����˳���Ϸ)"), NULL, MB_RETRYCANCEL);
		
		if (ret == IDRETRY) {//����
			goto again;
		}
		else if (ret == IDCANCEL) {//ȡ�����˳���Ϸ
			exit(0);
		}
	}
	else {
		//��socket����Ϊ������
		unsigned long ul = 1;
		ioctlsocket(csock, FIONBIO, &ul);
	}
	
}

//�ͷ���Դ
Control::~Control()
{
	closesocket(csock);
	//��ֹʹ��DLL
	WSACleanup();
}

//��������Ϸ��ѭ�����ж���Ϸ�Ƿ���������ж�
bool Control::IsQuitGame()
{
	return m_isQuitGame;
}

//�����û�����������
void Control::sendUserNameToServer()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_LOGIN_REQUEST;
	//��wchar_tת��Ϊchar
	wcstombs(pdu.msg, m_account, sizeof(pdu.msg));
	std::cout << pdu.msg << std::endl;
	strcpy(pdu.msg + 32, "a");
	send(csock, (char*)&pdu, sizeof(pdu), 0);
}

//����������Ľ����¼�
void Control::handleMainWindowRecv()
{
	PDU pdu;
	
	int ret=recv(csock, (char*)&pdu, sizeof(pdu), 0);
	
	//������ʱû����Ϣ�򷵻�
	if (ret <= 0)return;

	
	if (pdu.msgType == ENUM_MSG_MATCH_RESPOND) {

		bool res=handleMatch(&pdu);
		
		if (res) {//���ӳɹ��ˣ���ʼ��Ϸ
			
			//����������������������
			PDU respdu;
			respdu.msgType = ENUM_MSG_GET_PEER_NAME_REQUEST;
			//��wchar_tת��Ϊchar
			wcstombs(respdu.msg, m_account, sizeof(respdu.msg));
			send(gameSock, (char*)&respdu, sizeof(respdu), 0);

			startGame();
		}
		////���������Է������
		//m_otherPlayerName.resize(PLAYER_NUM - 1);
		//char otherPlayerName[32];
		//for (int i = 0; i < PLAYER_NUM - 1; i++) {
		//	memset(otherPlayerName, 0, sizeof(otherPlayerName));
		//	strncpy(otherPlayerName, pdu.msg + i * 32,32);
		//	m_otherPlayerName[i] = std::string(otherPlayerName);
		//}
		////��ʼ��Ϸ
		//startGame();
	}
}

//ƥ�����
void Control::match()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_MATCH_REQUEST;
	//��wchar_tת��Ϊchar
	wcstombs(pdu.msg, m_account, sizeof(pdu.msg));
	send(csock, (char*)&pdu, sizeof(pdu), 0);
	m_windowState = WINDOW_STATE_MATCH;
	//std::cout << __func__ << std::endl;
}

//ȡ��ƥ��
void Control::cancelMatch()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_CANCEL_MATCH_REQUEST;
	send(csock, (char*)&pdu, sizeof(pdu), 0);
	settextstyle(50, 0, _T("΢���ź�"));
	m_windowState = WINDOW_STATE_MAIN;
	
}

//����������¼�
void Control::mainWindowMouseEvent()
{

	ExMessage msg;
	while (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {

			int mx = msg.x;
			int my = msg.y;
			//ƥ�䰴ť��Χ
			if (m_windowState == WINDOW_STATE_MAIN&&mx > (WIDTH - BTN_SIZE_MATCH_W) / 2 && mx< (WIDTH - BTN_SIZE_MATCH_W) / 2 + BTN_SIZE_MATCH_W &&
				my>(HEIGHT - BTN_SIZE_MATCH_H) / 2 && my < (HEIGHT - BTN_SIZE_MATCH_H) / 2 + BTN_SIZE_MATCH_H) {
				match();
			}
			//ƥ����淵�ذ�ť
			else if (m_windowState == WINDOW_STATE_MATCH && mx < BTN_SIZE_CANCEL_MATCH_W && my < BTN_SIZE_CANCEL_MATCH_H) {//ȡ��ƥ����
				cancelMatch();
			}
			//���ð�ť
			else if (m_windowState == WINDOW_STATE_MAIN && mx > m_settingBtn.beginX && mx< m_settingBtn.endX && my>m_settingBtn.beginY && my < m_settingBtn.endY) {
				m_setting.play();
			}
		}
		else if (msg.message == WM_MOUSEMOVE) {
			//���λ��ƥ�䰴ť��Χʱ��������Ч��
			int mx = msg.x;
			int my = msg.y;
			if (m_windowState == WINDOW_STATE_MAIN&&mx > (WIDTH - BTN_SIZE_MATCH_W) / 2 && mx< (WIDTH - BTN_SIZE_MATCH_W) / 2 + BTN_SIZE_MATCH_W &&
				my>(HEIGHT - BTN_SIZE_MATCH_H) / 2 && my < (HEIGHT - BTN_SIZE_MATCH_H) / 2 + BTN_SIZE_MATCH_H) {
				m_matchBtnCharSize = 58;
			}
			else {
				m_matchBtnCharSize = 50;
			}
		}
	}

}

//��Ϸѭ��
void Control::startGame()
{
	settextcolor(BLACK);
	setfillcolor(RGB(249, 204, 226));
	settextstyle(20, 0, _T("΢���ź�"));
	init();
	//std::cout << "������\n";
	
	//BeginBatchDraw();
	while (1) {
		cleardevice();
		mouseEvent();
		gameRecvEvent();
		if (IsQuitGame())break;
		draw();
		downcountTime();
		FlushBatchDraw();
		Sleep(60);
	}
	//EndBatchDraw();
	delete m_chat;
	
	m_windowState = WINDOW_STATE_MAIN;
}

//����������
void Control::drawMainWindowUI()
{
	putimage(0, 0, &m_bgImg);
	//���Ƹս�����Ϸʱ��������
	if(m_windowState == WINDOW_STATE_MAIN)
	{
		settextstyle(m_matchBtnCharSize, 0, _T("΢���ź�"));
		button((WIDTH - BTN_SIZE_MATCH_W) / 2, (HEIGHT - BTN_SIZE_MATCH_H) / 2, _T("ƥ����Ϸ"), true, BTN_SIZE_MATCH_W, BTN_SIZE_MATCH_H);

		//��ʾ�����
		settextstyle(30, 0, _T("΢���ź�"));
		m_userNameBox.endX = BTN_SIZE_USER_NAME_BOX_W + textwidth(m_name);
		m_userNameBox.draw(m_name);

		//�������ð�ť
		m_settingBtn.draw(&m_settingImg);
	}
	//����ƥ�����
	else if (m_windowState == WINDOW_STATE_MATCH) {
		outtextxy((WIDTH - BTN_SIZE_MATCH_W) / 2, (HEIGHT - BTN_SIZE_MATCH_H) / 2, _T("ƥ����..."));
		setfillcolor(RGB(249, 204, 226));
		button(0, 0, _T("����"));
	}
	//����ƥ��ʧ�ܽ���
	else if (m_windowState == WINDOW_STATE_MATCH_FAILED) {
		outtextxy((WIDTH - BTN_SIZE_180_60_W) / 2, (HEIGHT - BTN_SIZE_180_60_H) / 2, _T("ƥ��ʧ�ܣ�������"));
		Sleep(1000);
		m_windowState = WINDOW_STATE_MAIN;
	}
	//����ƥ��ɹ�����--δ������
	else if (m_windowState == WINDOW_STATE_MATCH_SUCCEED) {
		outtextxy((WIDTH - BTN_SIZE_180_60_W) / 2, (HEIGHT - BTN_SIZE_180_60_H) / 2, _T("ƥ��ɹ�"));
		Sleep(1000);
		m_windowState = WINDOW_STATE_MAIN;
	}

	
}

//�����������ƥ������Ļظ�
bool Control::handleMatch(PDU*pdu)
{
	
	if (strcmp(pdu->msg, MATCH_SUCCEED) == 0) {
		//��������������Ϸ���̵Ķ˿ں�
		char port[32];
		strncpy(port, pdu->msg + 32, 32);
		gamePORT = atoi(port);

		//���ӷ�������Ϸ����
		bool ret=sendGameLinkRequest();
		if (ret)
		{
			m_windowState = WINDOW_STATE_MATCH_SUCCEED;//δ������
			return true;
		}
	}
	else if (strcmp(pdu->msg, MATCH_FAILED) == 0) {
		m_windowState = WINDOW_STATE_MATCH_FAILED;
	}
	else {
		std::cout << __func__<<" error\n";
	}
	m_windowState = WINDOW_STATE_MATCH_FAILED;
	return false;
}

//����ƥ�����Ϸsocket��������¿����ӽ��̽�������
bool Control::sendGameLinkRequest()
{
	
	gameSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	unsigned long ul = 1;
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	sockaddr.sin_port = htons(gamePORT);
again:
	int ret = connect(gameSock, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

	
	if (ret != 0) {
		int ret = MessageBox(NULL, _T("δ���ӵ�������,\n�Ƿ����ԣ�\n(ȡ�����˳���Ϸ)"), NULL, MB_RETRYCANCEL);
		if (ret == IDRETRY) {
			goto again;
		}
		else if (ret == IDCANCEL) {
			return false;
		}
	}
	else {//���÷�����
		ioctlsocket(gameSock, FIONBIO, &ul);

		return true;
	}
	return false;
}

//��Ϸ����ʱ���շ������ظ�
void Control::gameRecvEvent()
{
	PDU pdu;
	int ret = recv(gameSock, (char*)&pdu, sizeof(pdu), 0);

	//������ʱû����Ϣ�򷵻�
	if (ret <= 0)return;

	if (pdu.msgType == ENUM_MSG_TAKECARD_RESPOND) {
		if (pdu.cardId < 0) {//���û����ʱ����������-1
			std::cout << "without card!\n";
			return;
		}
		else if (pdu.cardId > 54) {//�������ʧ�ܻ���û�н��յ����ص�����
			std::cout << "take card error\n";
			return;
		}
		
		//���ƷŽ��������ƿ�
		Card card1(pdu.cardId);
		m_card.push_back(card1);
	}
	else if (pdu.msgType == ENUM_MSG_TAKECARD_RESEND) {//������ȡ�Ƶ�ת��
		if (pdu.cardId < 0) {//���û����ʱ����������-1
			return;
		}
		else if (pdu.cardId > 54) {//���û�н��յ����ص�����
			return;
		}
		Card card1(pdu.cardId);
		
		//std::cout << __func__<<pdu.player << std::endl;
		//�ҳ�player��Ӧ���ƿ��������ҵ���
		switch (pdu.player) {
		case 0:
		{
			m_otherPlayerCard[0].push_back(card1);
			break;

		}
		case 1:
		{
			m_otherPlayerCard[1].push_back(card1);
			break;
		}
		default:
			break;
		}

	}
	else if (pdu.msgType == ENUM_MSG_GET_PEER_NAME_RESPOND) {//��ȡ����������ֻظ�
		//���������Է������
		m_otherPlayerName.resize(PLAYER_NUM - 1);
		char otherPlayerName[32];
		for (int i = 0; i < PLAYER_NUM - 1; i++) {
			memset(otherPlayerName, 0, sizeof(otherPlayerName));
			strncpy(otherPlayerName, pdu.msg + i * 32, 32);
			m_otherPlayerName[i] = std::string(otherPlayerName);
			std::cout << i << ":" << m_otherPlayerName[i] << std::endl;
		}
		//��ȡ��ǰ���Ʒ�
		m_nowTurn = pdu.player;

		//��Ϸ��ʼʱÿ�����Ĭ�������ſ���--�ڴ˴����п���ȷ��ÿ����Ҷ��Ѿ����ӵ�������
		for (int i = 0; i < 2; i++) {
			getRandomCardFromServer();
		}

		//���¼��㵹��ʱ�Ŀ�ʼʱ��
		time_t nowTime;
		time(&nowTime);
		localtime_s(&beginP, &nowTime);
	}
	else if (pdu.msgType == ENUM_MSG_GAMEOVER_NOTIFY) {//��Ϸ��������ֹ֪ͨ
		if (strcmp(pdu.msg, GAME_INTERRUPT) == 0)//�������;�˳������
		{

			closesocket(gameSock);//�ر�����������߳���Ϸ������������

			m_isQuitGame = true;//��������ʾ����ǰ���ҵ���draw��������ʾ��ÿλ��ҵĿ���
			draw();
	
			myMessageBox(_T("����ҷ������˳�,\n��Ϸ������ֹ\n(���ȷ���˳�������Ϸ)"),_T("�쳣"));

			
		}
		else if (strcmp(pdu.msg, GAME_OVER) == 0) {//��Ϸ�������������

			m_isQuitGame = true;//��������ʾ����ǰ���ҵ���draw��������ʾ��ÿλ��ҵĿ���

			closesocket(gameSock);//�ر�����������߳���Ϸ������������
			//ƴ����ʾ���ʤ����
			wchar_t winnerName[32] = { 0 };
			if (pdu.player == PLAYER_NUM - 1) {
				swprintf(winnerName,_T("��(%s)"), m_name);
			}
			else {
				mbstowcs(winnerName, m_otherPlayerName[pdu.player].c_str(), 32);
			}
			wchar_t sequence[64] = { 0 };
			swprintf(sequence, _T("��Ϸ����\n %s Ӯ��\n(���ȷ���˳�������Ϸ)"), winnerName);
			
			draw();

			myMessageBox(sequence, _T("��Ϸ����"));
		}
	}
	else if (pdu.msgType == ENUM_MSG_UPDATE_DOWNCOUNT_TIME_RESPOND) {//���µ���ʱ�ظ�
		//��ȡ��ǰ���Ʒ�
		m_nowTurn = pdu.player;
		//���¼��㵹��ʱ�Ŀ�ʼʱ��ĳ�ʼʱ��
		time_t nowTime;
		time(&nowTime);
		localtime_s(&beginP, &nowTime);
	}
	else if (pdu.msgType == ENUM_MSG_SEND_MESSAGE_RESPOND) {
		m_chat->recvEvent(&pdu);
	}

}

//����ʱ
void Control::downcountTime()
{
	time_t nowTime;
	time(&nowTime);
	tm p;
	localtime_s(&p, &nowTime);
	//cout << p.tm_year + 1900 <<":"<< p.tm_mon + 1 << ":" << p.tm_mday << ":" << p.tm_hour << ":" << p.tm_min << ":" << p.tm_sec << endl;
	int minute = p.tm_min - beginP.tm_min;
	int second = p.tm_sec - beginP.tm_sec;
	if (second < 0) {
		minute--;
		second += 60;
	}
	
	//����ʱ�����˾��Զ�֪ͨ�������л���һ����һ��ȡ�����
	if (second >= DURATION_OF_ONE_ROUND&& m_nowTurn == 2 ) {
		m_nowTurn = -1;//��������ֹ�������Ļظ�δ������ٴν��뵼�¶�η���֪ͨ
		sendUpdateDowncountTimeNotify();
	}
	second = DURATION_OF_ONE_ROUND - second;
	//���Ƽ�ʱ
	swprintf_s(m_downcountTimeSecond, _T("%d"), second);
}

//���͸��µ���ʱ֪ͨ�����������Ա������������һ��ȡ�Ʒ�������ͬ�����
void Control::sendUpdateDowncountTimeNotify()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_UPDATE_DOWNCOUNT_TIME_NOTIFY;
	send(gameSock, (char*)&pdu, sizeof(pdu), 0);
}




