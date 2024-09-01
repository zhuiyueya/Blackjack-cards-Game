#include "MyInputBox.h"
#include <stdexcept>
#include<iostream>
#define INPUTBOX_SIZE_W 200
#define INPUTBOX_SIZE_H 30
#define INPUTBOX_MIDDLE_MARGIN 10
#define BTN_WIDTH 60//消息盒子的宽度
#define BTN_HEIGHT 30//消息盒子的高度
#define TOP_MARGIN 5//顶部空白
#define MIDDLE_LINE_MARGIN 20//标题与内容之间的线与空白的宽度
#define TEXT_MIDDLE_MARGIN 10//内容中不同行间的间隔
#define BOTTOM_MARGIN 20//底部空白
#define LEFT_MARGIN 20//左边空白
#define RIGHT_MARGIN 20//右边空白
#define BTN_CONTENT_MIDDLE_MARGIN 20//按钮与内容之间的间隔

//第一参数：提示信息，第二参数:标题,第三：按钮类型，第四：用来保存用户输入内容的容器，第五：容器个数，第六：每个容器的大小
MyInputBox::MyInputBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, wchar_t **label,
	wchar_t** output, int num, std::initializer_list<int> lenList)
{
	//std::cout << "x\n";
	//try{
	//	if (num != lenList.size()) {
	//		
	//		throw std::runtime_error("address error occured");
	//	}
	//}
	//catch (const std::runtime_error& e) {
	//	
	//	std::cerr << "catch exception:" << e.what() << std::endl;
	//	//throw;
	//}
	//std::cout << 1;

	

	setfillcolor(WHITE);

	settextcolor(BLACK);

	setlinecolor(BLACK);

	settextstyle(20, 0, _T("微软雅黑"));

	//拷贝提示内容
	m_content = new wchar_t[wcslen(pcontent) + 1];
	wcscpy(m_content, pcontent);
	//拷贝标题
	m_title = new wchar_t[wcslen(ptitle) + 1];
	wcscpy(m_title, ptitle);

	m_isQuit = false;

	int rfNum = 0;//记录\n换行符的个数
	int maxTextW = 0;//内容中不同行之间的最大宽度
	int contentLen = wcslen(pcontent);
	wchar_t* preP = m_content;

	m_contentSplitPoint.push_back(0);
	//分割content，确定不同行
	for (int i = 0; i < contentLen; i++) {
		if (m_content[i] == _T('\n')) {
			rfNum++;
			m_content[i] = _T('\0');
			maxTextW = max(maxTextW, textwidth(preP));
			preP = m_content + i + 1;
			m_contentSplitPoint.push_back(i + 1);
		}
	}
	maxTextW = max(maxTextW, textwidth(preP));
	//若没有换行符，则为最大宽度即整段内容的宽度
	if (rfNum == 0)maxTextW = textwidth(pcontent);

	//存储用户给定标签并计算其中宽度最大的
	int maxInputLabelW = 0;
	if (label != NULL) {

		m_inputBoxLabel.resize(num);
		m_label.resize(num);
		for (int i = 0; i < num; i++) {
			//将用户提供的每个输入框的标签复制下来
			m_label[i] = new wchar_t[wcslen(label[i]) + 1];
			wcscpy(m_label[i], label[i]);

			maxInputLabelW = max(maxInputLabelW,textwidth(m_label[i]));
		}

	}
	
	//输入窗口总宽度 获取 提示文最大宽度+按钮宽度+前两者之间空白  与  输入框宽度 之间的较大值作为整个输入窗口宽度
	int maxWindowWidth = max(maxTextW , INPUTBOX_SIZE_W+ maxInputLabelW) + LEFT_MARGIN + RIGHT_MARGIN;
	//使得刚好绘制在中间
	m_beginX = (getwidth() - (maxWindowWidth)) / 2;

	//提示文的总height：
	int contentH = textheight(pcontent) * (rfNum + 1) + TEXT_MIDDLE_MARGIN * (rfNum);
	
	//输入窗口总高度 ： 标题高度+总提示文高度+顶部留白+提示文与标题之间线留白+底部留白+
	// num个输入框高度+num个输入框间隔（一个为提示文与输入框的,一个为输入框和底部按钮的）
	int maxWindowHeight = textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN + BOTTOM_MARGIN +
		num * INPUTBOX_SIZE_H + (num+1)*INPUTBOX_MIDDLE_MARGIN + BTN_HEIGHT;
	//使得刚好绘制在中间
	m_beginY = (getheight() - maxWindowHeight)/ 2;

	m_endX = m_beginX + maxWindowWidth;
	//暂时计算到提示文底部
	m_endY = m_beginY + textheight(ptitle) + contentH + TOP_MARGIN + MIDDLE_LINE_MARGIN;


	m_output.resize(num);
	m_inputBox.resize(num);
	m_isInputing.resize(num);

	int i = 0;
	for (int len : lenList) {
		if (i > num) {

		}
		
		//确定输入框位置
		m_endY += INPUTBOX_MIDDLE_MARGIN;
		m_inputBox[i] = Button(m_endX - RIGHT_MARGIN- INPUTBOX_SIZE_W, m_endY, INPUTBOX_SIZE_W, INPUTBOX_SIZE_H);

		//确认输入框标签位置
		if (!m_inputBoxLabel.empty())
			m_inputBoxLabel[i] = Button(m_beginX + LEFT_MARGIN, m_endY, maxInputLabelW, INPUTBOX_SIZE_H);

		m_endY += INPUTBOX_SIZE_H;

		m_output[i].first = output[i];
		wcscpy(m_output[i].first, _T(""));

		m_output[i].second = len;//记录总容量

		m_isInputing[i] = false;//全部初始化为不处于输入状态

		i++;
	}
	//输入框与按钮间隔
	m_endY += INPUTBOX_MIDDLE_MARGIN;

	m_cursorPos.first = -100;
	m_cursorPos.second = -100;


	switch (messageBoxType) {
	case MMBT_OK:
	{
		int btnNum = 1;
		int btnX = m_beginX + ((m_endX - m_beginX) - (2*btnNum-1) * BTN_WIDTH) / 2;
		OptionBtn btn(btnX, m_endY, BTN_WIDTH, BTN_HEIGHT, MMBT_OK, _T("确定"));
		m_buttons.push_back(btn);
		break;
	}
	case MMBT_TRY:
	{
		int btnNum = 2;
		break;
	}
	}
	

	//恢复正常正确
	m_endY = m_beginY + maxWindowHeight;
}

int MyInputBox::play()
{
	// 创建一个矩形区域
	HRGN rgn = CreateRectRgn(m_beginX,m_beginY,m_endX,m_endY);
	// 将该矩形区域设置为裁剪区
	setcliprgn(rgn);
	// 不再使用 rgn，清理 rgn 占用的系统资源
	DeleteObject(rgn);

	
	while (!m_isQuit) {
		//cleardevice();
		draw();
		inputEvent();
		FlushBatchDraw();
		Sleep(60);
	}

	// 取消之前设置的裁剪区
	setcliprgn(NULL);

	return 0;
}

MyInputBox::~MyInputBox()
{
	//释放内存
	for (int i = 0; i < m_label.size(); i++) {
		delete m_label[i];
	}
	delete []m_title;
	delete[]m_content;
	
}

void MyInputBox::inputEvent()
{

	ExMessage msg;
	while (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			int mx = msg.x;
			int my = msg.y;
			bool isClickEmptyArea = true;
			//逐个按钮判断是否被点击
			for (OptionBtn& btn : m_buttons) {
				if (mx > btn.beginX && mx<btn.endX && my>btn.beginY && my < btn.endY) {
					isClickEmptyArea = false;
					switch (btn.m_label) {
					case MMBT_OK:
					{
						m_clickedBtnType = MMBT_OK;
						m_isQuit = true;
						break;
					}
					}
				}
			}
			//逐个输入框判断是否被点击
			int choosedBtn=0;
			for (Button& btn : m_inputBox) {
				if (mx > btn.beginX && mx<btn.endX && my>btn.beginY && my < btn.endY) {
					isClickEmptyArea = false;
					break;
				}
				choosedBtn++;
			}
			//说明有被按到的输入框
			if (choosedBtn != m_inputBox.size()) {

				for (int i = 0; i < m_isInputing.size(); i++) {
					if (m_isInputing[i]) {
						m_isInputing[i] = false;
						break;
					}
				}
				m_isInputing[choosedBtn] = true;
				//废弃方案-以|符号添加进数组实现光标
				//for (int i = 0; i < m_inputBox.size(); i++) {
				//	if (m_isInputing[i]) {
				//		//能进入if说明绝对有|符号，则数组长度至少为1
				//		int contentLen= wcslen(m_output[i].first);
				//		m_output[i].first[contentLen - 1] = _T('\0');
				//		break;
				//	}
				//}
				//
				//if (!m_isInputing[choosedBtn]) {
				//	
				//	int contentLen = wcslen(m_output[choosedBtn].first);
				//	m_output[choosedBtn].first[contentLen] = _T('|');
				//	m_output[choosedBtn].first[contentLen+1] = _T('\0');
				//	std::wcout << m_output[choosedBtn].first;
				//}
				//

				settextstyle(16, 0, _T("微软雅黑"));
				//更新光标位置-在输入字符时也需要更新x坐标
				m_cursorPos.first = m_inputBox[choosedBtn].beginX+textwidth(m_output[choosedBtn].first)+10;
				
				m_cursorPos.second= m_inputBox[choosedBtn].beginY+ 
					((m_inputBox[choosedBtn].endY - m_inputBox[choosedBtn].beginY) - 16) / 2;
			}

			//鼠标点击了空白区域，设置所有输入框为非输入状态
			if (isClickEmptyArea) {
				for (int i = 0; i < m_inputBox.size(); i++) {
					if (m_isInputing[i]) {
						m_isInputing[i] = false;
						break;
					}
				}
			}



		}
		else if (msg.message == WM_CHAR) {
			//输入为回退(backspace)时
			if (msg.vkcode == VK_BACK) {
				for (int i = 0; i < m_isInputing.size(); i++) {
					if (m_isInputing[i]) {
						int len = wcslen(m_output[i].first);
						if(len>0)
						m_output[i].first[len - 1] = _T('\0');
						//更新光标位置
						settextstyle(16, 0, _T("微软雅黑"));
						m_cursorPos.first = m_inputBox[i].beginX + textwidth(m_output[i].first) + 10;
						break;
					}
				}
			}//输入为普通字符时
			else {
				
				for (int i = 0; i < m_isInputing.size(); i++) {
					if (m_isInputing[i]) {
						if(wcslen(m_output[i].first)<m_output[i].second-1)
						swprintf(m_output[i].first, _T("%s%c"), m_output[i].first, msg.ch);
						//更新光标位置
						settextstyle(16, 0, _T("微软雅黑"));
						m_cursorPos.first = m_inputBox[i].beginX + textwidth(m_output[i].first)+10;
						break;
					}
				}
			}
		}
	}

}

void MyInputBox::draw()
{
	
	//背景板
	fillroundrect(m_beginX, m_beginY, m_endX, m_endY, 10, 10);

	int x = m_beginX + LEFT_MARGIN;
	int y = TOP_MARGIN + m_beginY;

	settextstyle(16, 0, _T("微软雅黑"));
	//标题
	outtextxy(x, y, m_title);

	//分割线
	y += textheight(m_title);
	line(m_beginX, y + MIDDLE_LINE_MARGIN / 4, m_endX, y + MIDDLE_LINE_MARGIN / 4);

	settextstyle(20, 0, _T("微软雅黑"));
	//提示内容
	y += MIDDLE_LINE_MARGIN;
	for (int i = 0; i < m_contentSplitPoint.size(); i++) {
		outtextxy(x, y + textheight(m_content) * i, m_content + m_contentSplitPoint[i]);
		y += TEXT_MIDDLE_MARGIN;
	}

	//按钮
	for (OptionBtn& btn : m_buttons) {
		btn.draw();
	}

	settextstyle(16, 0, _T("微软雅黑"));
	//绘制输入框
	for (int i = 0; i < m_inputBox.size(); i++) {
		m_inputBox[i].draw(m_output[i].first, BTN_TEXT_STYLE_LEFT);
	}

	settextstyle(20, 0, _T("微软雅黑"));
	//绘制输入框标签
	for (int i = 0; i < m_inputBoxLabel.size(); i++) {
		m_inputBoxLabel[i].draw(m_label[i], BTN_TEXT_STYLE_LEFT);
	}


	//绘制光标,且实现闪烁效果
	static int flash = 0;
	if(flash%5!=0&& flash % 6 != 0&& flash % 7 != 0&& flash % 8 != 0&& flash % 9 != 0)
	line(m_cursorPos.first, m_cursorPos.second, m_cursorPos.first, m_cursorPos.second + 16);
	flash++;
	if (flash == INT_MAX)flash = 0;
}

//第一参数：提示信息，第二参数:标题,第三：按钮类型，第四：用来保存用户输入内容的容器，第五：容器个数，第六：每个容器的大小
int myInputBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, wchar_t** label, wchar_t** output, int num, std::initializer_list<int> lenList)
{
	MyInputBox box(pcontent, ptitle, messageBoxType, label, output, num, lenList);
	return box.play();
}
