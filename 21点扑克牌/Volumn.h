#pragma once
#include"Slider.h"
#include"Button.h"
class Volumn
{
public:
	Volumn(int *volumn);
	Volumn();
	void inputEvent(ExMessage *msg);
	void draw();
	static Volumn& getInstance();
	void setVolumn();
private:
	Slider m_volumnSlider;
	Button m_volumnAddBtn;
	Button m_volumnSubBtn;
	int m_volumn;
};

