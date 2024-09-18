#include "MyMessageBox.h"

#define BTN_WIDTH 60//��Ϣ���ӵĿ��
#define BTN_HEIGHT 30//��Ϣ���ӵĸ߶�
#define TOP_MARGIN 5//�����հ�
#define MIDDLE_LINE_MARGIN 20//����������֮�������հ׵Ŀ��
#define TEXT_MIDDLE_MARGIN 10//�����в�ͬ�м�ļ��
#define BOTTOM_MARGIN 20//�ײ��հ�
#define LEFT_MARGIN 20//��߿հ�
#define RIGHT_MARGIN 20//�ұ߿հ�

MyMessageBox::MyMessageBox(int x, int y, const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, LPCTSTR bgImgPath )
{
	//��messageBoxType��ָ�����ͣ���ת��ΪMMBT_OK
	if (messageBoxType < MMBT_OK || messageBoxType >= MMBT_MAX)messageBoxType = MMBT_OK;

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

	int rfNum = 0;//\n���з��ĸ���
	int maxW=0;//�����в�ͬ��֮��������
	int contentLen = wcslen(pcontent);
	wchar_t* preP = content;

	contentSplitPoint.push_back(0);
	//�ָ�content��ȷ����ͬ��
	for (int i = 0; i < contentLen; i++) {
		if (content[i] == _T('\n')) {
			rfNum++;
			content[i] = _T('\0');
			maxW=max(maxW,textwidth(preP));
			preP = content + i + 1;
			contentSplitPoint.push_back(i+1);
		}
	}

	maxW = max(maxW, textwidth(preP));
	//��û�л��з�����Ϊ����ȼ��������ݵĿ��
	if (rfNum == 0)maxW = textwidth(pcontent);
	
	//�жϰ�ť�ܳ����Ƿ���ı���
	int totalBtnW = 0;
	switch (messageBoxType) {
	case MMBT_OK:
	{
		totalBtnW = max(totalBtnW, BTN_WIDTH);
		break;
	}
	case MMBT_TRY:
	{
		totalBtnW = max(totalBtnW, BTN_WIDTH * 3);
		break;
	}
	}
	maxW = max(maxW, totalBtnW);

	endX = beginX + maxW + LEFT_MARGIN + RIGHT_MARGIN;
	int contentH = textheight(pcontent) * (rfNum + 1) + TEXT_MIDDLE_MARGIN * (rfNum+1);
	endY = beginY + textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN + BOTTOM_MARGIN + BTN_HEIGHT;

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
		OptionBtn btn(beginX+(endX-beginX-BTN_WIDTH)/2, endY - BOTTOM_MARGIN - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT,
			MMBR_OK, _T("ȷ��"));
		buttons.push_back(btn);
		break;
	}
	case MMBT_TRY:
	{
		OptionBtn btn(beginX + LEFT_MARGIN, endY - BOTTOM_MARGIN - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT,
			MMBR_TRY, _T("����"));
		buttons.push_back(btn);

		btn = OptionBtn(endX - RIGHT_MARGIN - BTN_WIDTH, endY - BOTTOM_MARGIN - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT,
			MMBR_CANCEL, _T("ȡ��"));
		buttons.push_back(btn);
		break;
	}
	}
	if (bgImgPath != _T(""))
	loadimage(&bgImg, bgImgPath,endX-beginX,endY-beginY );
}

MyMessageBox::MyMessageBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, LPCTSTR bgImgPath)
{
	//��messageBoxType��ָ�����ͣ���ת��ΪMMBT_OK
	if (messageBoxType < MMBT_OK || messageBoxType >= MMBT_MAX)messageBoxType = MMBT_OK;

	setfillcolor(WHITE);

	settextcolor(BLACK);

	setlinecolor(BLACK);

	content = new wchar_t[wcslen(pcontent) + 1];
	wcscpy(content, pcontent);
	title = new wchar_t[wcslen(ptitle) + 1];
	wcscpy(title, ptitle);

	isQuit = false;

	int rfNum = 0;//\n���з��ĸ���
	int maxW = 0;//�����в�ͬ��֮��������
	int contentLen = wcslen(pcontent);
	wchar_t* preP = content;

	contentSplitPoint.push_back(0);
	//�ָ�content��ȷ����ͬ��
	for (int i = 0; i < contentLen; i++) {
		if (content[i] == _T('\n')) {
			rfNum++;
			content[i] = _T('\0');
			maxW = max(maxW, textwidth(preP));
			preP = content + i + 1;
			contentSplitPoint.push_back(i + 1);
		}
	}
	maxW = max(maxW, textwidth(preP));
	//��û�л��з�����Ϊ����ȼ��������ݵĿ��
	if (rfNum == 0)maxW = textwidth(pcontent);

	//�жϰ�ť�ܳ����Ƿ���ı���
	int totalBtnW = 0;
	switch (messageBoxType) {
	case MMBT_OK:
	{
		totalBtnW = max(totalBtnW,BTN_WIDTH);
		break;
	}
	case MMBT_TRY:
	{
		totalBtnW = max(totalBtnW, BTN_WIDTH*3);
		break;
	}
	}
	maxW = max(maxW, totalBtnW);

	beginX = (getwidth() - (maxW+ LEFT_MARGIN + RIGHT_MARGIN)) / 2;

	int contentH = textheight(pcontent) * (rfNum + 1) + TEXT_MIDDLE_MARGIN * (rfNum+1);
	beginY = (getheight() - (textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN + BOTTOM_MARGIN+BTN_HEIGHT)) / 2;

	endX = beginX + maxW + LEFT_MARGIN + RIGHT_MARGIN;
	endY = beginY+textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN + BOTTOM_MARGIN + BTN_HEIGHT;

	switch (messageBoxType) {
		case MMBT_OK:
		{
			OptionBtn btn(beginX + (endX - beginX - BTN_WIDTH) / 2, endY-BOTTOM_MARGIN-BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT,
				MMBR_OK, _T("ȷ��"));
			buttons.push_back(btn);
			break;
		}
		case MMBT_TRY:
		{
			OptionBtn btn(beginX+LEFT_MARGIN, endY - BOTTOM_MARGIN - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT,
				MMBR_TRY, _T("����"));
			buttons.push_back(btn);

			btn=OptionBtn(endX-RIGHT_MARGIN-BTN_WIDTH, endY - BOTTOM_MARGIN - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT,
				MMBR_CANCEL, _T("ȡ��"));
			buttons.push_back(btn);
			break;
		}
	}
	if (bgImgPath != _T(""))
	loadimage(&bgImg, bgImgPath, endX - beginX, endY - beginY);
}

MyMessageBox::~MyMessageBox()
{
	delete []content;
	delete[]title;
}

void MyMessageBox::draw()
{
	
	//������
	fillroundrect(beginX, beginY, endX, endY, 10, 10);

	putimage(beginX, beginY, &bgImg);

	int x = beginX+LEFT_MARGIN;
	int y = TOP_MARGIN+beginY;
	//����
	outtextxy(x, y, title);
	
	//�ָ���
	y += textheight(title);
	line(beginX, y + MIDDLE_LINE_MARGIN / 4, endX, y + MIDDLE_LINE_MARGIN / 4);

	//��Ҫ����
	y += MIDDLE_LINE_MARGIN;
	for (int i = 0; i < contentSplitPoint.size(); i++) {
		outtextxy(x, y + textheight(content) * i, content + contentSplitPoint[i]);
		y += TEXT_MIDDLE_MARGIN;
	}

	//��ť
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
			//�����ť�ж��Ƿ񱻵��
			for (OptionBtn& btn : buttons) {
				if (mx > btn.beginX && mx<btn.endX && my>btn.beginY && my < btn.endY) {
					switch (btn.m_label) {
						case MMBR_OK:
						{
							clickedBtnType = MMBR_OK;
							isQuit = true;
							break;
						}
						case MMBR_TRY:
						{
							clickedBtnType = MMBR_TRY;
							isQuit = true;
							break;
						}
						case MMBR_CANCEL:
						{
							clickedBtnType = MMBR_CANCEL;
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
	//EndBatchDraw();
	draw();
	FlushBatchDraw();
	while (!isQuit) {
		inputEvent();
		Sleep(60);
	}
	//BeginBatchDraw();
	return clickedBtnType;
}

int myMessageBox(int x, int y, const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, LPCTSTR bgImgPath)
{
	MyMessageBox box(x, y, pcontent, ptitle, messageBoxType, bgImgPath);
	return box.play();
}

int myMessageBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, LPCTSTR bgImgPath)
{
	
	MyMessageBox box(pcontent, ptitle, messageBoxType, bgImgPath);
	return box.play();
}
