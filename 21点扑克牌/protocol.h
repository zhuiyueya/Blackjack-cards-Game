#pragma once
#include<graphics.h>
#include<vector>
#include<string>
#define WIDTH 960
#define HEIGHT 530
#define CARD_WIDTH 80
#define CARD_HEIGHT 105
#define PLAYER_NUM 3
#define BTN_SIZE_100_40_H 40
#define BTN_SIZE_100_40_W 100
#define BTN_SIZE_180_60_H 60
#define BTN_SIZE_180_60_W 180

#define BTN_SIZE_300_40_H 40
#define BTN_SIZE_300_40_W 300

#define BTN_SIZE_80_40_H 40
#define BTN_SIZE_80_40_W 80

#define MATCH_SUCCEED "match succeed"
#define MATCH_FAILED "match fail"

#define GAME_INTERRUPT "game is interrupted"
#define GAME_OVER "game over,winner produce"

#define LOGIN_SUCCEED "login succeed"
#define LOGIN_FAILED "login failed account or pwd wrong"

#define BTN_SKIP_ROUND_SIZE_H BTN_SIZE_100_40_H
#define BTN_SKIP_ROUND_SIZE_W BTN_SIZE_100_40_W

#define DURATION_OF_ONE_ROUND 15

struct PDU {
	int player;
	int cardId;
	int msgType;
	char msg[64];
	int msgLen;
	int mainMsg[];
};
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
void init();
void button(int x,int y,LPCTSTR text,bool isTransparent=false,int width=100,int height=40);
