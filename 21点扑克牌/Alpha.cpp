#include "Alpha.h"

int alpha = 128;//͸���ȣ�0~256

DWORD* g_pBuf11;//������

//����͸����
void setalpha(int al) {
	alpha = al;
}

//����ǰ��ɫ�뱳��ɫ�İ�͸������
COLORREF alphagb(int r1, int g1, int b1, int r2, int g2, int b2) {
	COLORREF rgb_alpha = RGB(
		(r1 * alpha + r2 * (256 - alpha)) >> 8,
		(g1 * alpha + g2 * (256 - alpha)) >> 8,
		(b1 * alpha + b2 * (256 - alpha)) >> 8
	);
	return rgb_alpha;
}

//��ʾ������ָ�룬����ȡ�����ϵ���ɫֵ
COLORREF fast_getpixelcolor(int x, int y, int width) {
	COLORREF c = g_pBuf11[y * width + x];
	return BGR(c);
}

//���ٻ�������͸�����㺯��
void putpixelalpha(DWORD* g_pbuf, int x, int y, COLORREF c) {
	int width = getwidth(), height = getheight(), puti = y * width + x;

	//���puti�����˻���������ֱ�ӷ���
	if (puti < 0 || puti >= (WIDTH * HEIGHT))return;

	COLORREF backdropcolor, bkcolor = getbkcolor();
	//��ȡ��������ɫ��
	backdropcolor = fast_getpixelcolor(x, y, width);

	int r2 = GetRValue(c), g2 = GetGValue(c), b2 = GetBValue(c), r1=GetRValue(backdropcolor), g1 = GetGValue(backdropcolor), b1 = GetBValue(backdropcolor);
	
	//�����ȡ����ɫ���ڱ���ɫ������͸������
	if (bkcolor == backdropcolor) {
		g_pbuf[puti] = BGR(c);
	}
	if (backdropcolor != bkcolor) {
		g_pbuf[puti] = BGR(alphagb(r2, g2, b2, r1, g1, b1));
	}
}

//���ƾ���
void rec(int x1, int y1, int x2, int y2, COLORREF c, int alp)
{
	setalpha(alp);
	g_pBuf11 = GetImageBuffer();
	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			putpixelalpha(g_pBuf11, j, i, c);
		}
	}
}
