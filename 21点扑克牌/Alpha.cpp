#include "Alpha.h"

int alpha = 128;//透明度：0~256

DWORD* g_pBuf11;//缓冲区

//设置透明度
void setalpha(int al) {
	alpha = al;
}

//进行前景色与背景色的半透明计算
COLORREF alphagb(int r1, int g1, int b1, int r2, int g2, int b2) {
	COLORREF rgb_alpha = RGB(
		(r1 * alpha + r2 * (256 - alpha)) >> 8,
		(g1 * alpha + g2 * (256 - alpha)) >> 8,
		(b1 * alpha + b2 * (256 - alpha)) >> 8
	);
	return rgb_alpha;
}

//显示缓冲区指针，并读取坐标上的颜色值
COLORREF fast_getpixelcolor(int x, int y, int width) {
	COLORREF c = g_pBuf11[y * width + x];
	return BGR(c);
}

//快速缓冲区半透明画点函数
void putpixelalpha(DWORD* g_pbuf, int x, int y, COLORREF c) {
	int width = getwidth(), height = getheight(), puti = y * width + x;

	//如果puti超出了缓冲区，则直接返回
	if (puti < 0 || puti >= (WIDTH * HEIGHT))return;

	COLORREF backdropcolor, bkcolor = getbkcolor();
	//读取背景上颜色点
	backdropcolor = fast_getpixelcolor(x, y, width);

	int r2 = GetRValue(c), g2 = GetGValue(c), b2 = GetBValue(c), r1=GetRValue(backdropcolor), g1 = GetGValue(backdropcolor), b1 = GetBValue(backdropcolor);
	
	//如果获取的颜色等于背景色不进行透明计算
	if (bkcolor == backdropcolor) {
		g_pbuf[puti] = BGR(c);
	}
	if (backdropcolor != bkcolor) {
		g_pbuf[puti] = BGR(alphagb(r2, g2, b2, r1, g1, b1));
	}
}

//绘制矩形
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
