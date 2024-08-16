#include "registerSystem.h"

registerSystem::registerSystem()
{
	loadimage(&m_bg, _T("./background-1.png"));
	
	//初始化账号输入框
	int accountX = (WIDTH - BTN_SIZE_300_40_W) / 2;
	int accountY = (HEIGHT - BTN_SIZE_300_40_H) / 2  - BTN_SIZE_300_40_H * 2;
	m_accountInputBox = Button(accountX, accountY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//初始化账号输入框的提示
	accountX -= BTN_SIZE_80_40_W * 2;
	m_accountInputTip = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//初始化密码输入框提示
	accountY += BTN_SIZE_300_40_H * 2;
	m_pwdInputTip = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//初始化密码输入框
	accountX += BTN_SIZE_80_40_W * 2;
	m_pwdInputBox = Button(accountX, accountY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//初始化确认提交按钮
	accountY += BTN_SIZE_300_40_H * 2;
	m_confirmSubmitBtn = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	m_isInputAccount = true;

	m_isConfirmSubmit = false;

	m_isInputPwd = false;
}

registerSystem::registerSystem(SOCKET sock)
{
	loadimage(&m_bg, _T("./background-1.png"));

	//初始化账号输入框
	int accountX = (WIDTH - BTN_SIZE_300_40_W) / 2;
	int accountY = (HEIGHT - BTN_SIZE_300_40_H) / 2 - BTN_SIZE_300_40_H * 2;
	m_accountInputBox = Button(accountX, accountY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//初始化账号输入框的提示
	accountX -= BTN_SIZE_80_40_W * 2;
	m_accountInputTip = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//初始化密码输入框提示
	accountY += BTN_SIZE_300_40_H * 2;
	m_pwdInputTip = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//初始化密码输入框
	accountX += BTN_SIZE_80_40_W * 2;
	m_pwdInputBox = Button(accountX,accountY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//初始化确认提交按钮
	accountY += BTN_SIZE_300_40_H * 2;
	m_confirmSubmitBtn = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);


	m_isInputAccount = true;

	m_isConfirmSubmit = false;

	m_csock = sock;

	m_isInputPwd = false;
}


//绘制登录界面
void registerSystem::draw()
{
	//绘制背景
	putimage(0, 0, &m_bg);

	//绘制账号输入框
	setfillcolor(WHITE);
	setlinecolor(BLACK);
	settextcolor(BLACK);
	m_accountInputBox.draw(m_account, BTN_TEXT_STYLE_LEFT);
	
	//绘制账号输入框的提示
	m_accountInputTip.draw(_T("账号:"));

	//绘制确认提交-登录按钮
	m_confirmSubmitBtn.draw(_T("登录"));

	//绘制密码输入框
	m_pwdInputBox.draw(m_pwd, BTN_TEXT_STYLE_LEFT);

	//绘制密码输入框的提示
	m_pwdInputTip.draw(_T("密码:"));
	
}

//监听登录界面鼠标事件
void registerSystem::mouseEvent()
{
	ExMessage msg;
	while (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			int mx = msg.x;
			int my = msg.y;
			//点击了账号输入框
			if (mx > m_accountInputBox.beginX && mx< m_accountInputBox.endX && my>m_accountInputBox.beginY && my < m_accountInputBox.endY) {
				m_isInputAccount = true;
				m_isInputPwd = false;
				//确认是否本来就是处于输入状态（m_isInputAccount==true），若是此时不用再绘制光标|，否则绘制，即添加|进数组
				int i;
				for (i = 0; i < wcslen(m_account); i++) {
					if (m_account[i] == _T('|')) {
						break;
					}
				}
				if (i == wcslen(m_account))
					swprintf(m_account, _T("%s|"), m_account);

				//去掉密码输入框的光标
				for (int i = 0; i < wcslen(m_pwd); i++) {
					if (m_pwd[i] == _T('|')) {
						m_pwd[i] = _T('\0');
						break;
					}
				}
				
			}//点击了密码输入框
			else if (mx > m_pwdInputBox.beginX && mx<m_pwdInputBox.endX && my>m_pwdInputBox.beginY && my < m_pwdInputBox.endY) {
				m_isInputPwd = true;
				m_isInputAccount = false;

				//确认是否本来就是处于输入状态（m_isInputPwd==true），若是此时不用再绘制光标|，否则绘制，即添加|进数组
				int i;
				for (i = 0; i < wcslen(m_pwd); i++) {
					if (m_pwd[i] == _T('|')) {
						break;
					}
				}
				if (i == wcslen(m_pwd))
					swprintf(m_pwd, _T("%s|"), m_pwd);

				//去掉账号输入框的光标
				for (int i = 0; i < wcslen(m_account); i++) {
					if (m_account[i] == _T('|')) {
						m_account[i] = _T('\0');
						break;
					}
				}

			}
			else {//非点击输入框

				m_isInputAccount = false;
				m_isInputPwd = false;
				//去掉账号输入框的光标
				for (int i = 0; i < wcslen(m_account); i++) {
					if (m_account[i] == _T('|')) {
						m_account[i] = _T('\0');
						break;
					}
				}

				//去掉密码输入框的光标
				for (int i = 0; i < wcslen(m_pwd); i++) {
					if (m_pwd[i] == _T('|')) {
						m_pwd[i] = _T('\0');
						break;
					}
				}

				//点击了提交按钮
				if (mx > m_confirmSubmitBtn.beginX && mx< m_confirmSubmitBtn.endX &&
					my>m_confirmSubmitBtn.beginY && my < m_confirmSubmitBtn.endY&&wcslen(m_account)>0) {
					sendLoginRequest();
				}
			}
		}
		else if (msg.message == WM_CHAR) {
			//正在输入账号
			if (m_isInputAccount) {
				//输入为回退(backspace)时
				if(msg.vkcode== VK_BACK){
					if (wcslen(m_account) > 1) {
						/*for (int i = 0; i < wcslen(m_account); i++) {
							if (m_account[i] == _T('|')) {
								m_account[i] = _T('\0');
								break;
							}
						}*/

						m_account[wcslen(m_account) - 2] = _T('|');
						m_account[wcslen(m_account) - 1] = _T('\0');
					}
				}
				//输入为普通字符时
				else {
					if (wcslen(m_account) < 32) {
						for (int i = 0; i < wcslen(m_account); i++) {
							if (m_account[i] == _T('|')) {
								m_account[i] = _T('\0');
								break;
							}
						}

						swprintf(m_account, _T("%s%c|"), m_account, msg.ch);
					}
				}
				
			}
			//正在输入密码
			else if (m_isInputPwd) {
				//输入为回退(backspace)时
				if (msg.vkcode == VK_BACK) {
					if (wcslen(m_pwd) > 1) {
						m_pwd[wcslen(m_pwd) - 2] = _T('|');
						m_pwd[wcslen(m_pwd) - 1] = _T('\0');
					}
				}//输入为普通字符时
				else {
					if (wcslen(m_pwd) < 32) {
						std::cout << wcslen(m_pwd) << std::endl;
						for (int i = 0; i < wcslen(m_pwd); i++) {
							if (m_pwd[i] == _T('|')) {
								m_pwd[i] = _T('\0');
								break;
							}
						}

						swprintf(m_pwd, _T("%s%c|"), m_pwd, msg.ch);
					}
				}
			}
			
			
		}
	}

	//光标|闪烁
	setInputFlashFlag(m_isInputAccount, m_account);
	setInputFlashFlag(m_isInputPwd, m_pwd);

	////设置输入账号时|闪烁
	//static int flash = 1;
	////|不可见
	//if (m_isInputAccount &&(flash % 30 == 0)) {
	//	for (int i = 0; i < wcslen(m_account); i++) {
	//		if (m_account[i] == _T('|')) {
	//			m_account[i] = _T('\0');
	//			break;
	//		}
	//	}
	//}
	////|可见
	//else if(m_isInputAccount&&(flash % 31 != 0&& flash % 32 != 0 && flash % 33 != 0) && flash % 34 != 0 && flash % 35 != 0){
	//	int i;
	//	for (i = 0; i < wcslen(m_account); i++) {
	//		if (m_account[i] == _T('|')) {
	//			break;
	//		}
	//	}
	//	if(i== wcslen(m_account))
	//	swprintf(m_account, _T("%s|"), m_account, msg.ch);
	//}
	//flash++;
	//if (flash == INT_MAX)flash = 1;
}

//判断是否确认提交
bool registerSystem::isConfirmSubmitJudge()
{
	return m_isConfirmSubmit;
}

//登录界面循环
wchar_t* registerSystem::play()
{
	settextstyle(20, 0, _T("微软雅黑"));
	BeginBatchDraw();
	while (1) {
		cleardevice();
		draw();
		mouseEvent();
		recvEvent();
		if (isConfirmSubmitJudge())break;
		FlushBatchDraw();
		Sleep(60);
	}
	EndBatchDraw();
	return m_account;
}


//获取账号
wchar_t* registerSystem::getAccount()
{
	return m_account;
}

//接收服务器信息
void registerSystem::recvEvent()
{
	PDU pdu;
	int ret=recv(m_csock, (char*)&pdu, sizeof(pdu), 0);

	//非阻塞时没有消息则返回
	if (ret <= 0)return;
	
	switch (pdu.msgType) {
		case ENUM_MSG_LOGIN_RESPOND:
		{
			if (strcmp(pdu.msg, LOGIN_SUCCEED) == 0) {
				m_isConfirmSubmit = true;
				std::cout << "登录成功\n";
			}
			else if (strcmp(pdu.msg, LOGIN_FAILED) == 0) {
				MessageBox(NULL, _T("登录失败:账号/密码错误！"), _T("登录"), MB_OK);
			}
			break;
		}
		default:
			break;
	}
}

//发送登录请求
void registerSystem::sendLoginRequest()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_LOGIN_REQUEST;
	//将wchar_t转换为char
	wcstombs(pdu.msg, m_account, sizeof(pdu.msg));
	wcstombs(pdu.msg+32, m_pwd, sizeof(pdu.msg));
	send(m_csock, (char*)&pdu, sizeof(pdu), 0);

}

//设置输入时光标|闪烁
void registerSystem::setInputFlashFlag(bool isInput, wchar_t* content)
{
	
	//设置输入账号时|闪烁
	static int flash = 1;

	//一边仅有奇数，一边仅有偶数
	//if(isInput==m_isInputAccount)std::cout << flash;

	//|不可见
	if (isInput && (flash % 30 == 0)||(flash%29==0)) {
		for (int i = 0; i < wcslen(content); i++) {
			if (content[i] == _T('|')) {
				
				content[i] = _T('\0');
				break;
			}
		}
	}
	//|可见
	else if (isInput && flash % 31 != 0 && flash % 32 != 0 && flash % 33 != 0 && flash % 34 != 0 && flash % 35 != 0&&flash%30!=0) {
		int i;
		for (i = 0; i < wcslen(content); i++) {
			if (content[i] == _T('|')) {
				break;
			}
		}
		if (i == wcslen(content))
			swprintf(content, _T("%s|"), content);
	}
	flash++;
	//防止越界，且能一直循环
	if (flash == INT_MAX)flash = 1;
}

//获取密码
wchar_t* registerSystem::getPwd()
{
	return m_pwd;
}




