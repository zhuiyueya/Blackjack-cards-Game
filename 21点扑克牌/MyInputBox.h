#pragma once
#include "MyMessageBox.h"
#include<initializer_list>
class MyInputBox
{   
public:
    //第一参数：提示信息，第二参数:标题,第三：按钮类型，第四：用来保存用户输入内容的容器，第五：容器个数，第六：每个容器的大小
    MyInputBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, wchar_t** label, wchar_t **output,int num,std::initializer_list<int>lenList);
    
    int play();

    ~MyInputBox();

private:

    void inputEvent();

    void draw();

private:
    std::vector<std::pair<wchar_t*, int>>m_output;//存储指向程序员提供的存储输入的空间的指针和对应的最大容量

    std::vector<wchar_t*>m_label;//存储输入框对应标签
    
    std::vector<Button>m_inputBox;//存储输入框

    std::vector<Button>m_inputBoxLabel;//存储输入框标签

    std::vector<bool>m_isInputing;//存储哪个输入框正在输入状态

    std::pair<int, int>m_cursorPos;//光标位置

    int m_beginX;

    int m_beginY;

    int m_endX;

    int m_endY;

    wchar_t* m_title;//信息盒子标题

    wchar_t* m_content;//主要内容

    std::vector<OptionBtn>m_buttons;//存储按钮

    std::vector<int>m_contentSplitPoint;//存储不同行的在content成员变量中的起始位置的偏移量

    bool m_isQuit;//是否关闭窗口

    int m_clickedBtnType;//被点击的按钮类型，对应于MyMessageBoxType

};

//第一参数：提示信息，第二参数:标题,第三：按钮类型，第四：用来保存用户输入内容的容器，第五：容器个数，第六：每个容器的大小
int myInputBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, wchar_t** label, wchar_t** output, int num, std::initializer_list<int>lenList);

