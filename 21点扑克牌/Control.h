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

	wchar_t m_account[32];//用户账号

	bool m_isQuitGame;

	std::vector<std::vector<Card>>m_otherPlayerCard;

	std::vector<std::string>m_otherPlayerName;

	int m_windowState;//当前所处界面状态

	SOCKET gameSock;//执行游戏时与服务器通信

	int gamePORT;//服务器子进程端口号

	int m_nowTurn;//当前回合

	IMAGE m_clockImg;
	tm beginP;
	wchar_t m_downcountTimeSecond[32];

	//登录界面类
	registerSystem m_loginSys;

	wchar_t m_name[32];//玩家游戏名

	//主界面显示用户名
	Button m_userNameBox;

	//匹配按钮字体大小
	int m_matchBtnCharSize;

	//设置--在login中登录完成后初始化
	Setting m_setting;

	//设置按钮
	Button m_settingBtn;

	//设置图标
	IMAGE m_settingImg;

	//聊天类
	ChatModule *m_chat;

public:

	//登录系统
	void login();

	Control();

	//游戏初始化
	void init();
	
	//游戏绘制
	void draw();

	//向服务器请求随机卡牌
	void getRandomCardFromServer();

	//鼠标事件
	void mouseEvent();

	//连接服务器
	void connectToServer();

	~Control();

	//判断是否退出游戏
	bool IsQuitGame();

	//登录时发送用户名数据
	void sendUserNameToServer();

	//处理接收服务器发送的数据
	void handleMainWindowRecv();

	//发送匹配请求
	void match();

	//取消匹配，发送取消匹配请求
	void cancelMatch();

	//主界面鼠标事件
	void mainWindowMouseEvent();

	//开始游戏
	void startGame();

	//开始游戏界面
	void drawMainWindowUI();

	//处理服务器对匹配请求的回复
	bool handleMatch(PDU* pdu);

	//处理匹配的游戏socket与服务器新开辟子进程进行连接
	bool sendGameLinkRequest();

	//游戏进行时接收服务器回复
	void gameRecvEvent();

	//倒计时
	void downcountTime();

	//发送更新倒计时通知给服务器，以便服务器发送下一轮取牌方给所有同局玩家
	void sendUpdateDowncountTimeNotify();

	


};


