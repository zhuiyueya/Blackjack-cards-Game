#include"protocol.h"
#include<conio.h>
#pragma comment(lib,"MSIMG32.LIB")
PDU* mkPDU(int msgLen)
{
	int pduLen = msgLen + sizeof(PDU);
	PDU* pdu = (PDU*)malloc(pduLen);
	pdu->msgLen = msgLen;
	return pdu;
}


void button(int x, int y, LPCTSTR text, bool isTransparent, int width, int height)
{
	if (isTransparent)
		setfillstyle(BS_NULL);
	else
		setfillstyle(BS_SOLID);
	int tx = x + (width - textwidth(text)) / 2;
	int ty = y + (height - textheight(text)) / 2;
	fillroundrect(x, y, x + width, y + height, 5, 5);
	outtextxy(tx, ty, text);
}

//绘制背景透明的图片的函数
void transparentImage(IMAGE* dst, int x, int y, IMAGE* src, UINT transaprentcolor)
{
	HDC dstDC = GetImageHDC(dst);
	HDC srcDC = GetImageHDC(src);
	int w = src->getwidth();
	int h = src->getheight();
	TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transaprentcolor);
}
