#pragma once
#include "Button.h"
enum OptionBtnLabel {
    ENUM_OPTION_BTN_LABEL_ACCOUNT,//�˺�
    ENUM_OPTION_BTN_LABEL_VOLUMN ,//����
};
class OptionBtn :
    public Button
{
public:
    OptionBtn(int x, int y, int width, int height, int label, const wchar_t* text); 
        
    void draw();
public:
    int m_label;
    wchar_t m_text[32];

    
};

