#include"Control.h"
#include"registerSystem.h"
#include<iostream>
#include<string>
#include <locale>
#include <codecvt>

//Æô¶¯ÓÎÏ·
void StartGame();
int main1() {
	wchar_t a[32] = _T("323234353");
	wchar_t n[32] = _T("asjdh12");
	
	Setting s(NULL,a,n);
	initgraph(WIDTH, HEIGHT);
	setbkmode(TRANSPARENT);
	BeginBatchDraw();
	s.play();
	return 0;
}
int main2() {
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
	c.drawMainWindowUI();
	c.connectToServer();
	c.login();
	settextstyle(50, 0, _T("Î¢ÈíÑÅºÚ"));
	BeginBatchDraw();
	while (1) {
		cleardevice();
		c.mainWindowMouseEvent();
		c.handleMainWindowRecv();
		c.drawMainWindowUI();
		FlushBatchDraw();
		Sleep(60);
	}
	EndBatchDraw();
	return 0;
}
//Æô¶¯ÓÎÏ·
void StartGame() {
	
	
	/*srand(time(0));
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
	EndBatchDraw();*/
	
	
}