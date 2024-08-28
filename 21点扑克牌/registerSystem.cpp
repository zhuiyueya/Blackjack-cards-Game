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

	//��ʼ��ע�ᰴť
	m_registerBtn = Button(accountX+ BTN_SIZE_300_40_W- BTN_SIZE_80_40_W, accountY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);


	int beginX = m_pwdInputBox.beginX;
	int beginY = m_pwdInputBox.beginY;

	//�����ʼ������
	beginY += BTN_SIZE_300_40_H * 2;
	m_emailInputBox = Button(beginX, beginY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//�����ʼ��������ʾ
	beginX -= BTN_SIZE_80_40_W * 2;
	m_emailInputTip = Button(beginX, beginY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//��֤���������ʾ
	beginY += BTN_SIZE_300_40_H * 2;
	m_verificationCodeInputTip = Button(beginX, beginY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//��֤�������
	beginX += BTN_SIZE_80_40_W * 2;
	m_verificationCodeInputBox = Button(beginX, beginY, BTN_SIZE_300_40_W, BTN_SIZE_300_40_H);

	//������֤�밴ť
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

	//��Ϊ��¼����
	if (m_windowState == WINDOW_STATE_LOGIN) {
		//����ȷ���ύ-��¼��ť
		m_confirmSubmitBtn.draw(_T("��¼"));
	}

	//�������������
	m_pwdInputBox.draw(m_pwd, BTN_TEXT_STYLE_LEFT);

	//����������������ʾ
	m_pwdInputTip.draw(_T("����:"));
	
	//����ע�ᰴť
	m_registerBtn.draw(_T("ע��"));

	//��Ϊע�����
	if (m_windowState == WINDOW_STATE_REGISTER) {

		//�����ʼ������
		m_emailInputBox.draw(m_email,BTN_TEXT_STYLE_LEFT);

		//�����ʼ��������ʾ
		m_emailInputTip.draw(_T("�ʼ���ַ:"));

		//������֤�������
		m_verificationCodeInputBox.draw(m_verificationCode, BTN_TEXT_STYLE_LEFT);

		//������֤���������ʾ
		m_verificationCodeInputTip.draw(_T("��֤��:"));

		//�޸ķ�����֤�밴ť��ʾ����
		wchar_t cooldown[16] = {0};
		if (m_isVCodeCooldown) {
			std::future_status ret=m_future.wait_for(std::chrono::seconds(0));
			if (ret== std::future_status::ready) {

				m_countdownTime--;
				if (m_countdownTime == 0) {//����ʱ����
					m_isVCodeCooldown = false;
				}
				else {//����ʱδ����������һ���̼߳�ʱһ��
					m_future = std::async(std::launch::async, &registerSystem::verifiCodeCountdown, this);
				}

				
			}
			swprintf(cooldown, _T("%d"), m_countdownTime);
		}
		else {
			wcscpy(cooldown, _T("����"));
		}

		//���Ʒ�����֤�밴ť
		m_sendVerificationCodeBtn.draw(cooldown);

		//����ע����淵�ذ�ť
		m_RegisterWindowReturnBtn.draw(_T("����"));
	}
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
				if (m_windowState == WINDOW_STATE_REGISTER) {
					m_isInputVerificationCode = false;
					m_isInputEmail = false;
					removeCursor(m_verificationCode);
					removeCursor(m_email);
				}
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
				
			}
			//��������������
			else if (mx > m_pwdInputBox.beginX && mx<m_pwdInputBox.endX && my>m_pwdInputBox.beginY && my < m_pwdInputBox.endY) {
				m_isInputPwd = true;
				m_isInputAccount = false;
				if (m_windowState == WINDOW_STATE_REGISTER) {
					m_isInputVerificationCode = false;
					m_isInputEmail = false;
					removeCursor(m_verificationCode);
					removeCursor(m_email);
				}

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
			//��ע������������������
			else if (m_windowState == WINDOW_STATE_REGISTER && mx > m_emailInputBox.beginX && mx<m_emailInputBox.endX &&
				my>m_emailInputBox.beginY && my < m_emailInputBox.endY) {

				//�л����
				m_isInputPwd = false;
				m_isInputAccount = false;
				m_isInputVerificationCode = false;
				m_isInputEmail = true;

				addCursor(m_email);
				removeCursor(m_account);
				removeCursor(m_pwd);
				removeCursor(m_verificationCode);

			}
			//��ע�����������֤�������
			else if (m_windowState == WINDOW_STATE_REGISTER && mx > m_verificationCodeInputBox.beginX && m_verificationCodeInputBox.endX > mx &&
				my > m_verificationCodeInputBox.beginY && m_verificationCodeInputBox.endY > my) {

				//�л����
				m_isInputPwd = false;
				m_isInputAccount = false;
				m_isInputVerificationCode = true;
				m_isInputEmail = false;

				addCursor(m_verificationCode);

				removeCursor(m_account);
				removeCursor(m_pwd);
				removeCursor(m_email);

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

				if (m_windowState == WINDOW_STATE_REGISTER) {
					m_isInputVerificationCode = false;
					m_isInputEmail = false;
					removeCursor(m_verificationCode);
					removeCursor(m_email);
				}

				//�ڵ�¼���������ύ��ť
				if (m_windowState==WINDOW_STATE_LOGIN&&mx > m_confirmSubmitBtn.beginX && mx< m_confirmSubmitBtn.endX &&
					my>m_confirmSubmitBtn.beginY && my < m_confirmSubmitBtn.endY&&wcslen(m_account)>0) {
					
					sendLoginRequest();
				}
				//�����ע�ᰴť
				else if (mx > m_registerBtn.beginX && mx< m_registerBtn.endX && my>m_registerBtn.beginY && my < m_registerBtn.endY) {
					//����ǰ����Ϊ��¼���ڣ�����ת��ע�ᴰ��
					if (m_windowState == WINDOW_STATE_LOGIN) {
						initRegisterWindow();
						m_windowState = WINDOW_STATE_REGISTER;
					}
					//����ǰ����Ϊע�ᴰ�ڣ������ע���˺ţ����������������
					else if (m_windowState == WINDOW_STATE_REGISTER) {
						
						sendRegisterRequest();
					}
				}
				//��ע���˺Ž������˷�����֤�밴ť
				else if (m_windowState == WINDOW_STATE_REGISTER && mx > m_sendVerificationCodeBtn.beginX && mx< m_sendVerificationCodeBtn.endX &&
					my>m_sendVerificationCodeBtn.beginY && my < m_sendVerificationCodeBtn.endY&&!m_isVCodeCooldown) {
						
					if (sendGetVerificationCodeRequest()) {
						m_countdownTime = VEIFI_CODE_EXPIRE_TIEM_S;
						m_isVCodeCooldown = true;
						m_future = std::async(std::launch::async, &registerSystem::verifiCodeCountdown, this);
					}
					
				}
				//��ע��������˷��ذ�ť
				else if (m_windowState == WINDOW_STATE_REGISTER && mx > m_RegisterWindowReturnBtn.beginX && mx< m_RegisterWindowReturnBtn.endX &&
					my>m_RegisterWindowReturnBtn.beginY && my < m_RegisterWindowReturnBtn.endY) {
					resetLoginWindowSet();
					m_windowState = WINDOW_STATE_LOGIN;

				}



			}
		}
		
		//�¼�Ϊ�����¼�ʱ
		else if (msg.message == WM_CHAR) {
			std::cout << 1;
			//���������˺�
			if (m_isInputAccount) {
				//����Ϊ����(backspace)ʱ
				if (msg.vkcode == VK_BACK) {
					delOldChar(m_account);
				}//����Ϊ��ͨ�ַ�ʱ
				else {
					addNewChar(m_account, msg.ch);
				}
				////����Ϊ����(backspace)ʱ
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
				////����Ϊ��ͨ�ַ�ʱ
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
			//������������
			else if (m_isInputPwd) {
				//����Ϊ����(backspace)ʱ
				if (msg.vkcode == VK_BACK) {
					delOldChar(m_pwd);
				}//����Ϊ��ͨ�ַ�ʱ
				else {
					addNewChar(m_pwd, msg.ch);
				}
				//����Ϊ����(backspace)ʱ
				//if (msg.vkcode == VK_BACK) {
				//	if (wcslen(m_pwd) > 1) {
				//		m_pwd[wcslen(m_pwd) - 2] = _T('|');
				//		m_pwd[wcslen(m_pwd) - 1] = _T('\0');
				//	}
				//}//����Ϊ��ͨ�ַ�ʱ
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
			//������������
			else if (m_isInputEmail) {
				//����Ϊ����(backspace)ʱ
				if (msg.vkcode == VK_BACK) {
					delOldChar(m_email);
				}//����Ϊ��ͨ�ַ�ʱ
				else {
					addNewChar(m_email, msg.ch);
				}
			}
			//����������֤��
			else if (m_isInputVerificationCode) {
				//����Ϊ����(backspace)ʱ
				if (msg.vkcode == VK_BACK) {
					delOldChar(m_verificationCode);
				}//����Ϊ��ͨ�ַ�ʱ
				else {
					addNewChar(m_verificationCode, msg.ch);
				}
			}
			
		}
	}

	//���|��˸
	setInputFlashFlag(m_isInputAccount, m_account);
	setInputFlashFlag(m_isInputPwd, m_pwd);
	if (m_windowState == WINDOW_STATE_REGISTER) {
		setInputFlashFlag(m_isInputEmail, m_email);
		setInputFlashFlag(m_isInputVerificationCode, m_verificationCode);
	}
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
				mbstowcs(m_userName, pdu.msg + 32, 32);
				std::wcout << "userName:" << m_userName << std::endl;
			}
			else if (strcmp(pdu.msg, LOGIN_FAILED) == 0) {
				MessageBox(FindWindowW(NULL, _T("21���˿���")), _T("��¼ʧ��:�˺�/�������"), _T("��¼"), MB_OK);
			}
			break;
		}
		case ENUM_MSG_REGIST_RESPOND:
		{
			if (strcmp(pdu.msg, REGISTER_SUCCEED)==0) {
				MessageBox(FindWindowW(NULL, _T("21���˿���")), _T("ע��ɹ���"), _T("ע��"), NULL);
				resetLoginWindowSet();
				m_windowState = WINDOW_STATE_LOGIN;
			}
			else if (strcmp(pdu.msg, REGISTER_FAILED) == 0) {
				MessageBox(FindWindowW(NULL, _T("21���˿���")), _T("ע��ʧ�ܣ�"), NULL, NULL);
				resetLoginWindowSet();
				m_windowState = WINDOW_STATE_LOGIN;
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

//���л�Ϊע�����ʱ�ĳ�ʼ��
void registerSystem::initRegisterWindow()
{
	int beginX = m_verificationCodeInputBox.beginX;
	int beginY = m_verificationCodeInputBox.beginY;

	//ע�ᰴť
	beginY+= BTN_SIZE_300_40_H * 2;
	m_registerBtn = Button(beginX+ BTN_SIZE_300_40_W- BTN_SIZE_80_40_W, beginY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//��մ洢����Ϣ
	wcsset(m_account, _T('\0'));
	wcsset(m_pwd, _T('\0'));
	wcsset(m_email, _T('\0'));
	wcsset(m_verificationCode, _T('\0'));
}

//Ϊ�������������ӹ��
void registerSystem::addCursor(wchar_t* content)
{
	//ȷ���Ƿ������Ǵ�������״̬��content==true�������Ǵ�ʱ�����ٻ��ƹ��|��������ƣ������|������
	int i;
	for (i = 0; i < wcslen(content); i++) {
		if (content[i] == _T('|')) {
			break;
		}
	}
	if (i == wcslen(content))
		swprintf(content, _T("%s|"), content);
}

//Ϊ��������������Ƴ����
void registerSystem::removeCursor(wchar_t* content)
{
	//ȥ��content�����Ĺ��
	for (int i = 0; i < wcslen(content); i++) {
		if (content[i] == _T('|')) {
			content[i] = _T('\0');
			break;
		}
	}
}

//��content��������ַ�
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

//��content���Ƴ����ַ�
void registerSystem::delOldChar(wchar_t* content)
{
	if (wcslen(content) > 1) {
		content[wcslen(content) - 2] = _T('|');
		content[wcslen(content) - 1] = _T('\0');
	}
}

//����ע������
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

//���ͻ�ȡ��֤������
bool registerSystem::sendGetVerificationCodeRequest()
{
	if (wcslen(m_email) <= 0) {
		
		MessageBox(FindWindowW(NULL, _T("21���˿���")), _T("�ʼ���ַ����Ϊ�գ�"), NULL, NULL);
		return false;
	}
	if (wcslen(m_email) <= 7|| wcscmp(_T("@qq.com"), m_email + (wcslen(m_email) - 7)) != 0) {
		MessageBox(FindWindowW(NULL, _T("21���˿���")), _T("��������ȷ���ʼ���ַ��ʽ(��@qq.com��β)��"), NULL, NULL);
		return false;
	}
	

	PDU pdu;
	pdu.msgType = ENUM_MSG_GET_VERIFICATION_CODE_REQUEST;
	wcstombs(pdu.msg, m_email, 32);
	send(m_csock, (char*)&pdu, sizeof(pdu), 0);
	return true;
}

//��ע����淵�ص�¼����ʱ����һЩ����
void registerSystem::resetLoginWindowSet()
{
	int beginX = m_pwdInputBox.beginX;
	int beginY = m_pwdInputBox.beginY;

	//ע�ᰴť
	beginY += BTN_SIZE_300_40_H * 2;
	m_registerBtn = Button(beginX + BTN_SIZE_300_40_W - BTN_SIZE_80_40_W, beginY, BTN_SIZE_80_40_W, BTN_SIZE_80_40_H);

	//�˳�ע�����ʱ����Ϊ����ȴ�ڣ�ȷ���ٴν���ע�����ʱδ���е���ʱ
	m_isVCodeCooldown = false;
}

//���ٴη�����֤��ĵ���ʱ
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









