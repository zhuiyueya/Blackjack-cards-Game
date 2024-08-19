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
	ENUM_MSG_LOGIN_REQUEST,//登录请求
	ENUM_MSG_LOGIN_RESPOND,//登录回复
	ENUM_MSG_REGIST_REQUEST,//注册请求
	ENUM_MSG_REGIST_RESPOND,//注册回复
	ENUM_MSG_MATCH_REQUEST,//匹配请求
	ENUM_MSG_MATCH_RESPOND,//匹配回复
	ENUM_MSG_TAKECARD_REQUEST,//取牌请求
	ENUM_MSG_TAKECARD_RESPOND,//取牌回复
	ENUM_MSG_TAKECARD_RESEND,//取牌转发
	ENUM_MSG_MATCH_GETNAME_REQUEST,//匹配成功后获取其他玩家名字请求
	ENUM_MSG_MATCH_GETNAME_RESPOND,//匹配成功后获取其他玩家名字回复
	ENUM_MSG_GET_PEER_NAME_REQUEST,//获取其他玩家名字请求
	ENUM_MSG_GET_PEER_NAME_RESPOND,//获取其他玩家名字回复
	ENUM_MSG_GAMEOVER_NOTIFY,//游戏结束通知
	ENUM_MSG_UPDATE_DOWNCOUNT_TIME_NOTIFY,//倒计时更新通知
	ENUM_MSG_UPDATE_DOWNCOUNT_TIME_RESPOND,//倒计时更新通知回复
	ENUM_MSG_CANCEL_MATCH_REQUEST,//取消匹配请求
	ENUM_MSG_CANCEL_MATCH_RESPOND,//取消匹配回复

	ENUM_MSG_GET_VERIFICATION_CODE_REQUEST,//获取验证码请求
	ENUM_MSG_GET_VERIFICATION_CODE_RESPOND,//获取验证码回复


};
//当前所处界面状态
enum windowstate {
	WINDOW_STATE_MAIN,//主界面
	WINDOW_STATE_MATCH,//匹配界面
	WINDOW_STATE_MATCH_SUCCEED, //匹配成功
	WINDOW_STATE_MATCH_FAILED,//匹配失败
	WINDOW_STATE_GAME,//游戏界面
	WINDOW_STATE_REGISTER,//注册界面
	WINDOW_STATE_LOGIN,//登录界面

};
PDU* mkPDU(int msgLen);
void init();
void button(int x,int y,LPCTSTR text,bool isTransparent=false,int width=100,int height=40);
