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

	//初始化注册按钮
	m_registerBtn = Button(accountX+ BTN_SIZE_300_40_W- BTN_SIZE_80_40_W, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);


	int beginX = m_pwdInputBox.beginX;
	int beginY = m_pwdInputBox.beginY;

	//电子邮件输入框
	beginY += BTN_SIZE_300_40_H * 2;
	m_emailInputBox = Button(beginX, beginY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//电子邮件输入框提示
	beginX -= BTN_SIZE_80_40_W * 2;
	m_emailInputTip = Button(beginX, beginY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//验证码输入框提示
	beginY += BTN_SIZE_300_40_H * 2;
	m_verificationCodeInputTip = Button(beginX, beginY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//验证码输入框
	beginX += BTN_SIZE_80_40_W * 2;
	m_verificationCodeInputBox = Button(beginX, beginY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//发送验证码按钮
	beginX += BTN_SIZE_300_40_W;
	m_sendVerificationCodeBtn = Button(beginX, beginY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	m_isInputAccount = true;

	m_isConfirmSubmit = false;

	m_csock = sock;

	m_isInputPwd = false;

	m_windowState = WINDOW_STATE_LOGIN;

	m_isInputVerificationCode = false;

	m_isInputEmail = false;

	m_RegisterWindowReturnBtn = Button(0, 0, BTN_SIZE_100_40_W, BTN_SIZE_100_40_H);

	m_isVCodeCooldown = false;
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

	//若为登录界面
	if (m_windowState == WINDOW_STATE_LOGIN) {
		//绘制确认提交-登录按钮
		m_confirmSubmitBtn.draw(_T("登录"));
	}

	//绘制密码输入框
	m_pwdInputBox.draw(m_pwd, BTN_TEXT_STYLE_LEFT);

	//绘制密码输入框的提示
	m_pwdInputTip.draw(_T("密码:"));
	
	//绘制注册按钮
	m_registerBtn.draw(_T("注册"));

	//若为注册界面
	if (m_windowState == WINDOW_STATE_REGISTER) {

		//绘制邮件输入框
		m_emailInputBox.draw(m_email,BTN_TEXT_STYLE_LEFT);

		//绘制邮件输入框提示
		m_emailInputTip.draw(_T("邮件地址:"));

		//绘制验证码输入框
		m_verificationCodeInputBox.draw(m_verificationCode, BTN_TEXT_STYLE_LEFT);

		//绘制验证码输入框提示
		m_verificationCodeInputTip.draw(_T("验证码:"));

		//修改发送验证码按钮显示内容
		wchar_t cooldown[16] = {0};
		if (m_isVCodeCooldown) {
			std::future_status ret=m_future.wait_for(std::chrono::seconds(0));
			if (ret== std::future_status::ready) {

				m_countdownTime--;
				if (m_countdownTime == 0) {//倒计时结束
					m_isVCodeCooldown = false;
				}
				else {//倒计时未结束开启新一轮线程计时一秒
					m_future = std::async(std::launch::async, &registerSystem::verifiCodeCountdown, this);
				}

				
			}
			swprintf(cooldown, _T("%d"), m_countdownTime);
		}
		else {
			wcscpy(cooldown, _T("发送"));
		}

		//绘制发送验证码按钮
		m_sendVerificationCodeBtn.draw(cooldown);

		//绘制注册界面返回按钮
		m_RegisterWindowReturnBtn.draw(_T("返回"));
	}
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
				if (m_windowState == WINDOW_STATE_REGISTER) {
					m_isInputVerificationCode = false;
					m_isInputEmail = false;
					removeCursor(m_verificationCode);
					removeCursor(m_email);
				}
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
				
			}
			//点击了密码输入框
			else if (mx > m_pwdInputBox.beginX && mx<m_pwdInputBox.endX && my>m_pwdInputBox.beginY && my < m_pwdInputBox.endY) {
				m_isInputPwd = true;
				m_isInputAccount = false;
				if (m_windowState == WINDOW_STATE_REGISTER) {
					m_isInputVerificationCode = false;
					m_isInputEmail = false;
					removeCursor(m_verificationCode);
					removeCursor(m_email);
				}

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
			//在注册界面点击了邮箱输入框
			else if (m_windowState == WINDOW_STATE_REGISTER && mx > m_emailInputBox.beginX && mx<m_emailInputBox.endX &&
				my>m_emailInputBox.beginY && my < m_emailInputBox.endY) {

				//切换光标
				m_isInputPwd = false;
				m_isInputAccount = false;
				m_isInputVerificationCode = false;
				m_isInputEmail = true;

				addCursor(m_email);
				removeCursor(m_account);
				removeCursor(m_pwd);
				removeCursor(m_verificationCode);

			}
			//在注册界面点击了验证码输入框
			else if (m_windowState == WINDOW_STATE_REGISTER && mx > m_verificationCodeInputBox.beginX && m_verificationCodeInputBox.endX > mx &&
				my > m_verificationCodeInputBox.beginY && m_verificationCodeInputBox.endY > my) {

				//切换光标
				m_isInputPwd = false;
				m_isInputAccount = false;
				m_isInputVerificationCode = true;
				m_isInputEmail = false;

				addCursor(m_verificationCode);

				removeCursor(m_account);
				removeCursor(m_pwd);
				removeCursor(m_email);

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

				if (m_windowState == WINDOW_STATE_REGISTER) {
					m_isInputVerificationCode = false;
					m_isInputEmail = false;
					removeCursor(m_verificationCode);
					removeCursor(m_email);
				}

				//在登录界面点击了提交按钮
				if (m_windowState==WINDOW_STATE_LOGIN&&mx > m_confirmSubmitBtn.beginX && mx< m_confirmSubmitBtn.endX &&
					my>m_confirmSubmitBtn.beginY && my < m_confirmSubmitBtn.endY&&wcslen(m_account)>0) {
					
					sendLoginRequest();
				}
				//点击了注册按钮
				else if (mx > m_registerBtn.beginX && mx< m_registerBtn.endX && my>m_registerBtn.beginY && my < m_registerBtn.endY) {
					//若当前窗口为登录窗口，则跳转到注册窗口
					if (m_windowState == WINDOW_STATE_LOGIN) {
						initRegisterWindow();
						m_windowState = WINDOW_STATE_REGISTER;
					}
					//若当前窗口为注册窗口，则进行注册账号，向服务器发送请求
					else if (m_windowState == WINDOW_STATE_REGISTER) {
						
						sendRegisterRequest();
					}
				}
				//在注册账号界面点击了发送验证码按钮
				else if (m_windowState == WINDOW_STATE_REGISTER && mx > m_sendVerificationCodeBtn.beginX && mx< m_sendVerificationCodeBtn.endX &&
					my>m_sendVerificationCodeBtn.beginY && my < m_sendVerificationCodeBtn.endY&&!m_isVCodeCooldown) {
						
					if (sendGetVerificationCodeRequest()) {
						m_countdownTime = VEIFI_CODE_EXPIRE_TIEM_S;
						m_isVCodeCooldown = true;
						m_future = std::async(std::launch::async, &registerSystem::verifiCodeCountdown, this);
					}
					
				}
				//在注册界面点击了返回按钮
				else if (m_windowState == WINDOW_STATE_REGISTER && mx > m_RegisterWindowReturnBtn.beginX && mx< m_RegisterWindowReturnBtn.endX &&
					my>m_RegisterWindowReturnBtn.beginY && my < m_RegisterWindowReturnBtn.endY) {
					resetLoginWindowSet();
					m_windowState = WINDOW_STATE_LOGIN;

				}



			}
		}
		
		//事件为键盘事件时
		else if (msg.message == WM_CHAR) {
			std::cout << 1;
			//正在输入账号
			if (m_isInputAccount) {
				//输入为回退(backspace)时
				if (msg.vkcode == VK_BACK) {
					delOldChar(m_account);
				}//输入为普通字符时
				else {
					addNewChar(m_account, msg.ch);
				}
				////输入为回退(backspace)时
				//if(msg.vkcode== VK_BACK){
				//	if (wcslen(m_account) > 1) {
				//		/*for (int i = 0; i < wcslen(m_account); i++) {
				//			if (m_account[i] == _T('|')) {
				//				m_account[i] = _T('\0');
				//				break;
				//			}
				//		}*/
				//
				//		m_account[wcslen(m_account) - 2] = _T('|');
				//		m_account[wcslen(m_account) - 1] = _T('\0');
				//	}
				//}
				////输入为普通字符时
				//else {
				//	if (wcslen(m_account) < 32) {
				//		for (int i = 0; i < wcslen(m_account); i++) {
				//			if (m_account[i] == _T('|')) {
				//				m_account[i] = _T('\0');
				//				break;
				//			}
				//		}
				//
				//		swprintf(m_account, _T("%s%c|"), m_account, msg.ch);
				//	}
				//}
				
			}
			//正在输入密码
			else if (m_isInputPwd) {
				//输入为回退(backspace)时
				if (msg.vkcode == VK_BACK) {
					delOldChar(m_pwd);
				}//输入为普通字符时
				else {
					addNewChar(m_pwd, msg.ch);
				}
				//输入为回退(backspace)时
				//if (msg.vkcode == VK_BACK) {
				//	if (wcslen(m_pwd) > 1) {
				//		m_pwd[wcslen(m_pwd) - 2] = _T('|');
				//		m_pwd[wcslen(m_pwd) - 1] = _T('\0');
				//	}
				//}//输入为普通字符时
				//else {
				//	if (wcslen(m_pwd) < 32) {
				//		std::cout << wcslen(m_pwd) << std::endl;
				//		for (int i = 0; i < wcslen(m_pwd); i++) {
				//			if (m_pwd[i] == _T('|')) {
				//				m_pwd[i] = _T('\0');
				//				break;
				//			}
				//		}
				//
				//		swprintf(m_pwd, _T("%s%c|"), m_pwd, msg.ch);
				//	}
				//}
			}
			//正在输入邮箱
			else if (m_isInputEmail) {
				//输入为回退(backspace)时
				if (msg.vkcode == VK_BACK) {
					delOldChar(m_email);
				}//输入为普通字符时
				else {
					addNewChar(m_email, msg.ch);
				}
			}
			//正在输入验证码
			else if (m_isInputVerificationCode) {
				//输入为回退(backspace)时
				if (msg.vkcode == VK_BACK) {
					delOldChar(m_verificationCode);
				}//输入为普通字符时
				else {
					addNewChar(m_verificationCode, msg.ch);
				}
			}
			
		}
	}

	//光标|闪烁
	setInputFlashFlag(m_isInputAccount, m_account);
	setInputFlashFlag(m_isInputPwd, m_pwd);
	if (m_windowState == WINDOW_STATE_REGISTER) {
		setInputFlashFlag(m_isInputEmail, m_email);
		setInputFlashFlag(m_isInputVerificationCode, m_verificationCode);
	}
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
				mbstowcs(m_userName, pdu.msg + 32, 32);
				std::wcout << "userName:" << m_userName << std::endl;
			}
			else if (strcmp(pdu.msg, LOGIN_FAILED) == 0) {
				MessageBox(FindWindowW(NULL, _T("21点扑克牌")), _T("登录失败:账号/密码错误！"), _T("登录"), MB_OK);
			}
			break;
		}
		case ENUM_MSG_REGIST_RESPOND:
		{
			if (strcmp(pdu.msg, REGISTER_SUCCEED)==0) {
				MessageBox(FindWindowW(NULL, _T("21点扑克牌")), _T("注册成功！"), _T("注册"), NULL);
				resetLoginWindowSet();
				m_windowState = WINDOW_STATE_LOGIN;
			}
			else if (strcmp(pdu.msg, REGISTER_FAILED) == 0) {
				MessageBox(FindWindowW(NULL, _T("21点扑克牌")), _T("注册失败！"), NULL, NULL);
				resetLoginWindowSet();
				m_windowState = WINDOW_STATE_LOGIN;
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

//当切换为注册界面时的初始化
void registerSystem::initRegisterWindow()
{
	int beginX = m_verificationCodeInputBox.beginX;
	int beginY = m_verificationCodeInputBox.beginY;

	//注册按钮
	beginY+= BTN_SIZE_300_40_H * 2;
	m_registerBtn = Button(beginX+ BTN_SIZE_300_40_W- BTN_SIZE_80_40_W, beginY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//清空存储的信息
	wcsset(m_account, _T('\0'));
	wcsset(m_pwd, _T('\0'));
	wcsset(m_email, _T('\0'));
	wcsset(m_verificationCode, _T('\0'));
}

//为鼠标点击的所属添加光标
void registerSystem::addCursor(wchar_t* content)
{
	//确认是否本来就是处于输入状态（content==true），若是此时不用再绘制光标|，否则绘制，即添加|进数组
	int i;
	for (i = 0; i < wcslen(content); i++) {
		if (content[i] == _T('|')) {
			break;
		}
	}
	if (i == wcslen(content))
		swprintf(content, _T("%s|"), content);
}

//为非鼠标点击的所属移除光标
void registerSystem::removeCursor(wchar_t* content)
{
	//去掉content输入框的光标
	for (int i = 0; i < wcslen(content); i++) {
		if (content[i] == _T('|')) {
			content[i] = _T('\0');
			break;
		}
	}
}

//往content内添加新字符
void registerSystem::addNewChar(wchar_t* content, TCHAR newch)
{
	int maxlength=31;
	
	for (int i = 0; i < wcslen(content); i++) {
		if (content[i] == _T('|')) {
			maxlength += 1;
			break;
		}
	}
	
	if (wcslen(content) < maxlength) {
		removeCursor(content);
		swprintf(content, _T("%s%c|"), content, newch);
	}
}

//往content内移除旧字符
void registerSystem::delOldChar(wchar_t* content)
{
	if (wcslen(content) > 1) {
		content[wcslen(content) - 2] = _T('|');
		content[wcslen(content) - 1] = _T('\0');
	}
}

//发送注册请求
void registerSystem::sendRegisterRequest()
{
	PDU* pdu = mkPDU(64);
	pdu->msgType = ENUM_MSG_REGIST_REQUEST;
	wcstombs(pdu->msg, m_account, 32);
	wcstombs(pdu->msg + 32, m_pwd, 32);
	wcstombs((char*)pdu->mainMsg, m_email, 32);
	wcstombs((char*)pdu->mainMsg+32, m_verificationCode, 32);
	std::cout << pdu->msgLen <<"|"<< (char*)(pdu->mainMsg) + 32 << "|" << sizeof(*pdu) << std::endl;
	send(m_csock, (char*)pdu, pdu->msgLen + sizeof(PDU), 0);
	free(pdu);
}

//发送获取验证码请求
bool registerSystem::sendGetVerificationCodeRequest()
{
	if (wcslen(m_email) <= 0) {
		
		MessageBox(FindWindowW(NULL, _T("21点扑克牌")), _T("邮件地址不能为空！"), NULL, NULL);
		return false;
	}
	if (wcslen(m_email) <= 7|| wcscmp(_T("@qq.com"), m_email + (wcslen(m_email) - 7)) != 0) {
		MessageBox(FindWindowW(NULL, _T("21点扑克牌")), _T("请输入正确的邮件地址格式(以@qq.com结尾)！"), NULL, NULL);
		return false;
	}
	

	PDU pdu;
	pdu.msgType = ENUM_MSG_GET_VERIFICATION_CODE_REQUEST;
	wcstombs(pdu.msg, m_email, 32);
	send(m_csock, (char*)&pdu, sizeof(pdu), 0);
	return true;
}

//从注册界面返回登录界面时重置一些设置
void registerSystem::resetLoginWindowSet()
{
	int beginX = m_pwdInputBox.beginX;
	int beginY = m_pwdInputBox.beginY;

	//注册按钮
	beginY += BTN_SIZE_300_40_H * 2;
	m_registerBtn = Button(beginX + BTN_SIZE_300_40_W - BTN_SIZE_80_40_W, beginY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//退出注册界面时设置为非冷却期，确保再次进入注册界面时未进行倒计时
	m_isVCodeCooldown = false;
}

//能再次发送验证码的倒计时
int registerSystem::verifiCodeCountdown()
{
	/*for (int i = 0; i < VEIFI_CODE_EXPIRE_TIEM_S; i++) {
		
		
		m_countdownTime--;
		
		std::cout << m_countdownTime << std::endl;
		
	}*/
	Sleep(1000);
	
	//m_isVCodeCooldown = false;
	return 0;
}

wchar_t* registerSystem::getUserName()
{
	return m_userName;
}









