#include"Control.h"
#include"registerSystem.h"
#include<iostream>
#include<string>
#include <locale>
#include <codecvt>
#include"Alpha.h"
#include"MyInputBox.h"
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

int main3() {


	initgraph(WIDTH, HEIGHT);
	setbkmode(TRANSPARENT);
	setfillcolor(WHITE);
	settextcolor(WHITE);
	wchar_t** xx = new wchar_t* [10];
	for (int i = 0; i < 10; i++)xx[i] = new wchar_t[10];


	wchar_t** a = new wchar_t* [10];
	for (int i = 0; i < 10; i++)a[i] = new wchar_t[10];
	wcscpy(a[0], _T("ÃÜÂë£º"));
	wcscpy(a[1], _T("ÃÜÂëÊÇ"));
	wcscpy(a[2], _T("ÃÜÂëÊÇ¹þ¹þ¹þ"));

	try {
		MyInputBox m(_T("ÇëÊäÈë"), _T("ÌáÊ¾"), 0, a, xx, 3, { 10,10,10 });
		m.play();

	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught in main: " << e.what() << std::endl;
		throw;
	}



	for (int i = 0; i < 10; i++)delete[]xx[i];
	std::cout << 2;
	delete[]xx;
	std::cout << 2;
	for (int i = 0; i < 10; i++)delete[]a[i];
	delete[]a;
	return 0;
}
int main4() {
	initgraph(WIDTH, HEIGHT);
	IMAGE img;
	loadimage(&img, _T("background-1.png"));
	while (1) {
		cleardevice();
		putimage(0, 0, &img);
		ExMessage msg;
		while (peekmessage(&msg)) {
			if (msg.ch == 'a') {
				fillroundrect(0, 0, 200, 200, 10, 10);
				while (1) {
					std::cout << 1;
				}
			}
			
		}
		Sleep(60);
		std::cout << 0;
	}
}
int main5() {
	initgraph(WIDTH, HEIGHT);
	IMAGE img;
	loadimage(&img, _T("background-1.png"));
	putimage(0, 0, &img);
	rec(WIDTH - 100, HEIGHT - 100, WIDTH, HEIGHT, RGB(0,0, 0), 128);
	while (1);
	ChatModule c;
	while (1) {
		cleardevice();
		putimage(0, 0, &img);
		c.draw();
		
		ExMessage msg;
		while (peekmessage(&msg)) {
			c.inputEvent(&msg);

		}
		Sleep(60);
		
	}
}

int main(){
	
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