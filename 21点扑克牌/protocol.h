#pragma once
#include<graphics.h>
#include<vector>
#include<string>

//-------------��Ҫ�޸ĵ�����-----------------

#define SERVER_IP "192.168.183.129"//������IP��ַ

//-------------��Ҫ�޸ĵ�����-----------------

#define MUSIC_PATH_MAIN_WINDOW _T("mainMusic1.mp3")//�����������ļ�·��



#define SERVER_PORT 8888 //�������غ�Ķ˿ں�

#define WIDTH 960 //���ڿ��
#define HEIGHT 530 //���ڸ߶�

#define CARD_NUM 54 //��������

#define CARD_WIDTH 80 //���ƿ��ƵĿ��
#define CARD_HEIGHT 105 //���ƿ��Ƶĸ߶�

#define PLAYER_NUM 3 //ͬ���������

#define BTN_SIZE_100_40_H 40
#define BTN_SIZE_100_40_W 100
#define BTN_SIZE_180_60_H 60
#define BTN_SIZE_180_60_W 180
#define BTN_SIZE_300_40_H 40
#define BTN_SIZE_300_40_W 300
#define BTN_SIZE_80_40_H 40
#define BTN_SIZE_80_40_W 80
#define BTN_SIZE_40_40_W 40
#define BTN_SIZE_40_40_H 40

#define BTN_SIZE_LOGIN_REGISTER_INPUX_BOX_W  BTN_SIZE_300_40_W//��¼ע����������Ĵ�С
#define BTN_SIZE_LOGIN_REGISTER_INPUX_BOX_H  BTN_SIZE_300_40_H

#define GAP_TEXT_CURSOR_W 10//��������ı��͹��֮��ļ�϶

#define BTN_SIZE_CANCEL_MATCH_W BTN_SIZE_100_40_W//ƥ����淵�ذ�ť��ȡ��ƥ�䣩
#define BTN_SIZE_CANCEL_MATCH_H BTN_SIZE_100_40_H

#define BTN_SIZE_MATCH_W BTN_SIZE_180_60_W //�������ƥ�䰴ť��С
#define BTN_SIZE_MATCH_H BTN_SIZE_180_60_H

#define BTN_SIZE_USER_NAME_BOX_W BTN_SIZE_100_40_W//��ʾ��������ں��Ӵ�С
#define BTN_SIZE_USER_NAME_BOX_H BTN_SIZE_100_40_H

#define BTN_SIZE_MAIN_OPTION_W BTN_SIZE_100_40_W//���ý�����ѡ�ť��С
#define BTN_SIZE_MAIN_OPTION_H BTN_SIZE_100_40_H

#define BTN_SIZE_SETTING_W BTN_SIZE_40_40_W//���ð�ť��С
#define BTN_SIZE_SETTING_H BTN_SIZE_40_40_H

#define BTN_SIZE_CHANGE_NAME_W BTN_SIZE_100_40_W//�޸����ְ�ť
#define BTN_SIZE_CHANGE_NAME_H BTN_SIZE_100_40_H

#define BTN_SIZE_CHANGE_PWD_W BTN_SIZE_100_40_W//�޸����밴ť
#define BTN_SIZE_CHANGE_PWD_H BTN_SIZE_100_40_H

#define BTN_SIZE_SETTING_RETURN_W BTN_SIZE_100_40_W//���ý��淵�ذ�ť
#define BTN_SIZE_SETTING_RETURN_H BTN_SIZE_100_40_H

#define MATCH_SUCCEED "match succeed"
#define MATCH_FAILED "match fail"

#define GAME_INTERRUPT "game is interrupted"
#define GAME_OVER "game over,winner produce"

#define LOGIN_SUCCEED "login succeed"
#define LOGIN_FAILED "login failed account or pwd wrong"

#define BTN_SKIP_ROUND_SIZE_H BTN_SIZE_100_40_H
#define BTN_SKIP_ROUND_SIZE_W BTN_SIZE_100_40_W

#define DURATION_OF_ONE_ROUND 15

#define REGISTER_SUCCEED "register succeed"//ע��ɹ�
#define REGISTER_FAILED "register failed"//ע��ʧ��

#define VERIFI_CODE_TIMER_ID 100 //������֤����ȴ�Ķ�ʱ��ID
#define VEIFI_CODE_EXPIRE_TIEM_S 60 //��֤�����ʱ�䣬��



//Э�鴫�䵥Ԫ
struct PDU {
	int player;
	int cardId;
	int msgType;
	char msg[64];
	int msgLen;//����
	int mainMsg[];//����
};
//��������
enum procotol {
	
	ENUM_MSG_LOGIN_REQUEST,//��¼����
	ENUM_MSG_LOGIN_RESPOND,//��¼�ظ�

	ENUM_MSG_REGIST_REQUEST,//ע������
	ENUM_MSG_REGIST_RESPOND,//ע��ظ�

	ENUM_MSG_MATCH_REQUEST,//ƥ������
	ENUM_MSG_MATCH_RESPOND,//ƥ��ظ�

	ENUM_MSG_TAKECARD_REQUEST,//ȡ������
	ENUM_MSG_TAKECARD_RESPOND,//ȡ�ƻظ�
	ENUM_MSG_TAKECARD_RESEND,//ȡ��ת��

	ENUM_MSG_MATCH_GETNAME_REQUEST,//ƥ��ɹ����ȡ���������������
	ENUM_MSG_MATCH_GETNAME_RESPOND,//ƥ��ɹ����ȡ����������ֻظ�

	ENUM_MSG_GET_PEER_NAME_REQUEST,//��ȡ���������������
	ENUM_MSG_GET_PEER_NAME_RESPOND,//��ȡ����������ֻظ�

	ENUM_MSG_GAMEOVER_NOTIFY,//��Ϸ����֪ͨ

	ENUM_MSG_UPDATE_DOWNCOUNT_TIME_NOTIFY,//����ʱ����֪ͨ
	ENUM_MSG_UPDATE_DOWNCOUNT_TIME_RESPOND,//����ʱ����֪ͨ�ظ�

	ENUM_MSG_CANCEL_MATCH_REQUEST,//ȡ��ƥ������
	ENUM_MSG_CANCEL_MATCH_RESPOND,//ȡ��ƥ��ظ�

	ENUM_MSG_GET_VERIFICATION_CODE_REQUEST,//��ȡ��֤������
	ENUM_MSG_GET_VERIFICATION_CODE_RESPOND,//��ȡ��֤��ظ�

	ENUM_MSG_CHANGE_USERNAME_REQUEST,//�޸��û�������
	ENUM_MSG_CHANGE_USERNAME_RESPOND,//�޸��û����ش�

	ENUM_MSG_SEND_MESSAGE_REQUEST,//������Ϣ����
	ENUM_MSG_SEND_MESSAGE_RESPOND,//������Ϣ�ظ�
};
//��ǰ��������״̬
enum windowstate {
	WINDOW_STATE_MAIN,//������
	WINDOW_STATE_MATCH,//ƥ�����
	WINDOW_STATE_MATCH_SUCCEED, //ƥ��ɹ�
	WINDOW_STATE_MATCH_FAILED,//ƥ��ʧ��
	WINDOW_STATE_GAME,//��Ϸ����
	WINDOW_STATE_REGISTER,//ע�����
	WINDOW_STATE_LOGIN,//��¼����

};
PDU* mkPDU(int msgLen);

//�ϻ��ư�ť������������Button��
void button(int x,int y,LPCTSTR text,bool isTransparent=false,int width=100,int height=40);

//͸��ͼƬ
void transparentImage(IMAGE* dst, int x, int y, IMAGE* src, UINT transaprentcolor);
