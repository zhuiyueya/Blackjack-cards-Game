#pragma once
#include "MyMessageBox.h"
#include<initializer_list>
class MyInputBox
{   
public:
    //��һ��������ʾ��Ϣ���ڶ�����:����,��������ť���ͣ����ģ����������û��������ݵ����������壺����������������ÿ�������Ĵ�С
    MyInputBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, wchar_t** label, wchar_t **output,int num,std::initializer_list<int>lenList);
    
    int play();

    ~MyInputBox();

private:

    void inputEvent();

    void draw();

private:
    std::vector<std::pair<wchar_t*, int>>m_output;//�洢ָ�����Ա�ṩ�Ĵ洢����Ŀռ��ָ��Ͷ�Ӧ���������

    std::vector<wchar_t*>m_label;//�洢������Ӧ��ǩ
    
    std::vector<Button>m_inputBox;//�洢�����

    std::vector<Button>m_inputBoxLabel;//�洢������ǩ

    std::vector<bool>m_isInputing;//�洢�ĸ��������������״̬

    std::pair<int, int>m_cursorPos;//���λ��

    int m_beginX;

    int m_beginY;

    int m_endX;

    int m_endY;

    wchar_t* m_title;//��Ϣ���ӱ���

    wchar_t* m_content;//��Ҫ����

    std::vector<OptionBtn>m_buttons;//�洢��ť

    std::vector<int>m_contentSplitPoint;//�洢��ͬ�е���content��Ա�����е���ʼλ�õ�ƫ����

    bool m_isQuit;//�Ƿ�رմ���

    int m_clickedBtnType;//������İ�ť���ͣ���Ӧ��MyMessageBoxType

};

//��һ��������ʾ��Ϣ���ڶ�����:����,��������ť���ͣ����ģ����������û��������ݵ����������壺����������������ÿ�������Ĵ�С
int myInputBox(const wchar_t* pcontent, const wchar_t* ptitle, int messageBoxType, wchar_t** label, wchar_t** output, int num, std::initializer_list<int>lenList);

