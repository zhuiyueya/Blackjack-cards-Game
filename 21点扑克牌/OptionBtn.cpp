#include "OptionBtn.h"

OptionBtn::OptionBtn(int x, int y, int width, int height, int label, const wchar_t* text) : Button(x, y, width, height), m_label(label)
{
	wcscpy(m_text, text);
}

void OptionBtn::draw()
{
	Button::draw(m_text);
}
