#pragma once
#include<graphics.h>
#include<vector>
#include<string>

//-------------需要修改的如下-----------------

#define SERVER_IP "192.168.183.129"//服务器IP地址

//-------------需要修改的如上-----------------

#define MUSIC_PATH_MAIN_WINDOW _T("mainMusic1.mp3")//主界面音乐文件路径



#define SERVER_PORT 8888 //服务器守候的端口号

#define WIDTH 960 //窗口宽度
#define HEIGHT 530 //窗口高度

#define CARD_NUM 54 //卡牌数量

#define CARD_WIDTH 80 //绘制卡牌的宽度
#define CARD_HEIGHT 105 //绘制卡牌的高度

#define PLAYER_NUM 3 //同局玩家数量

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

#define BTN_SIZE_LOGIN_REGISTER_INPUX_BOX_W  BTN_SIZE_300_40_W//登录注册界面输入框的大小
#define BTN_SIZE_LOGIN_REGISTER_INPUX_BOX_H  BTN_SIZE_300_40_H

#define GAP_TEXT_CURSOR_W 10//输入框中文本和光标之间的间隙

#define BTN_SIZE_CANCEL_MATCH_W BTN_SIZE_100_40_W//匹配界面返回按钮（取消匹配）
#define BTN_SIZE_CANCEL_MATCH_H BTN_SIZE_100_40_H

#define BTN_SIZE_MATCH_W BTN_SIZE_180_60_W //主界面的匹配按钮大小
#define BTN_SIZE_MATCH_H BTN_SIZE_180_60_H

#define BTN_SIZE_USER_NAME_BOX_W BTN_SIZE_100_40_W//显示玩家名窗口盒子大小
#define BTN_SIZE_USER_NAME_BOX_H BTN_SIZE_100_40_H

#define BTN_SIZE_MAIN_OPTION_W BTN_SIZE_100_40_W//设置界面主选项按钮大小
#define BTN_SIZE_MAIN_OPTION_H BTN_SIZE_100_40_H

#define BTN_SIZE_SETTING_W BTN_SIZE_40_40_W//设置按钮大小
#define BTN_SIZE_SETTING_H BTN_SIZE_40_40_H

#define BTN_SIZE_CHANGE_NAME_W BTN_SIZE_100_40_W//修改名字按钮
#define BTN_SIZE_CHANGE_NAME_H BTN_SIZE_100_40_H

#define BTN_SIZE_CHANGE_PWD_W BTN_SIZE_100_40_W//修改密码按钮
#define BTN_SIZE_CHANGE_PWD_H BTN_SIZE_100_40_H

#define BTN_SIZE_SETTING_RETURN_W BTN_SIZE_100_40_W//设置界面返回按钮
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

#define REGISTER_SUCCEED "register succeed"//注册成功
#define REGISTER_FAILED "register failed"//注册失败

#define VERIFI_CODE_TIMER_ID 100 //发送验证码冷却的定时器ID
#define VEIFI_CODE_EXPIRE_TIEM_S 60 //验证码过期时间，秒



//协议传输单元
struct PDU {
	int player;
	int cardId;
	int msgType;
	char msg[64];
	int msgLen;//后增
	int mainMsg[];//后增
};
//数据类型
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

	ENUM_MSG_CHANGE_USERNAME_REQUEST,//修改用户名请求
	ENUM_MSG_CHANGE_USERNAME_RESPOND,//修改用户名回答

	ENUM_MSG_SEND_MESSAGE_REQUEST,//发送消息请求
	ENUM_MSG_SEND_MESSAGE_RESPOND,//发送消息回复
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

//老绘制按钮函数，以增加Button类
void button(int x,int y,LPCTSTR text,bool isTransparent=false,int width=100,int height=40);

//透明图片
void transparentImage(IMAGE* dst, int x, int y, IMAGE* src, UINT transaprentcolor);
