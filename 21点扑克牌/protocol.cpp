#include"protocol.h"

PDU* mkPDU(int msgLen)
{
	int pduLen = msgLen + sizeof(PDU);
	PDU* pdu = (PDU*)malloc(pduLen);
	pdu->msgLen = msgLen;
	return pdu;
}

void init()
{

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
