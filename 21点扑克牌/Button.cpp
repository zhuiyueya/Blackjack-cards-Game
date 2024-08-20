#include "Button.h"

Button::Button()
{
}

Button::Button(int x, int y, int width, int height)
{
	beginX = x;
	beginY = y;
	endX = x + width;
	endY = y + height;
}

void Button::draw(LPCTSTR text, enum BTNTEXTSTYLE type)
{
	int tx;
	int ty;
	if (type == BTN_TEXT_STYLE_MIDDLE) {
		tx = beginX + ((endX - beginX) - textwidth(text)) / 2;
		ty = beginY + ((endY - beginY) - textheight(text)) / 2;
	}
	else if (type == BTN_TEXT_STYLE_LEFT) {
		tx = beginX + textwidth(_T("|"));
		ty = beginY + ((endY - beginY) - textheight(text)) / 2;
	}
	fillroundrect(beginX, beginY, endX, endY , 5, 5);
	outtextxy(tx, ty, text);
}

void Button::draw(IMAGE* img)
{
	transparentImage(NULL, beginX, beginY , img, BLACK);
	//putimage(, (endX - beginX), (endY - beginY), img, img->getwidth(), img->getheight());
}
