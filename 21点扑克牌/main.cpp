#include"Control.h"
#include"registerSystem.h"
#include<iostream>
#include<string>
#include <locale>
#include <codecvt>
//Æô¶¯ÓÎÏ·
void StartGame();
int main1() {
	registerSystem sys;
	initgraph(WIDTH, HEIGHT);
	setbkmode(TRANSPARENT);
	BeginBatchDraw();
	while (1) {
		cleardevice();
		sys.draw();
		sys.mouseEvent();
		FlushBatchDraw();
		Sleep(60);
	}
}
int main() {

	srand(time(0));
	Control c;
	initgraph(WIDTH, HEIGHT);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, _T("Î¢ÈíÑÅºÚ"));
	c.drawStartGameUI();
	c.connectToServer();
	c.login();
	settextstyle(50, 0, _T("Î¢ÈíÑÅºÚ"));
	BeginBatchDraw();
	while (1) {
		cleardevice();
		c.mainWindowMouseEvent();
		c.handleRecv();
		c.drawStartGameUI();
		FlushBatchDraw();
		Sleep(60);
	}
	EndBatchDraw();
	return 0;
}
//Æô¶¯ÓÎÏ·
void StartGame() {
	
	
	srand(time(0));
	Control c;
	initgraph(WIDTH, HEIGHT);
	c.init();
	c.draw();
	c.connectToServer();
	c.getUserNameInput();
	BeginBatchDraw();
	while (1) {

		cleardevice();
		c.mouseEvent();
		c.handleRecv();
		if (c.IsQuitGame())break;
		c.draw();
		FlushBatchDraw();
		Sleep(60);
	}
	EndBatchDraw();
	
	
}