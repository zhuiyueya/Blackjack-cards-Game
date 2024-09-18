#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<cstring>
#include <netinet/in.h>
#include<string.h>
#include<climits>
#include<signal.h>
#include<vector>
#include<queue>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>
#include<time.h>
#include<mysql.h>
#include<hiredis/hiredis.h>


//----------------------------需要修改的内容如下--------------------------


#define REDIS_SERVER_IP "127.0.0.1"//redis服务器IP地址
#define REDIS_SERVER_PORT 6379  //redi服务器端口

#define MYSQL_SERVER_IP "127.0.0.1"//mysql服务器的ip地址
#define MYSQL_SERVER_PORT 3306//mysql服务器端口
#define MYSQL_SERVER_ACCOUNT "bamboo"//mysql服务器账号
#define MYSQL_SERVER_PWD "bamboo"//mysql服务器密码
#define MYSQL_SERVER_DATABASE_NAME "card"//mysql服务器用来存储用户信息的数据库名


//----------------------------需要修改的内容如上--------------------------


#define MAX_SERVER_WAIT_QUEUE_SIZE 10//连接请求队列的最大长度，listen的第二个参数

#define MAX_EPOLL_WAIT_HANDLE_NUM_ONCE 1024//epoll_wait函数一次可以返回的最大事件数，为epoll_wait第三参数


#define VERIFI_CODE_EXPIRE_TIME_S 60 //验证码过期时间/秒

#define REGISTER_SUCCEED "register succeed"//注册成功
#define REGISTER_FAILED "register failed"//注册失败

#define PLAYER_NUM 3
#define MATCH_SUCCEED "match succeed"
#define MATCH_FAILED "match fail"
#define GAME_INTERRUPT "game is interrupted"
#define GAME_OVER "game over,winner produce"
#define LOGIN_SUCCEED "login succeed"
#define LOGIN_FAILED "login failed account or pwd wrong"

#define PORT 8888
int nextPORT=PORT+1;//用来给fork出来的子进程用
struct PDU {
	int player;
	int cardId;
	int msgType;
	char msg[64];
	int msgLen;
	int mainMsg[];
};

PDU* mkPDU(int msgLen)
{
	int pduLen = msgLen + sizeof(PDU);
	PDU* pdu = (PDU*)malloc(pduLen);
	pdu->msgLen = msgLen;
	return pdu;
}

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
struct UserInfo{
	struct epoll_event event;
	char name[32];
	int belongId;
	std::vector<int>cards;

};
//存储所有连接用户的fd
std::vector<struct epoll_event>ets;
//存储所有连接用户的名字
std::vector<std::string>user_names;

//正在匹配的用户队列
std::vector<struct UserInfo>matchQ;
//存储匹配完成后同一局的玩家信息
std::vector<struct UserInfo>peerPlayerInfo(PLAYER_NUM);
//记录每轮取牌开始时间
tm beginTime;
//当前取牌序号
int turn=0;//默认一局中第一个开始匹配的先抽牌
//数据库
MYSQL sql;
//redis库，存储邮箱对应验证码
redisContext *redisC;

bool used_card[54];
char user_name[3][32];
//struct epoll_event et[3];
//int relation[3][3]={{2,0,1},{1,2,0},{0,1,2}};
int relation[3][3]={{2,0,0},{0,2,1},{1,1,2}};//每名玩家在其他玩家那里对应的玩家编号,2代表自己

//产生随机卡牌号
int randomCardChoosed(){
	static int card_num = 54;
	if (card_num <= 0)return -1;
	int card_num1;
	while (used_card[(card_num1 = rand() % 54)]);

	used_card[card_num1] = 1;
	card_num--;
	return card_num1;
}

//找出并记录发送请求的玩家序号
int findCurPlayer(struct epoll_event *ev){
	int curPlayer;
	for(int i=0;i<PLAYER_NUM;i++)
		if(peerPlayerInfo[i].event.data.fd==ev->data.fd)
			curPlayer=i;
	return curPlayer;
}

//废弃，倒计时计算有客户端承当
void downcountTime(){
	time_t nowTime;
	time(&nowTime);
	tm *p;
	p=localtime(&nowTime);
	int minute = p->tm_min - beginTime.tm_min;
	int second = p->tm_sec - beginTime.tm_sec;
	if (second < 0) {
		minute--;
		second += 60;
	}
	if(second>=15){
		turn=(turn+1)%(PLAYER_NUM);
		PDU pdu;
		pdu.msgType=ENUM_MSG_UPDATE_DOWNCOUNT_TIME_NOTIFY;
		for(int i=0;i<PLAYER_NUM;i++){
			pdu.player=relation[turn][i];
			send(peerPlayerInfo[i].event.data.fd,(char*)&pdu,sizeof(pdu),0);
		}
		time(&nowTime);
		p=localtime(&nowTime);
		beginTime=*p;
	}
	

}
//判断几号玩家是胜利者
int judgeWinner(){
	std::vector<int>score(PLAYER_NUM,0);
	for(int i=0;i<PLAYER_NUM;i++){
		int A_nums=0;
		for(int &card:peerPlayerInfo[i].cards){
			if(card>=0&&card<=3){
				score[i]+=3;
			}else if(card>=4&&card<=7){
				score[i]+=4;
			}else if(card>=8&&card<=11){
				score[i]+=5;
			}else if(card>=12&&card<=15){
				score[i]+=6;
			}else if(card>=16&&card<=19){
				score[i]+=7;
			}else if(card>=20&&card<=23){
				score[i]+=8;
			}else if(card>=24&&card<=27){
				score[i]+=9;
			}else if(card>=28&&card<=31){
				score[i]+=10;
			}else if(card>=32&&card<=35){
				score[i]+=10;
			}else if(card>=36&&card<=39){
				score[i]+=10;
			}else if(card>=40&&card<=43){
				score[i]+=10;
			}else if(card>=44&&card<=47){
				A_nums++;
			}else if(card>=48&&card<=51){
				score[i]+=2;
			}else if(card>=52&&card<=53){
				score[i]+=0;
			}
		}
		//处理A
		if(A_nums==1){
			if(score[i]+11>21)score[i]+=1;
			else
				score[i]+=11;
		}else if(A_nums==2){
			if(score[i]+12>21)score[i]+=2;
			else
				score[i]+=12;
		}else if(A_nums==3){
			if(score[i]+13>21)score[i]+=3;
			else
				score[i]+=13;
		}else if(A_nums==4){
			if(score[i]+14>21)score[i]+=4;
			else
				score[i]+=14;
		}
	}
	//选出最终赢家
	int res=-1;
	int resindex=-1;//记录赢家的下标，作为返回值
	for(int i=0;i<PLAYER_NUM;i++){
		if(score[i]<=21){
			if(score[i]>res){
				res=score[i];
				resindex=i;
			}
		}
	}
	if(res==-1){
		res=INT_MAX;
		for(int i=0;i<PLAYER_NUM;i++){
			if(score[i]<res){
				res=score[i];
				resindex=i;
			}
		}
	}
	return resindex;
	
}

//处理游戏正常结束
void handleGameOver(){
	PDU pdu;
	pdu.msgType=ENUM_MSG_GAMEOVER_NOTIFY;
	strncpy(pdu.msg,GAME_OVER,32);
	//判断最终胜利方
	pdu.player=judgeWinner();
	//向所有玩家发送获胜玩家在每个玩家中对应的编号
	for(int i=0;i<PLAYER_NUM;i++){
		pdu.player=relation[judgeWinner()][i];
		send(peerPlayerInfo[i].event.data.fd,(char*)&pdu,sizeof(pdu),0);
	}
}


//在数据库中查找msg存储账号对应的名称
void getUserName(char*msg,char *nameBuf){
        MYSQL_RES*res=nullptr;
        MYSQL_ROW row;
	char sentence[256]={0};
	sprintf(sentence,"select * from user where account='%s';",msg);

	 if (mysql_query(&sql, sentence)) {
        	fprintf(stderr, "mysql_query() failed: %s\n", mysql_error(&sql));
        	return;
   	 }

    	res = mysql_store_result(&sql);
  	if (!res) {
        	fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(&sql));
        	return;
    	}

	//找到了该用户
        if(res->row_count>0){
		row=mysql_fetch_row(res);
		sprintf(nameBuf,"%s",row[2]);
        }

        mysql_free_result(res);
}

//子进程-接受事件
void gameRecvEvent(struct epoll_event*ev){
	PDU pdu;
	int ret=recv(ev->data.fd,(void*)&pdu,sizeof(pdu),0);
	switch(pdu.msgType){
		case ENUM_MSG_GET_PEER_NAME_REQUEST ://获取同局其他玩家用户名请求
		{
			char curPlayerName[32];
			strncpy(curPlayerName,pdu.msg,32);
			//遍历peerPlayerInfo，找到对应的,并存储epoll_event和belongId
			for(int i=0;i<PLAYER_NUM;i++){
				if(strcmp(curPlayerName,peerPlayerInfo[i].name)==0&&peerPlayerInfo[i].belongId==-1){
					peerPlayerInfo[i].event=*ev;
					peerPlayerInfo[i].belongId=nextPORT;
					//std::cout<<curPlayerName<<peerPlayerInfo[i].name<<"  join:----------"<<peerPlayerInfo[i].event.data.fd<<std::endl;
				}
			}
			//遍历peerPlayerInfo，看是否所以同局玩家都以发送GET_PEER_NAME请求
			int getPeerCount=0;
			for(int i=0;i<PLAYER_NUM;i++){
				if(peerPlayerInfo[i].belongId!=-1){
					getPeerCount++;
				}
			}
			//满足上述条件时
			if(getPeerCount==PLAYER_NUM){
				PDU respdu;
				respdu.msgType=ENUM_MSG_GET_PEER_NAME_RESPOND;
				//向同一局的每个玩家发送其他玩家名字
				for(int i=0;i<PLAYER_NUM;i++){
					int curfd=peerPlayerInfo[i].event.data.fd;
					int index=0;
					//通知客户端当前取牌玩家
					respdu.player=relation[turn][i];
					for(int j=0;j<PLAYER_NUM;j++){
						if(curfd!=peerPlayerInfo[j].event.data.fd){
							char nameBuf[32]={0};
							//获取账号对应的用户名
							getUserName(peerPlayerInfo[j].name,nameBuf);

							strncpy(respdu.msg+index*32,nameBuf,32);
							index++;
						}
					}
					send(curfd,(char*)&respdu,sizeof(respdu),0);
					
				}

			}
			break;
		}
		case ENUM_MSG_TAKECARD_REQUEST ://取牌请求
		{
			PDU respdu;
			//生成随机牌号
			respdu.cardId=randomCardChoosed();
			if(pdu.msgLen==30)
			std::cout<<"你干嘛\n";
			

			respdu.msgType=ENUM_MSG_TAKECARD_RESPOND;
			send(ev->data.fd,(void*)&respdu,sizeof(respdu),0);
			//找出并记录发送请求的玩家序号
			int curPlayer;
			for(int i=0;i<PLAYER_NUM;i++)
				if(peerPlayerInfo[i].event.data.fd==ev->data.fd)
					curPlayer=i;
			//将牌对应编号存储进数组
			peerPlayerInfo[curPlayer].cards.push_back(respdu.cardId);
			//转发给其他非请求方
			for(int i=0;i<PLAYER_NUM;i++){
				if(peerPlayerInfo[i].event.data.fd!=ev->data.fd){
					respdu.player=relation[curPlayer][i];//找出相应转发方编号
					respdu.msgType=ENUM_MSG_TAKECARD_RESEND;
					send(peerPlayerInfo[i].event.data.fd,(void*)&respdu,sizeof(respdu),0);
				}
			}
			break;
		}
		case ENUM_MSG_UPDATE_DOWNCOUNT_TIME_NOTIFY ://更新倒计时请求
		{
			//抽牌方循环进行
			turn=(turn+1)%(PLAYER_NUM);
			//循环完后处理获胜
			if(turn==0){
				handleGameOver();		
				break;
			}
			PDU pdu;
			pdu.msgType=ENUM_MSG_UPDATE_DOWNCOUNT_TIME_RESPOND;
			//向每名玩家发送更新倒计时回复
			for(int i=0;i<PLAYER_NUM;i++){
				pdu.player=relation[turn][i];
				send(peerPlayerInfo[i].event.data.fd,(char*)&pdu,sizeof(pdu),0);
			}
			break;
		}
		case ENUM_MSG_SEND_MESSAGE_REQUEST:  //发送消息请求               
		{                         
			//std::cout<<"受到拉"<<pdu.msgLen<<std::endl;
			PDU*respdu=mkPDU(pdu.msgLen);                         
			//char *buf=new char[pdu.msgLen];
			//接受pdu的mainMsg部分信息
			int n=recv(ev->data.fd,(void*)(respdu->mainMsg),respdu->msgLen,0);
			//std::cout<<n<<std::endl;
			//memcpy(respdu->mainMsg,buf,pdu.msgLen);
			//std::cout<<"发送了："<<(char*)respdu->mainMsg<<std::endl;
			respdu->msgType=ENUM_MSG_SEND_MESSAGE_RESPOND;
			//转发给所有玩家，包括发送方
			for(int i=0;i<PLAYER_NUM;i++){
				send(peerPlayerInfo[i].event.data.fd,(char*)respdu,sizeof(PDU)+respdu->msgLen,0);
			}
			free(respdu);
			//delete []buf;
			break;
		}
		default:
			break;
	}
}

//处理由于玩家退出导致的无法正常游戏
void handleGameInterrupt(struct epoll_event *ev){
	PDU pdu;
	pdu.msgType=ENUM_MSG_GAMEOVER_NOTIFY;
	strncpy(pdu.msg,GAME_INTERRUPT,32);
	send(ev->data.fd,(char*)&pdu,sizeof(pdu),0);
}

//子进程-匹配成功后与客户端新建立连接并进行通信
void gaming(){

	//初始化与连接数据库
        mysql_init(&sql);
        if(!mysql_real_connect(&sql,"127.0.0.1","bamboo","bamboo","card",3306,NULL,0)){
        	fprintf(stderr, "connection failed: %s\n", mysql_error(&sql));
                mysql_close(&sql);
        	return;
        }

	//取出匹配队列中的三个玩家信息-账号
	for(int i=0;i<3;i++){
		strncpy(peerPlayerInfo[i].name,matchQ.front().name,32);
		matchQ.erase(matchQ.begin());
		peerPlayerInfo[i].belongId=-1;//表未设置event,或未进入对局
	}
	//重置牌
	memset(used_card,0,54);

	//随机种子
	srand(time(0));

	//初始化socket
	int ssock=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in saddr,caddr;
	socklen_t caddr_len=sizeof(caddr);
	saddr.sin_addr.s_addr=htonl(INADDR_ANY);
	saddr.sin_port=htons(nextPORT);
	saddr.sin_family=AF_INET;
	//绑定
	int ret=bind(ssock,(struct sockaddr*)&saddr,sizeof(saddr));
	if(ret==-1){
		std::cout<<"bind error\n";
	}
	ret=listen(ssock,PLAYER_NUM);
	if(ret==-1){
		std::cout<<"listen error\n";
	}
	//创建epoll监听树
	int efd=epoll_create(PLAYER_NUM+1);
	struct epoll_event event,events[PLAYER_NUM+1];
	//将监听fd添加到epoll树
	event.events=EPOLLIN;
	event.data.fd=ssock;
	epoll_ctl(efd,EPOLL_CTL_ADD,ssock,&event);
	//用来判断是否结束当局游戏
	bool isQuitGame=false;
	while(1){
		int ret=epoll_wait(efd,events,4,-1);
		for(int i=0;i<ret;i++){
			if(events[i].events&(EPOLLERR|EPOLLHUP)){//客户端关闭连接或连接发送错误的情况
				//当一方客户端关闭时，应该让其他客户端也终止游戏
				for(int j=0;j<PLAYER_NUM;j++){
					//找出非主动退出的那一方并通知客户端进行处理
					if(peerPlayerInfo[j].event.data.fd!=events[i].data.fd)
					handleGameInterrupt(&peerPlayerInfo[j].event);

					//将与客户端通信的fd从epoll树上摘除
					//std::cout<<"delete:----------"<<peerPlayerInfo[j].event.data.fd<<std::endl;
					int res=epoll_ctl(efd,EPOLL_CTL_DEL,peerPlayerInfo[j].event.data.fd,NULL);
					if(res==-1)perror("gaming epoll_ctl error:");
					//关闭连接
					close(peerPlayerInfo[j].event.data.fd);
					if(res==-1)perror("gaming close error:");
					std::cout<<"故障\n";
					
				}
				isQuitGame=true;
				break;
			}
			else if(events[i].events&EPOLLIN){//客户端数据
				if(events[i].data.fd==ssock){//客户端连接请求
					int fd=accept(ssock,(struct sockaddr*)&caddr,&caddr_len);
					event.events=EPOLLIN;
					event.data.fd=fd;
					epoll_ctl(efd,EPOLL_CTL_ADD,fd,&event);
					//std::cout<<"game connected\n";
				}else{
					gameRecvEvent(&events[i]);
				}
			}
			
		}
		if(isQuitGame)break;
	}
	//从epoll树上移除监听fd
	epoll_ctl(efd,EPOLL_CTL_DEL,ssock,NULL);
	//关闭epoll监听树头节点
	close(efd);
	//关闭监听socket
	close(ssock);

	//关闭数据库
        mysql_close(&sql);

	exit(0);
	
}

//在数据库中查找是否有用户登陆的用户账号和密码
bool findUser(char*msg,char *nameBuf){
        MYSQL_RES*res;
        MYSQL_ROW row;
	char sentence[128]={0};
	sprintf(sentence,"select * from user where account='%s' and pwd='%s';",msg,msg+32);
	//std::cout<<sentence<<std::endl;
        mysql_query(&sql,sentence);
        res=mysql_store_result(&sql);
	//找到了该用户
        if(res->row_count>0){
		row=mysql_fetch_row(res);
		sprintf(nameBuf,"%s",row[2]);
        	mysql_free_result(res);
		return true;
        }
        mysql_free_result(res);
	return false;
}

//处理登陆请求
void handle_login(PDU*pdu,struct epoll_event *ev){
	//若找到成功，存储用户名
	char nameBuf[32];
	//查找是否有指定用户
	bool ret=findUser(pdu->msg,nameBuf);

	PDU respdu;
	respdu.msgType=ENUM_MSG_LOGIN_RESPOND;
	if(ret){//成功
		strcpy(respdu.msg,LOGIN_SUCCEED);
		strcpy(respdu.msg+32,nameBuf);
		std::cout<<pdu->msg<<"---coming"<<std::endl;
	}else{//失败
		strcpy(respdu.msg,LOGIN_FAILED);
	}
	send(ev->data.fd,(char*)&respdu,sizeof(respdu),0);
}


//在数据库中添加新用户的用户账号和密码
bool addUser(PDU*pdu,char *mainMsg){
	char sentence[256]={0};
	char name[32];
	sprintf(name,"user%d",rand()%10000);
	sprintf(sentence,"insert into user(account,pwd,name,email) values('%s','%s','%s','%s');",pdu->msg,pdu->msg+32,name,mainMsg);
	std::cout<<sentence<<std::endl;
	//std::cout<<sentence<<std::endl;
	//该函数执行成功时返回0
        bool ret=mysql_query(&sql,sentence);

	MYSQL_RES*res;
        res=mysql_store_result(&sql);
	if(res){
        	mysql_free_result(res);
	}

	//添加成功
	if(!ret){
		std::cout<<"添加成功\n";
		return true;
	}
	std::cout<<"添加失败\n";
	return false;
}

//处理注册请求
void handle_register(int fd,PDU*pdu,char*mainMsg){
	PDU respdu;
	respdu.msgType=ENUM_MSG_REGIST_RESPOND;
	//判断验证码是否正确
	char comma[128]={0};
	sprintf(comma,"GET %s",mainMsg);
	redisReply *reply=(redisReply*)redisCommand(redisC,comma);
	//std::cout<<comma<<reply->type<<std::endl;
	if(reply && reply->type == REDIS_REPLY_STRING){
		//std::cout<<"verifiCode--:"<<mainMsg+32<<reply->str<<std::endl;
		//判断查询到的验证码是否与用户发送的一样
		if(strcmp(mainMsg+32,reply->str)==0){
			//添加用户到数据库
			bool ret=addUser(pdu,mainMsg);
			if(ret){
				//通知客户端注册成功
				strcpy(respdu.msg,REGISTER_SUCCEED);
				send(fd,(char*)&respdu,sizeof(respdu),0);

				freeReplyObject(reply);

				return;
			}
		}
	}

	freeReplyObject(reply);
	//通知客户端注册失败
	strcpy(respdu.msg,REGISTER_FAILED);
	send(fd,(char*)&respdu,sizeof(respdu),0);

}

//生成随机验证码
int  produceVerificationCode(char *code){
	int ret=rand()%8999+1000;
	sprintf(code,"%d",ret);
	return ret;
}

//处理验证码
void handle_verification(PDU *pdu,int fd){
	//生成验证码
	char code[5];
	produceVerificationCode(code);

	//向redis服务器中添加邮箱地址与对应验证码
	char comma[128]={0};
	sprintf(comma,"SET %s %s",pdu->msg,code);
	redisReply *reply=(redisReply*)redisCommand(redisC,comma);
	freeReplyObject(reply);

	//设置验证码存活期限
	memset(comma,'0',sizeof(comma));
	sprintf(comma,"EXPIRE %s %d",pdu->msg,VERIFI_CODE_EXPIRE_TIME_S);
	reply=(redisReply*)redisCommand(redisC,comma);
	freeReplyObject(reply);

	//开启子进程且让其执行发送验证码的程序
	int ret=fork();
	if(ret==0){
		ret=execl("./sendVCode","./sendVCode",pdu->msg,code,NULL);
		if(ret==-1){
			perror("execl error");
			exit(1);
		}
	}
		
}

//处理修改名字请求
void handle_changeUserName(char*msg,int fd){
	char newName[32];
	char account[32];
	strcpy(account,msg);
	strcpy(newName,msg+32);

	//执行mysql修改命令
	char sequence[128];
	sprintf(sequence,"UPDATE user SET name='%s' WHERE account='%s'",newName,account);
	mysql_query(&sql,sequence);

	MYSQL_RES*res;
        res=mysql_store_result(&sql);
	if(res){
        	mysql_free_result(res);
	}

	PDU respdu;
	respdu.msgType=ENUM_MSG_CHANGE_USERNAME_RESPOND;
	strcpy(respdu.msg,newName);
	send(fd,(char*)&respdu,sizeof(respdu),0);
}

//处理客户端请求
void handle_recv(struct epoll_event *ev){
/*
	PDU*recvpdu=mkPDU(64);
	read(ev->data.fd,(void*)recvpdu,sizeof(PDU)+64);
	PDU pdu;
	pdu.msgType=recvpdu->msgType;
	memcpy((void*)pdu.msg,(void*)recvpdu->msg,sizeof(pdu.msg));
	pdu.player=recvpdu->player;
	pdu.cardId=recvpdu->cardId;
	pdu.msgLen=recvpdu->msgLen;
	std::cout<<recvpdu->mainMsg<<std::endl;
*/	
	//读取客户端信息，不包含mainMsg里的
	PDU pdu;
	read(ev->data.fd,(void*)&pdu,sizeof(pdu));
		
	if(pdu.msgType==ENUM_MSG_LOGIN_REQUEST){//登陆请求
		handle_login(&pdu,ev);
	}
	else if(pdu.msgType==ENUM_MSG_CHANGE_USERNAME_REQUEST)//修改名字请求
	{
		handle_changeUserName(pdu.msg,ev->data.fd);
	}
	else if(pdu.msgType==ENUM_MSG_GET_VERIFICATION_CODE_REQUEST){//获取验证码请求
		handle_verification(&pdu,ev->data.fd);
	}
	else if(pdu.msgType==ENUM_MSG_REGIST_REQUEST){//注册请求
		//读取pdu的mainMsg里的数据
		char *mainMsg=(char*)malloc(pdu.msgLen);
		//memcpy((void*)mainMsg,(void*)recvpdu->mainMsg,sizeof(mainMsg));
		//注意：sizeof(mainMsg)返回了8,即指针大小
		int ret=read(ev->data.fd,(void*)mainMsg,pdu.msgLen);
		//std::cout<<mainMsg<<"|"<<mainMsg+32<<"|"<<ret<<"|"<<pdu.msgLen<<"|"<<sizeof(mainMsg)<<std::endl;

		handle_register(ev->data.fd,&pdu,mainMsg);
		free(mainMsg);
	}
	else if(pdu.msgType==ENUM_MSG_MATCH_REQUEST){//匹配请求
		//将新到的匹配用户加入匹配队列
		struct UserInfo newMatchUser;
		strncpy(newMatchUser.name,pdu.msg,32);
		newMatchUser.event=*ev;
		matchQ.push_back(newMatchUser);
		//std::cout<<"---------------matchQ-size----:"<<matchQ.size()<<std::endl;
		
		//当匹配队列中等待的玩家数量达到PLAYER——NUM后，开启游戏
		if(matchQ.size()>=PLAYER_NUM){
			//开启游戏子进程
			int ret=fork();
			if(ret==-1){//增加子进程失败
			}
			else if(ret==0){//子进程
				//std::cout<<"fork succeed\n";
				gaming();

			}
			else if(ret>0){//父进程
				//向PLAYER_NUM个同局客户端发送MATCH回复，以便客户端知道匹配成功了
				for(int i=0;i<PLAYER_NUM;i++){
					PDU respdu;
					respdu.msgType=ENUM_MSG_MATCH_RESPOND;
				     	strcpy(respdu.msg,MATCH_SUCCEED);
					//附加上游戏子进程的端口号
					char portBuf[32];
					sprintf(portBuf,"%d",nextPORT);
					strcpy(respdu.msg+32,portBuf);
					send(matchQ.front().event.data.fd,(void*)&respdu,sizeof(respdu),0);
					//从匹配队列中清除
					matchQ.erase(matchQ.begin());
				}
				//下一个对局的端口号
				nextPORT++;

			}

		}

	}
	else if(pdu.msgType==ENUM_MSG_CANCEL_MATCH_REQUEST){//取消匹配请求
		//从匹配队列中找出并删除
		for(int i=0;i<matchQ.size();i++){
			if(matchQ[i].event.data.fd==ev->data.fd){
				matchQ.erase(matchQ.begin()+i);
				break;
			}
		}
	}
	else{
		std::cout<<"else\n";
	}

	//free(recvpdu);
}

//捕捉子进程结束的信号，对其进行回收
void recycleChildProcess(int signum){
	int pid;
	int state;
	//pid=wait(&state);
	while((pid=waitpid(-1,&state,WNOHANG)>0)){
		//std::cout<<"state======="<<state<<std::endl;
		std::cout<<"child process recycled--"<<pid<<std::endl;
	}
	
		//std::cout<<"child process recycled--"<<pid<<std::endl;
	/*
	while((pid=wait(&state))!=-1){
		//std::cout<<"state======="<<state<<std::endl;
		std::cout<<"child process recycled--"<<pid<<std::endl;
	}
	*/
}
int main(){
	//注册SIGCHLD信号,回收游戏子进程(当玩家匹配成功进入游戏时服务器会开辟子进程管理游戏）
	struct sigaction act;
	act.sa_handler=recycleChildProcess;
	act.sa_flags=0;
	sigemptyset(&(act.sa_mask));
	sigaction(SIGCHLD,&act,NULL);

	//注册忽略SIGPIPE信号,避免有客户端异常退出导致服务器终止
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );

	//初始化与连接数据库,存储用户账号信息
        mysql_init(&sql);
        if(!mysql_real_connect(&sql,MYSQL_SERVER_IP,MYSQL_SERVER_ACCOUNT,MYSQL_SERVER_PWD,MYSQL_SERVER_DATABASE_NAME,MYSQL_SERVER_PORT,NULL,0)){
        	fprintf(stderr, "connection failed: %s\n", mysql_error(&sql));
                mysql_close(&sql);
        	return 1;
        }

	//连接redis服务器,存储用户注册时发送验证码后邮箱对应的验证码
	redisC=redisConnect(REDIS_SERVER_IP,REDIS_SERVER_PORT);

	//随机种子
	srand(time(0));

	memset(used_card,0,54);

	//创建socket
	int ssock=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in saddr,caddr;
	socklen_t caddr_len=sizeof(caddr);
	saddr.sin_addr.s_addr=htonl(INADDR_ANY);
	saddr.sin_port=htons(PORT);
	saddr.sin_family=AF_INET;
	//绑定
	int ret=bind(ssock,(struct sockaddr*)&saddr,sizeof(saddr));
	if(ret==-1){
		std::cout<<"bind error\n";
	}

	//设置为被动监听
	ret=listen(ssock,MAX_SERVER_WAIT_QUEUE_SIZE);
	if(ret==-1){
		std::cout<<"listen error\n";
	}

	//创建epoll树
	int efd=epoll_create(1024);

	struct epoll_event event,events[MAX_EPOLL_WAIT_HANDLE_NUM_ONCE];
	//把监听fd添加进epoll树
	event.events=EPOLLIN;
	event.data.fd=ssock;
	epoll_ctl(efd,EPOLL_CTL_ADD,ssock,&event);

	int timeout=1000;
	while(1){
		int ret=epoll_wait(efd,events,MAX_EPOLL_WAIT_HANDLE_NUM_ONCE,timeout);
		if(ret==-1){
			perror("epoll_wait:");
		}
		//if(ret!=0)
		//std::cout<<"-----------epoll_wait,return:"<<ret<<std::endl;
		for(int i=0;i<ret;i++){
			if(events[i].events&(EPOLLERR|EPOLLHUP)){//客户端关闭连接或连接发生错误的情况
				//std::cout<<"client close link or error\n";
				//若玩家在排队时强制退出，将其移出匹配队列
				for(int i=0;i<matchQ.size();i++){
					if(matchQ[i].event.data.fd==events[i].data.fd){
						matchQ.erase(matchQ.begin()+i);
						break;
					}
				}
				//将与客户端通信的fd从epoll树上摘除
				int res=epoll_ctl(efd,EPOLL_CTL_DEL,events[i].data.fd,NULL);
				if(res==-1)
					perror("main epoll_ctl fail:");
				//关闭连接
				res=close(events[i].data.fd);
				if(res==-1)
					perror("main close fail:");
			}
			else if(events[i].events&EPOLLIN){//客户端发送数据的情况
				//新客户端连接
				if(events[i].data.fd==ssock){
					//处理连接请求并添加进epoll树
					int fd=accept(ssock,(struct sockaddr*)&caddr,&caddr_len);
					event.events=EPOLLIN;
					event.data.fd=fd;
					epoll_ctl(efd,EPOLL_CTL_ADD,fd,&event);
				}else{//老客户端发送请求
					handle_recv(&events[i]);
				}
			}
			
		}
	}
	//关闭epoll树的头
	close(efd);

	//关闭监听socket
	close(ssock);

	//清除redis连接
	redisFree(redisC);

	//关闭数据库
        mysql_close(&sql);

}
