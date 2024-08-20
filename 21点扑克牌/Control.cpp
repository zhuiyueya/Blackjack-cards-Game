#include "Control.h"
#include<iostream>
#include <Ws2tcpip.h>
//登录系统
void Control::login()
{
	m_loginSys = registerSystem(csock);
	m_loginSys.play();
	wcscpy(m_account, m_loginSys.getAccount());
	wcscpy(m_name, m_loginSys.getUserName());
	std::wcout << m_account << std::endl;

	//登录完初始化设置
	m_setting = Setting(csock, m_account, m_name);
}

Control::Control()
{
	//加载对应卡牌图片
	m_cardImg.resize(CARD_NUM+1);//还有一张图为扑克牌背部图，即抽牌显示的卡片

	initgraph(1040, 525);//临时开启，用来绘制整张卡牌集合图片方便切割

	loadimage(&m_bgImg, _T("./card.png"));
	putimage(0, 0, &m_bgImg);

	//切割图片获取单张牌的图片并保存
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
	
	//加载背景
	loadimage(&m_bgImg, _T("./background-1.png"));

	//初始到主窗口
	m_windowState = WINDOW_STATE_MAIN;

	m_otherPlayerCard.resize(PLAYER_NUM-1);

	loadimage(&m_clockImg, _T("./clock.png"),40,40);
	
	m_userNameBox = Button(0, 0, BTN_SIZE_USER_NAME_BOX_W, BTN_SIZE_USER_NAME_BOX_H);

	m_matchBtnCharSize = 50;

	m_settingBtn = Button(WIDTH - BTN_SIZE_SETTING_W,0, BTN_SIZE_SETTING_W, BTN_SIZE_SETTING_H);
		
	loadimage(&m_settingImg, _T("./设置.png"), BTN_SIZE_SETTING_W, BTN_SIZE_SETTING_H);
}

//游戏初始化
void Control::init()
{
	memset(m_usedCard, 0, 54);

	m_isQuitGame = false;
	
	for (int i = 0; i < PLAYER_NUM - 1; i++) {
		m_otherPlayerCard[i].clear();
	}
	
	m_card.clear();

	//可以防止有其他玩家未连接成功时的一瞬玩家快速点击造成的取牌
	m_nowTurn = -1;
}

//游戏界面绘制
void Control::draw()
{
	
	putimage(0, 0, &m_bgImg);
	//绘制玩家扑克牌
	int allwidth = (m_card.size() + 1) * CARD_WIDTH / 2;
	int beginX = (WIDTH - allwidth) / 2;
	int beginY = 2*HEIGHT / 3;
	for (int i = 0; i < m_card.size(); i++) {
		putimage(beginX + i * CARD_WIDTH / 2, beginY, &m_cardImg[m_card[i].getId()]);
	}

	//绘制中心抽牌区
	putimage((WIDTH - CARD_WIDTH) / 2, (HEIGHT - CARD_HEIGHT) / 2, &m_cardImg[54]);

	//绘制玩家名字
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
	
	//绘制其他玩家卡牌
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
			putimage(beginX + j * CARD_WIDTH / 2, beginY, &m_cardImg[m_otherPlayerCard[i][j].getId()]);
		}
	}

	//绘制闹钟
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

		//绘制“过”按钮
		button(clockX + m_clockImg.getwidth() + BTN_SKIP_ROUND_SIZE_W / 2, clockY, _T("过"));

		break;
	}
	default:
		break;
	}
	transparentImage(NULL, clockX, clockY, &m_clockImg, BLACK);
	//putimage(clockX, clockY, &m_clockImg);

	settextstyle(20, 0, _T("微软雅黑"));
	int downcountTimeX = (m_clockImg.getwidth() - textwidth(m_downcountTimeSecond)) / 2 + clockX;
	int downcountTimeY= (m_clockImg.getheight() - textheight(m_downcountTimeSecond)) / 2 + clockY;
	outtextxy(downcountTimeX, downcountTimeY,m_downcountTimeSecond);

	//绘制返回按钮
	button(0, 0, _T("返回"));

	
}

//生成随机卡牌-暂废弃
void Control::randomCardDeal()
{
	static int card_num = 53;
	//std::cout << card_num << std::endl;
	if (card_num <= 0)return;
	int card_num1;
	while (m_usedCard[(card_num1 = rand() % 54)]);
	
	m_usedCard[card_num1] = 1;
	Card card1(card_num1);
	m_card.push_back(card1);
	
	card_num--;
}

//向服务器请求卡牌
void Control::getRandomCardFromServer()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_TAKECARD_REQUEST;
	send(gameSock, (char*)&pdu, sizeof(pdu), 0);
	
}

//游戏按键事件
void Control::mouseEvent()
{
	ExMessage msg;
	while (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			
			int mx = msg.x;
			int my = msg.y;

			//闹钟的绘制起始位置
			int clockY = 2 * HEIGHT / 3 - m_clockImg.getheight();
			int clockX = (WIDTH - m_clockImg.getwidth()) / 2;
				
			//抽卡区
			if (mx > (WIDTH - CARD_WIDTH) / 2 && mx< (WIDTH - CARD_WIDTH) / 2 + CARD_WIDTH && my>(HEIGHT - CARD_HEIGHT) / 2 && my < (HEIGHT - CARD_HEIGHT) / 2 + CARD_HEIGHT) {
				if (m_nowTurn == PLAYER_NUM - 1)//在非自方回合时防止玩家获取牌，也可以防止有其他玩家未连接成功时的一瞬玩家快速点击造成的取牌
					getRandomCardFromServer();
				else
					std::cout << m_nowTurn << std::endl;
			}
			else if (mx < 100 && my < 40) {
				closesocket(gameSock);
				m_isQuitGame = true;
			}
			else if (m_nowTurn == PLAYER_NUM - 1&&clockX + m_clockImg.getwidth() + BTN_SKIP_ROUND_SIZE_W / 2 < mx && mx < clockX + m_clockImg.getwidth() + 3 * BTN_SKIP_ROUND_SIZE_W / 2 &&
				clockY<my && clockY + BTN_SKIP_ROUND_SIZE_W>my) {
				m_nowTurn = -1;//仅用来防止服务器的回复未到达而再次进入导致多次发送通知
				sendUpdateDowncountTimeNotify();
			}
			/*else if (mx > WIDTH - 100 && my < 40) {
				match();
			}*/
		}
	}
}

//请求与服务器进行连接
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
	
	//std::cout << ret;
	//未成功连接
	if (ret != 0) {
		//std::cout << "connect error\n"<< WSAGetLastError();
		ret=MessageBox(NULL, _T("未连接到服务器,\n是否重试？\n(取消将退出游戏)"), NULL, MB_RETRYCANCEL);
		
		if (ret == IDRETRY) {//重试
			goto again;
		}
		else if (ret == IDCANCEL) {//取消，退出游戏
			exit(0);
		}
	}
	else {
		//将socket设置为非阻塞
		unsigned long ul = 1;
		ioctlsocket(csock, FIONBIO, &ul);
	}
	
}

//释放资源
Control::~Control()
{
	closesocket(csock);
	//禁止使用DLL
	WSACleanup();
}

//获取用户输入账号
void Control::getUserNameInput()
{
	while (!InputBox(m_account, 32, _T("请输入用户名(请使用英文)：")));
	
	sendUserNameToServer();
}

//用来在游戏主循环中判断游戏是否结束或者中断
bool Control::IsQuitGame()
{
	return m_isQuitGame;
}

//发送用户名给服务器
void Control::sendUserNameToServer()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_LOGIN_REQUEST;
	//将wchar_t转换为char
	wcstombs(pdu.msg, m_account, sizeof(pdu.msg));
	std::cout << pdu.msg << std::endl;
	strcpy(pdu.msg + 32, "a");
	send(csock, (char*)&pdu, sizeof(pdu), 0);
}

//处理主界面的接收事件
void Control::handleMainWindowRecv()
{
	PDU pdu;
	
	int ret=recv(csock, (char*)&pdu, sizeof(pdu), 0);
	
	//非阻塞时没有消息则返回
	if (ret <= 0)return;

	
	if (pdu.msgType == ENUM_MSG_MATCH_RESPOND) {

		bool res=handleMatch(&pdu);
		
		if (res) {//连接成功了，开始游戏
			
			//向服务器请求其他玩家名字
			PDU respdu;
			respdu.msgType = ENUM_MSG_GET_PEER_NAME_REQUEST;
			//将wchar_t转换为char
			wcstombs(respdu.msg, m_account, sizeof(respdu.msg));
			send(gameSock, (char*)&respdu, sizeof(respdu), 0);

			startGame();
		}
		////拷贝其他对方玩家名
		//m_otherPlayerName.resize(PLAYER_NUM - 1);
		//char otherPlayerName[32];
		//for (int i = 0; i < PLAYER_NUM - 1; i++) {
		//	memset(otherPlayerName, 0, sizeof(otherPlayerName));
		//	strncpy(otherPlayerName, pdu.msg + i * 32,32);
		//	m_otherPlayerName[i] = std::string(otherPlayerName);
		//}
		////开始游戏
		//startGame();
	}
}

//匹配玩家
void Control::match()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_MATCH_REQUEST;
	//将wchar_t转换为char
	wcstombs(pdu.msg, m_account, sizeof(pdu.msg));
	send(csock, (char*)&pdu, sizeof(pdu), 0);
	m_windowState = WINDOW_STATE_MATCH;
	//std::cout << __func__ << std::endl;
}

//取消匹配
void Control::cancelMatch()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_CANCEL_MATCH_REQUEST;
	send(csock, (char*)&pdu, sizeof(pdu), 0);
	settextstyle(50, 0, _T("微软雅黑"));
	m_windowState = WINDOW_STATE_MAIN;
	
}

//主界面鼠标事件
void Control::mainWindowMouseEvent()
{

	ExMessage msg;
	while (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {

			int mx = msg.x;
			int my = msg.y;
			//匹配按钮范围
			if (m_windowState == WINDOW_STATE_MAIN&&mx > (WIDTH - BTN_SIZE_MATCH_W) / 2 && mx< (WIDTH - BTN_SIZE_MATCH_W) / 2 + BTN_SIZE_MATCH_W &&
				my>(HEIGHT - BTN_SIZE_MATCH_H) / 2 && my < (HEIGHT - BTN_SIZE_MATCH_H) / 2 + BTN_SIZE_MATCH_H) {
				match();
			}
			//匹配界面返回按钮
			else if (m_windowState == WINDOW_STATE_MATCH && mx < BTN_SIZE_CANCEL_MATCH_W && my < BTN_SIZE_CANCEL_MATCH_H) {//取消匹配区
				cancelMatch();
			}
			//设置按钮
			else if (m_windowState == WINDOW_STATE_MAIN && mx > m_settingBtn.beginX && mx< m_settingBtn.endX && my>m_settingBtn.beginY && my < m_settingBtn.endY) {
				m_setting.play();
			}
		}
		else if (msg.message == WM_MOUSEMOVE) {
			//鼠标位于匹配按钮范围时字体增大效果
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

//游戏循环
void Control::startGame()
{
	settextcolor(BLACK);
	setfillcolor(RGB(249, 204, 226));
	settextstyle(20, 0, _T("微软雅黑"));
	init();
	
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
	
	m_windowState = WINDOW_STATE_MAIN;
}

//绘制主界面
void Control::drawMainWindowUI()
{
	putimage(0, 0, &m_bgImg);
	//绘制刚进入游戏时的主界面
	if(m_windowState == WINDOW_STATE_MAIN)
	{
		settextstyle(m_matchBtnCharSize, 0, _T("微软雅黑"));
		button((WIDTH - BTN_SIZE_MATCH_W) / 2, (HEIGHT - BTN_SIZE_MATCH_H) / 2, _T("匹配游戏"), true, BTN_SIZE_MATCH_W, BTN_SIZE_MATCH_H);

		//显示玩家名
		settextstyle(30, 0, _T("微软雅黑"));
		m_userNameBox.endX = BTN_SIZE_USER_NAME_BOX_W + textwidth(m_name);
		m_userNameBox.draw(m_name);

		//绘制设置按钮
		m_settingBtn.draw(&m_settingImg);
	}
	//绘制匹配界面
	else if (m_windowState == WINDOW_STATE_MATCH) {
		outtextxy((WIDTH - BTN_SIZE_MATCH_W) / 2, (HEIGHT - BTN_SIZE_MATCH_H) / 2, _T("匹配中..."));
		setfillcolor(RGB(249, 204, 226));
		button(0, 0, _T("返回"));
	}
	//绘制匹配失败界面
	else if (m_windowState == WINDOW_STATE_MATCH_FAILED) {
		outtextxy((WIDTH - BTN_SIZE_180_60_W) / 2, (HEIGHT - BTN_SIZE_180_60_H) / 2, _T("匹配失败，请重试"));
		Sleep(1000);
		m_windowState = WINDOW_STATE_MAIN;
	}
	//绘制匹配成功界面--未起到作用
	else if (m_windowState == WINDOW_STATE_MATCH_SUCCEED) {
		outtextxy((WIDTH - BTN_SIZE_180_60_W) / 2, (HEIGHT - BTN_SIZE_180_60_H) / 2, _T("匹配成功"));
		Sleep(1000);
		m_windowState = WINDOW_STATE_MAIN;
	}

	
}

//处理服务器对匹配请求的回复
bool Control::handleMatch(PDU*pdu)
{
	
	if (strcmp(pdu->msg, MATCH_SUCCEED) == 0) {
		//拷贝服务器的游戏进程的端口号
		char port[32];
		strncpy(port, pdu->msg + 32, 32);
		gamePORT = atoi(port);

		//连接服务器游戏进程
		bool ret=sendGameLinkRequest();
		if (ret)
		{
			m_windowState = WINDOW_STATE_MATCH_SUCCEED;//未起到作用
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

//处理匹配的游戏socket与服务器新开辟子进程进行连接
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
		int ret = MessageBox(NULL, _T("未连接到服务器,\n是否重试？\n(取消将退出游戏)"), NULL, MB_RETRYCANCEL);
		if (ret == IDRETRY) {
			goto again;
		}
		else if (ret == IDCANCEL) {
			return false;
		}
	}
	else {//设置非阻塞
		ioctlsocket(gameSock, FIONBIO, &ul);

		return true;
	}
	return false;
}

//游戏进行时接收服务器回复
void Control::gameRecvEvent()
{
	PDU pdu;
	int ret = recv(gameSock, (char*)&pdu, sizeof(pdu), 0);

	//非阻塞时没有消息则返回
	if (ret <= 0)return;

	if (pdu.msgType == ENUM_MSG_TAKECARD_RESPOND) {
		if (pdu.cardId < 0) {//解决没有牌时服务器返回-1
			std::cout << "without card!\n";
			return;
		}
		else if (pdu.cardId > 54) {//解决发送失败或者没有接收到返回的乱码
			std::cout << "take card error\n";
			return;
		}
		//将牌放进己方的牌库
		Card card1(pdu.cardId);
		m_card.push_back(card1);
	}
	else if (pdu.msgType == ENUM_MSG_TAKECARD_RESEND) {//其他人取牌的转发
		if (pdu.cardId < 0) {//解决没有牌时服务器返回-1
			return;
		}
		else if (pdu.cardId > 54) {//解决没有接收到返回的乱码
			return;
		}
		Card card1(pdu.cardId);
		//std::cout << __func__<<pdu.player << std::endl;
		//找出player对应的牌库放其他玩家的牌
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
	else if (pdu.msgType == ENUM_MSG_GET_PEER_NAME_RESPOND) {//获取其他玩家名字回复
		//拷贝其他对方玩家名
		m_otherPlayerName.resize(PLAYER_NUM - 1);
		char otherPlayerName[32];
		for (int i = 0; i < PLAYER_NUM - 1; i++) {
			memset(otherPlayerName, 0, sizeof(otherPlayerName));
			strncpy(otherPlayerName, pdu.msg + i * 32, 32);
			m_otherPlayerName[i] = std::string(otherPlayerName);
			std::cout << i << ":" << m_otherPlayerName[i] << std::endl;
		}
		//获取当前抽牌方
		m_nowTurn = pdu.player;

		//游戏开始时每名玩家默认有两张卡牌--在此处进行可以确保每名玩家都已经连接到服务器
		for (int i = 0; i < 2; i++) {
			getRandomCardFromServer();
		}

		//更新计算倒计时的开始时间
		time_t nowTime;
		time(&nowTime);
		localtime_s(&beginP, &nowTime);
	}
	else if (pdu.msgType == ENUM_MSG_GAMEOVER_NOTIFY) {//游戏结束或终止通知
		if (strcmp(pdu.msg, GAME_INTERRUPT) == 0)//有玩家中途退出的情况
		{
			closesocket(gameSock);//关闭与服务器子线程游戏服务器的连接

			myMessageBox(NULL, _T("有玩家非正常退出,\n游戏被迫终止\n(点击确认退出当局游戏)"));

			m_isQuitGame = true;
		}
		else if (strcmp(pdu.msg, GAME_OVER) == 0) {//游戏正常结束的情况

			closesocket(gameSock);//关闭与服务器子线程游戏服务器的连接
			//拼接提示语和胜利方
			wchar_t winnerName[32] = { 0 };
			if (pdu.player == PLAYER_NUM - 1) {
				swprintf(winnerName,_T("你(%s)"), m_account);
			}
			else {
				mbstowcs(winnerName, m_otherPlayerName[pdu.player].c_str(), 32);
			}
			wchar_t sequence[64] = { 0 };
			swprintf(sequence, _T("游戏结束\n %s 赢了\n(点击确认退出当局游戏)"), winnerName);
			myMessageBox(_T("游戏结束"), sequence);

			m_isQuitGame = true;
		}
	}
	else if (pdu.msgType == ENUM_MSG_UPDATE_DOWNCOUNT_TIME_RESPOND) {//更新倒计时回复
		//获取当前抽牌方
		m_nowTurn = pdu.player;
		//更新计算倒计时的开始时间的初始时间
		time_t nowTime;
		time(&nowTime);
		localtime_s(&beginP, &nowTime);
	}
}

//倒计时
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
	
	//倒计时结束了就自动通知服务器切换下一个下一个取牌玩家
	if (second >= DURATION_OF_ONE_ROUND&& m_nowTurn == 2 ) {
		m_nowTurn = -1;//仅用来防止服务器的回复未到达而再次进入导致多次发送通知
		sendUpdateDowncountTimeNotify();
	}
	second = DURATION_OF_ONE_ROUND - second;
	//绘制计时
	swprintf_s(m_downcountTimeSecond, _T("%d"), second);
}

//发送更新倒计时通知给服务器，以便服务器发送下一轮取牌方给所有同局玩家
void Control::sendUpdateDowncountTimeNotify()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_UPDATE_DOWNCOUNT_TIME_NOTIFY;
	send(gameSock, (char*)&pdu, sizeof(pdu), 0);
}




