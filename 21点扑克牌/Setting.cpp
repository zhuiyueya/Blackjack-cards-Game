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
	std::vector<const wchar_t*>optionName = { _T("�˻�"),_T("����") };
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
	//������ѡ��
	for (OptionBtn &btn:m_mainOptions) {
		btn.draw();
	}

	//���Ʒ��ذ�ť
	m_returnBtn.draw(_T("����"));

	//���ݵ�ǰ�������ڱ�ǩ�����Ӵ���
	switch (m_curMainWindowLabel) {
		case ENUM_OPTION_BTN_LABEL_ACCOUNT://�˻����
		{
			wchar_t userName[64];
			swprintf(userName, _T("�ǳƣ�%s"), m_userName);

			wchar_t account[64];
			swprintf(account, _T("�˺ţ�%s"), m_account);

			outtextxy(((WIDTH - BTN_SIZE_MAIN_OPTION_W) - textwidth(account)) / 2+ BTN_SIZE_MAIN_OPTION_W, 
				((HEIGHT - BTN_SIZE_MAIN_OPTION_H) - textheight(userName)) / 2- textheight(account)*2, userName);
			outtextxy(((WIDTH - BTN_SIZE_MAIN_OPTION_W) - textwidth(account)) / 2+BTN_SIZE_MAIN_OPTION_W, 
				((HEIGHT - BTN_SIZE_MAIN_OPTION_H) - textheight(account)) / 2, account);

			m_changeNameBtn.draw(_T("�޸��ǳ�"));
			m_changePwdBtn.draw(_T("�޸�����"));

			break;
		}
		case ENUM_OPTION_BTN_LABEL_VOLUMN://�������
		{

			break;
		}
		default:
			break;
	}

}

//��ѭ��
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

//�û������¼�
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

			//���ذ�ť
			if (mx > m_returnBtn.beginX && mx< m_returnBtn.endX && my>m_returnBtn.beginY && my < m_returnBtn.endY) {
				m_isQuit = true;
			}

			//���Ӵ������ʹ��������¼�
			switch (m_curMainWindowLabel) {
				case ENUM_OPTION_BTN_LABEL_ACCOUNT:
				{
					//������޸��ǳư�ť
					if (mx > m_changeNameBtn.beginX && mx<m_changeNameBtn.endX && my>m_changeNameBtn.beginY && my < m_changeNameBtn.endY) {
						sendChangeUserName();
					}
					//������޸����밴ť
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

//����������ظ�
void Setting::handleRecv()
{
	PDU pdu;
	int ret = recv(m_sock, (char*)&pdu, sizeof(pdu), 0);
	if (ret <= 0)return;
	
	switch (pdu.msgType) {
		case ENUM_MSG_CHANGE_USERNAME_RESPOND:
		{
			//��ֹ�����������ǳ�ѭ���б�����Ϊ����ɫ
			draw();
			setfillstyle(BS_SOLID);
			myMessageBox( _T("�޸ĳɹ���"), _T("�ǳ�"));
			setfillstyle(BS_NULL);//����Ϊ͸����������ͼ��ʱ�����
			mbstowcs(m_userName, pdu.msg, 32);
			break;
		}

		

		
	}
}

//�����޸��ǳ�����
void Setting::sendChangeUserName()
{
	//��ֹ�����������ǳ�ѭ���б�����Ϊ����ɫ
	draw();

	//EndBatchDraw();
	setfillstyle(BS_SOLID);
	wchar_t* newName = new wchar_t[32];
	wchar_t* label = new wchar_t[10];
	wcscpy(label, _T("�ǳ�:"));
	myInputBox(_T("���������ǳ�:"), _T("�޸��ǳ�"), MMBT_OK, &label, &newName, 1, { 32 });
	setfillstyle(BS_NULL);//����Ϊ͸����������ͼ��ʱ�����
	delete[]label;
	

	PDU pdu;
	pdu.msgType = ENUM_MSG_CHANGE_USERNAME_REQUEST;
	wcstombs(pdu.msg, m_account, 32);
	wcstombs(pdu.msg + 32, newName, 32);
	send(m_sock, (char*)&pdu, sizeof(pdu), 0);

	delete[]newName;
}
