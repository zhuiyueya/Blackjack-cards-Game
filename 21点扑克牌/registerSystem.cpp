#include "registerSystem.h"

registerSystem::registerSystem()
{
	loadimage(&m_bg, _T("./background-1.png"));
	
	//��ʼ���˺������
	int accountX = (WIDTH - BTN_SIZE_300_40_W) / 2;
	int accountY = (HEIGHT - BTN_SIZE_300_40_H) / 2  - BTN_SIZE_300_40_H * 2;
	m_accountInputBox = Button(accountX, accountY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//��ʼ���˺���������ʾ
	accountX -= BTN_SIZE_80_40_W * 2;
	m_accountInputTip = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//��ʼ�������������ʾ
	accountY += BTN_SIZE_300_40_H * 2;
	m_pwdInputTip = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//��ʼ�����������
	accountX += BTN_SIZE_80_40_W * 2;
	m_pwdInputBox = Button(accountX, accountY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//��ʼ��ȷ���ύ��ť
	accountY += BTN_SIZE_300_40_H * 2;
	m_confirmSubmitBtn = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	m_isInputAccount = true;

	m_isConfirmSubmit = false;

	m_isInputPwd = false;
}

registerSystem::registerSystem(SOCKET sock)
{
	loadimage(&m_bg, _T("./background-1.png"));

	//��ʼ���˺������
	int accountX = (WIDTH - BTN_SIZE_300_40_W) / 2;
	int accountY = (HEIGHT - BTN_SIZE_300_40_H) / 2 - BTN_SIZE_300_40_H * 2;
	m_accountInputBox = Button(accountX, accountY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//��ʼ���˺���������ʾ
	accountX -= BTN_SIZE_80_40_W * 2;
	m_accountInputTip = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//��ʼ�������������ʾ
	accountY += BTN_SIZE_300_40_H * 2;
	m_pwdInputTip = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//��ʼ�����������
	accountX += BTN_SIZE_80_40_W * 2;
	m_pwdInputBox = Button(accountX,accountY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//��ʼ��ȷ���ύ��ť
	accountY += BTN_SIZE_300_40_H * 2;
	m_confirmSubmitBtn = Button(accountX, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);


	m_isInputAccount = true;

	m_isConfirmSubmit = false;

	m_csock = sock;

	m_isInputPwd = false;
}


//���Ƶ�¼����
void registerSystem::draw()
{
	//���Ʊ���
	putimage(0, 0, &m_bg);

	//�����˺������
	setfillcolor(WHITE);
	setlinecolor(BLACK);
	settextcolor(BLACK);
	m_accountInputBox.draw(m_account, BTN_TEXT_STYLE_LEFT);
	
	//�����˺���������ʾ
	m_accountInputTip.draw(_T("�˺�:"));

	//����ȷ���ύ-��¼��ť
	m_confirmSubmitBtn.draw(_T("��¼"));

	//�������������
	m_pwdInputBox.draw(m_pwd, BTN_TEXT_STYLE_LEFT);

	//����������������ʾ
	m_pwdInputTip.draw(_T("����:"));
	
}

//������¼��������¼�
void registerSystem::mouseEvent()
{
	ExMessage msg;
	while (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			int mx = msg.x;
			int my = msg.y;
			//������˺������
			if (mx > m_accountInputBox.beginX && mx< m_accountInputBox.endX && my>m_accountInputBox.beginY && my < m_accountInputBox.endY) {
				m_isInputAccount = true;
				m_isInputPwd = false;
				//ȷ���Ƿ������Ǵ�������״̬��m_isInputAccount==true�������Ǵ�ʱ�����ٻ��ƹ��|��������ƣ������|������
				int i;
				for (i = 0; i < wcslen(m_account); i++) {
					if (m_account[i] == _T('|')) {
						break;
					}
				}
				if (i == wcslen(m_account))
					swprintf(m_account, _T("%s|"), m_account);

				//ȥ�����������Ĺ��
				for (int i = 0; i < wcslen(m_pwd); i++) {
					if (m_pwd[i] == _T('|')) {
						m_pwd[i] = _T('\0');
						break;
					}
				}
				
			}//��������������
			else if (mx > m_pwdInputBox.beginX && mx<m_pwdInputBox.endX && my>m_pwdInputBox.beginY && my < m_pwdInputBox.endY) {
				m_isInputPwd = true;
				m_isInputAccount = false;

				//ȷ���Ƿ������Ǵ�������״̬��m_isInputPwd==true�������Ǵ�ʱ�����ٻ��ƹ��|��������ƣ������|������
				int i;
				for (i = 0; i < wcslen(m_pwd); i++) {
					if (m_pwd[i] == _T('|')) {
						break;
					}
				}
				if (i == wcslen(m_pwd))
					swprintf(m_pwd, _T("%s|"), m_pwd);

				//ȥ���˺������Ĺ��
				for (int i = 0; i < wcslen(m_account); i++) {
					if (m_account[i] == _T('|')) {
						m_account[i] = _T('\0');
						break;
					}
				}

			}
			else {//�ǵ�������

				m_isInputAccount = false;
				m_isInputPwd = false;
				//ȥ���˺������Ĺ��
				for (int i = 0; i < wcslen(m_account); i++) {
					if (m_account[i] == _T('|')) {
						m_account[i] = _T('\0');
						break;
					}
				}

				//ȥ�����������Ĺ��
				for (int i = 0; i < wcslen(m_pwd); i++) {
					if (m_pwd[i] == _T('|')) {
						m_pwd[i] = _T('\0');
						break;
					}
				}

				//������ύ��ť
				if (mx > m_confirmSubmitBtn.beginX && mx< m_confirmSubmitBtn.endX &&
					my>m_confirmSubmitBtn.beginY && my < m_confirmSubmitBtn.endY&&wcslen(m_account)>0) {
					sendLoginRequest();
				}
			}
		}
		else if (msg.message == WM_CHAR) {
			//���������˺�
			if (m_isInputAccount) {
				//����Ϊ����(backspace)ʱ
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
				//����Ϊ��ͨ�ַ�ʱ
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
			//������������
			else if (m_isInputPwd) {
				//����Ϊ����(backspace)ʱ
				if (msg.vkcode == VK_BACK) {
					if (wcslen(m_pwd) > 1) {
						m_pwd[wcslen(m_pwd) - 2] = _T('|');
						m_pwd[wcslen(m_pwd) - 1] = _T('\0');
					}
				}//����Ϊ��ͨ�ַ�ʱ
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

	//���|��˸
	setInputFlashFlag(m_isInputAccount, m_account);
	setInputFlashFlag(m_isInputPwd, m_pwd);

	////���������˺�ʱ|��˸
	//static int flash = 1;
	////|���ɼ�
	//if (m_isInputAccount &&(flash % 30 == 0)) {
	//	for (int i = 0; i < wcslen(m_account); i++) {
	//		if (m_account[i] == _T('|')) {
	//			m_account[i] = _T('\0');
	//			break;
	//		}
	//	}
	//}
	////|�ɼ�
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

//�ж��Ƿ�ȷ���ύ
bool registerSystem::isConfirmSubmitJudge()
{
	return m_isConfirmSubmit;
}

//��¼����ѭ��
wchar_t* registerSystem::play()
{
	settextstyle(20, 0, _T("΢���ź�"));
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


//��ȡ�˺�
wchar_t* registerSystem::getAccount()
{
	return m_account;
}

//���շ�������Ϣ
void registerSystem::recvEvent()
{
	PDU pdu;
	int ret=recv(m_csock, (char*)&pdu, sizeof(pdu), 0);

	//������ʱû����Ϣ�򷵻�
	if (ret <= 0)return;
	
	switch (pdu.msgType) {
		case ENUM_MSG_LOGIN_RESPOND:
		{
			if (strcmp(pdu.msg, LOGIN_SUCCEED) == 0) {
				m_isConfirmSubmit = true;
				std::cout << "��¼�ɹ�\n";
			}
			else if (strcmp(pdu.msg, LOGIN_FAILED) == 0) {
				MessageBox(NULL, _T("��¼ʧ��:�˺�/�������"), _T("��¼"), MB_OK);
			}
			break;
		}
		default:
			break;
	}
}

//���͵�¼����
void registerSystem::sendLoginRequest()
{
	PDU pdu;
	pdu.msgType = ENUM_MSG_LOGIN_REQUEST;
	//��wchar_tת��Ϊchar
	wcstombs(pdu.msg, m_account, sizeof(pdu.msg));
	wcstombs(pdu.msg+32, m_pwd, sizeof(pdu.msg));
	send(m_csock, (char*)&pdu, sizeof(pdu), 0);

}

//��������ʱ���|��˸
void registerSystem::setInputFlashFlag(bool isInput, wchar_t* content)
{
	
	//���������˺�ʱ|��˸
	static int flash = 1;

	//һ�߽���������һ�߽���ż��
	//if(isInput==m_isInputAccount)std::cout << flash;

	//|���ɼ�
	if (isInput && (flash % 30 == 0)||(flash%29==0)) {
		for (int i = 0; i < wcslen(content); i++) {
			if (content[i] == _T('|')) {
				
				content[i] = _T('\0');
				break;
			}
		}
	}
	//|�ɼ�
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
	//��ֹԽ�磬����һֱѭ��
	if (flash == INT_MAX)flash = 1;
}

//��ȡ����
wchar_t* registerSystem::getPwd()
{
	return m_pwd;
}




