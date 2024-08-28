#pragma once
#include"protocol.h"
#include"OptionBtn.h"
#include"MyMessageBox.h"
#include"MyInputBox.h"
#include<winsock2.h>
#include<vector>
#pragma comment(lib,"ws2_32.lib")
//设置界面
class Setting
{
public:
	Setting();

	Setting(SOCKET sock, wchar_t* account, wchar_t* userName);

	//设置界面主循环
	void play();
private:
	//绘制
	void draw();

	//用户输入事件
	void inputEvent();

	//处理服务器回复
	void handleRecv();

	//发送修改昵称请求
	void sendChangeUserName();

private:
	wchar_t* m_account;
	wchar_t* m_userName;
	SOCKET m_sock;
	std::vector<OptionBtn>m_mainOptions;//左侧主选项
	int m_curMainWindowLabel;//当前主选项对应窗口
	IMAGE m_bgImg;
	//修改用户名按钮
	Button m_changeNameBtn;
	
	//修改密码按钮
	Button m_changePwdBtn;

	bool m_isQuit;//是否退出设置界面

	Button m_returnBtn;//返回主界面的按钮
};

