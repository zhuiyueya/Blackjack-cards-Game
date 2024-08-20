#include "MyMessageBox.h"

#define BTN_WIDTH 60//消息盒子的宽度
#define BTN_HEIGHT 30//消息盒子的高度
#define TOP_MARGIN 5//顶部空白
#define MIDDLE_LINE_MARGIN 20//标题与内容之间的线与空白的宽度
#define TEXT_MIDDLE_MARGIN 10//内容中不同行间的间隔
#define BOTTOM_MARGIN 20//底部空白
#define LEFT_MARGIN 20//左边空白
#define RIGHT_MARGIN 20//右边空白
#define BTN_CONTENT_MIDDLE_MARGIN 20//按钮与内容之间的间隔
MyMessageBox::MyMessageBox(int x, int y, const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType)
{
	setfillcolor(WHITE);
	
	settextcolor(BLACK);

	setlinecolor(BLACK);

	beginX = x;
	beginY = y;

	content = new wchar_t[wcslen(pcontent) + 1];
	wcscpy(content, pcontent);
	title = new wchar_t[wcslen(ptitle) + 1];
	wcscpy(title, ptitle);

	isQuit = false;

	int rfNum = 0;//\n换行符的个数
	int maxW=0;//内容中不同行之间的最大宽度
	int contentLen = wcslen(pcontent);
	wchar_t* preP = content;

	contentSplitPoint.push_back(0);
	//分割content，确定不同行
	for (int i = 0; i < contentLen; i++) {
		if (content[i] == _T('\n')) {
			rfNum++;
			content[i] = _T('\0');
			maxW=max(maxW,textwidth(preP));
			preP = content + i + 1;
			contentSplitPoint.push_back(i+1);
		}
	}
	//若没有换行符，则为最大宽度即整段内容的宽度
	if (rfNum == 0)maxW = textwidth(pcontent);
	

	endX = x + maxW + BTN_WIDTH + LEFT_MARGIN + RIGHT_MARGIN+ BTN_CONTENT_MIDDLE_MARGIN;
	int contentH = textheight(pcontent) * (rfNum + 1) + TEXT_MIDDLE_MARGIN * (rfNum);
	endY = beginY+textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN + BOTTOM_MARGIN;
	/*int tw = textwidth(pcontent);
	if (tw > MAX_MESSAGEBOX_WIDTH) {
		endX = MAX_MESSAGEBOX_WIDTH+BTN_WIDTH;
		int x = tw / MAX_MESSAGEBOX_WIDTH + 1;
		endY = textheight(ptitle) + textheight(pcontent) * x + TEXT_MIDDLE_MARGIN*(x-1) + TOP_MARGIN + MIDDLE_LINE_MARGIN+ BOTTOM_MARGIN;
	}
	else {
		endX = tw + BTN_WIDTH;
		endY= textheight(ptitle)+ textheight(pcontent) + TOP_MARGIN + MIDDLE_LINE_MARGIN + BOTTOM_MARGIN;
	}*/
	
	switch (messageBoxType) {
		case MMBT_OK:
		{
			OptionBtn btn(endX - BTN_WIDTH-RIGHT_MARGIN, beginY + BTN_HEIGHT+ textheight(ptitle)+ TOP_MARGIN + MIDDLE_LINE_MARGIN, BTN_WIDTH, BTN_HEIGHT, MMBT_OK,_T("确定"));
			buttons.push_back(btn);
			break;
		}
		case MMBT_TRY:
		{
			break;
		}
	}
}

MyMessageBox::MyMessageBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType)
{

	setfillcolor(WHITE);

	settextcolor(BLACK);

	setlinecolor(BLACK);

	content = new wchar_t[wcslen(pcontent) + 1];
	wcscpy(content, pcontent);
	title = new wchar_t[wcslen(ptitle) + 1];
	wcscpy(title, ptitle);

	isQuit = false;

	int rfNum = 0;//\n换行符的个数
	int maxW = 0;//内容中不同行之间的最大宽度
	int contentLen = wcslen(pcontent);
	wchar_t* preP = content;

	contentSplitPoint.push_back(0);
	//分割content，确定不同行
	for (int i = 0; i < contentLen; i++) {
		if (content[i] == _T('\n')) {
			rfNum++;
			content[i] = _T('\0');
			maxW = max(maxW, textwidth(preP));
			preP = content + i + 1;
			contentSplitPoint.push_back(i + 1);
		}
	}
	//若没有换行符，则为最大宽度即整段内容的宽度
	if (rfNum == 0)maxW = textwidth(pcontent);


	beginX = (getwidth() - (maxW + BTN_WIDTH + LEFT_MARGIN + RIGHT_MARGIN + BTN_CONTENT_MIDDLE_MARGIN)) / 2;
	int contentH = textheight(pcontent) * (rfNum + 1) + TEXT_MIDDLE_MARGIN * (rfNum);
	beginY = (getheight() - (textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN + BOTTOM_MARGIN)) / 2;

	endX = beginX + maxW + BTN_WIDTH + LEFT_MARGIN + RIGHT_MARGIN + BTN_CONTENT_MIDDLE_MARGIN;
	endY = beginY+textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN + BOTTOM_MARGIN;

	switch (messageBoxType) {
		case MMBT_OK:
		{
			OptionBtn btn(endX - BTN_WIDTH - RIGHT_MARGIN, beginY + BTN_HEIGHT + textheight(ptitle) + TOP_MARGIN + MIDDLE_LINE_MARGIN, BTN_WIDTH, BTN_HEIGHT, MMBT_OK, _T("确定"));
			buttons.push_back(btn);
			break;
		}
		case MMBT_TRY:
		{
			break;
		}
	}

}

MyMessageBox::~MyMessageBox()
{
	delete []content;
	delete[]title;
}

void MyMessageBox::draw()
{
	//背景板
	fillroundrect(beginX, beginY, endX, endY, 10, 10);

	int x = beginX+LEFT_MARGIN;
	int y = TOP_MARGIN+beginY;
	//标题
	outtextxy(x, y, title);
	
	//分割线
	y += textheight(title);
	line(beginX, y + MIDDLE_LINE_MARGIN / 4, endX, y + MIDDLE_LINE_MARGIN / 4);

	//主要内容
	y += MIDDLE_LINE_MARGIN;
	for (int i = 0; i < contentSplitPoint.size(); i++) {
		outtextxy(x, y + textheight(content) * i, content + contentSplitPoint[i]);
		y += TEXT_MIDDLE_MARGIN;
	}

	//按钮
	for (OptionBtn& btn : buttons) {
		btn.draw();
	}

}

void MyMessageBox::inputEvent()
{
	ExMessage msg;
	while (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			int mx = msg.x;
			int my = msg.y;
			//逐个按钮判断是否被点击
			for (OptionBtn& btn : buttons) {
				if (mx > btn.beginX && mx<btn.endX && my>btn.beginY && my < btn.endY) {
					switch (btn.m_label) {
						case MMBT_OK:
						{
							clickedBtnType = MMBT_OK;
							isQuit = true;
							break;
						}
					}
				}
			}
		}
	}
}

int MyMessageBox::play()
{
	draw();
	while (!isQuit) {
		inputEvent();
	}
	return clickedBtnType;
}

int myMessageBox(int x, int y, const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType)
{
	MyMessageBox box(x, y, pcontent, ptitle, messageBoxType);
	return box.play();
}

int myMessageBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType)
{
	
	MyMessageBox box(pcontent, ptitle, messageBoxType);
	return box.play();
}
