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

	//当切换为注册界面时的初始化
	void initRegisterWindow();

	//为鼠标点击的所属添加光标
	void addCursor(wchar_t *content);

	//为非鼠标点击的所属移除光标
	void removeCursor(wchar_t* content);

	//往content内添加新字符
	void addNewChar(wchar_t* content,char newch);

	//往content内移除旧字符
	void delOldChar(wchar_t* content);

	//发送注册请求
	void sendRegisterRequest();

	//发送获取验证码请求
	bool sendGetVerificationCodeRequest();


	//从注册界面返回登录界面时重置一些设置
	void resetLoginWindowSet();

	//能再次发送验证码的倒计时
	int verifiCodeCountdown();

	wchar_t* getUserName();
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

	//当前窗口类别
	windowstate m_windowState;

	//注册按钮
	Button m_registerBtn;

	//电子邮件输入框
	Button m_emailInputBox;

	//电子邮件输入框提示
	Button m_emailInputTip;

	//验证码输入框
	Button m_verificationCodeInputBox;

	//验证码输入框提示
	Button m_verificationCodeInputTip;

	//发送验证码按钮
	Button m_sendVerificationCodeBtn;

	//存储邮箱地址
	wchar_t m_email[33] = {0};

	//存储验证码
	wchar_t m_verificationCode[33] = { 0 };

	//是否正在输入邮件地址
	bool m_isInputEmail;

	//是否正在输入验证码
	bool m_isInputVerificationCode;

	//注册界面返回按钮
	Button m_RegisterWindowReturnBtn;

	//倒计时
	int m_countdownTime;

	//是否处于发送验证码冷却器
	bool m_isVCodeCooldown;

	//发送验证码冷却定时线程的future
	std::future<int>m_future;

	//存储用户游戏名
	wchar_t m_userName[32];
};





