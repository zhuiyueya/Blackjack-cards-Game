#include "Setting.h"

#include<iostream>
Setting::Setting()
{
}

Setting::Setting(SOCKET sock, wchar_t* account, wchar_t* userName)
{
	m_account = account;
	m_sock = sock;
	m_userName = userName;
	std::vector<const wchar_t*>optionName = { _T("账户"),_T("音量") };
	std::vector< enum OptionBtnLabel> optionLabel = { ENUM_OPTION_BTN_LABEL_ACCOUNT ,ENUM_OPTION_BTN_LABEL_VOLUMN };
	for (int i = 0; i < optionName.size(); i++) {
		OptionBtn btn(0, BTN_SIZE_MAIN_OPTION_H*(i+2), BTN_SIZE_MAIN_OPTION_W, BTN_SIZE_MAIN_OPTION_H, optionLabel[i], optionName[i]);
		m_mainOptions.push_back(btn);
	}

	m_curMainWindowLabel = ENUM_OPTION_BTN_LABEL_ACCOUNT;

	loadimage(&m_bgImg, _T("./background-1.png"));

	int btnNum = 2;
	int totalW = (btnNum - 1) * 40 + BTN_SIZE_CHANGE_NAME_W + BTN_SIZE_CHANGE_PWD_W;
	int btnX = ((WIDTH - BTN_SIZE_MAIN_OPTION_W) - totalW) / 2 + BTN_SIZE_MAIN_OPTION_W;
	int btnY = 2 * HEIGHT / 3;
	m_changeNameBtn = Button(btnX, btnY, BTN_SIZE_CHANGE_NAME_W, BTN_SIZE_CHANGE_NAME_H);
	m_changePwdBtn = Button(btnX + 40 + BTN_SIZE_CHANGE_NAME_W, btnY, BTN_SIZE_CHANGE_PWD_W, BTN_SIZE_CHANGE_PWD_H);
	m_returnBtn = Button(0, 0, BTN_SIZE_SETTING_RETURN_W, BTN_SIZE_SETTING_RETURN_H);
}

void Setting::draw()
{
	putimage(0, 0, &m_bgImg);
	//绘制主选项
	for (OptionBtn &btn:m_mainOptions) {
		btn.draw();
	}

	//绘制返回按钮
	m_returnBtn.draw(_T("返回"));

	//根据当前所属窗口标签绘制子窗口
	switch (m_curMainWindowLabel) {
		case ENUM_OPTION_BTN_LABEL_ACCOUNT://账户相关
		{
			wchar_t userName[64];
			swprintf(userName, _T("昵称：%s"), m_userName);

			wchar_t account[64];
			swprintf(account, _T("账号：%s"), m_account);

			outtextxy(((WIDTH - BTN_SIZE_MAIN_OPTION_W) - textwidth(account)) / 2+ BTN_SIZE_MAIN_OPTION_W, 
				((HEIGHT - BTN_SIZE_MAIN_OPTION_H) - textheight(userName)) / 2- textheight(account)*2, userName);
			outtextxy(((WIDTH - BTN_SIZE_MAIN_OPTION_W) - textwidth(account)) / 2+BTN_SIZE_MAIN_OPTION_W, 
				((HEIGHT - BTN_SIZE_MAIN_OPTION_H) - textheight(account)) / 2, account);

			m_changeNameBtn.draw(_T("修改昵称"));
			m_changePwdBtn.draw(_T("修改密码"));

			break;
		}
		case ENUM_OPTION_BTN_LABEL_VOLUMN://音量相关
		{

			break;
		}
		default:
			break;
	}

}

//主循环
void Setting::play()
{
	m_isQuit = false;
	while (!m_isQuit) {
		cleardevice();
		inputEvent();
		handleRecv();
		draw();
		FlushBatchDraw();
		Sleep(60);
	}
}

//用户输入事件
void Setting::inputEvent()
{
	ExMessage msg;
	while (peekmessage(&msg)) {
		
		if (msg.message == WM_LBUTTONDOWN) {
			int mx = msg.x;
			int my = msg.y;
			for (OptionBtn& btn : m_mainOptions) {
				if (mx > btn.beginX && mx<btn.endX && my>btn.beginY && my < btn.endY) {
					m_curMainWindowLabel = btn.m_label;
					break;
				}
			}

			//返回按钮
			if (mx > m_returnBtn.beginX && mx< m_returnBtn.endX && my>m_returnBtn.beginY && my < m_returnBtn.endY) {
				m_isQuit = true;
			}

			//分子窗口类型处理输入事件
			switch (m_curMainWindowLabel) {
				case ENUM_OPTION_BTN_LABEL_ACCOUNT:
				{
					//点击了修改昵称按钮
					if (mx > m_changeNameBtn.beginX && mx<m_changeNameBtn.endX && my>m_changeNameBtn.beginY && my < m_changeNameBtn.endY) {
						sendChangeUserName();
					}
					//点击了修改密码按钮
					else if (mx > m_changePwdBtn.beginX && mx< m_changePwdBtn.endX && my>m_changePwdBtn.beginY && my < m_changePwdBtn.endY) {
						
					}
				}
				case ENUM_OPTION_BTN_LABEL_VOLUMN:
				{

				}
			}
		}
	}
}

//处理服务器回复
void Setting::handleRecv()
{
	PDU pdu;
	int ret = recv(m_sock, (char*)&pdu, sizeof(pdu), 0);
	if (ret <= 0)return;
	
	switch (pdu.msgType) {
		case ENUM_MSG_CHANGE_USERNAME_RESPOND:
		{
			//防止进入输入新昵称循环中背景变为纯黑色
			draw();
			setfillstyle(BS_SOLID);
			myMessageBox( _T("修改成功！"), _T("昵称"));
			setfillstyle(BS_NULL);//设置为透明，即绘制图形时不填充
			mbstowcs(m_userName, pdu.msg, 32);
			break;
		}

		

		
	}
}

//发送修改昵称请求
void Setting::sendChangeUserName()
{
	//防止进入输入新昵称循环中背景变为纯黑色
	draw();

	//EndBatchDraw();
	setfillstyle(BS_SOLID);
	wchar_t* newName = new wchar_t[32];
	wchar_t* label = new wchar_t[10];
	wcscpy(label, _T("昵称:"));
	myInputBox(_T("请输入新昵称:"), _T("修改昵称"), MMBT_OK, &label, &newName, 1, { 32 });
	setfillstyle(BS_NULL);//设置为透明，即绘制图形时不填充
	delete[]label;
	

	PDU pdu;
	pdu.msgType = ENUM_MSG_CHANGE_USERNAME_REQUEST;
	wcstombs(pdu.msg, m_account, 32);
	wcstombs(pdu.msg + 32, newName, 32);
	send(m_sock, (char*)&pdu, sizeof(pdu), 0);

	delete[]newName;
}
