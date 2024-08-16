#pragma once
#include"protocol.h"
#include"Button.h"
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
class registerSystem
{

public:
	//构造函数
	registerSystem();
	registerSystem(SOCKET sock);
	
	//绘制登录界面
	void draw();
	//监听登录界面鼠标事件
	void mouseEvent();

	//判断是否确认提交
	bool isConfirmSubmitJudge();

	//执行循环--登录界面循环
	wchar_t* play();
	
	//获取账号
	wchar_t* getAccount();

	//读取服务端信息
	void recvEvent();

	//发送登录请求
	void sendLoginRequest();

	//设置输入时光标|闪烁
	void setInputFlashFlag(bool isInput, wchar_t* content);

	//获取密码
	wchar_t* getPwd();

private:
	IMAGE m_bg;//背景图
	wchar_t m_account[33] = {0};//存储账号
	wchar_t m_pwd[33] = { 0 };//存储密码
	//账号输入框
	Button m_accountInputBox;
	//账号输入框提示
	Button m_accountInputTip;
	//确认提交按钮
	Button m_confirmSubmitBtn;
	//是否处于输入账号状态
	bool m_isInputAccount;
	//是否确认提交账号密码
	bool m_isConfirmSubmit;

	//与服务器通信
	SOCKET m_csock;

	//密码输入框
	Button m_pwdInputBox;
	//密码输入框提示
	Button m_pwdInputTip;
	
	//是否处于输入密码状态
	bool m_isInputPwd;
};

