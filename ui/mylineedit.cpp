#include "mylineedit.h"
//#include "syszuxim.h"
#include <QDebug>
#include <QApplication>

#define SHOW_TIME 60*1000

CKeyBoardLineEdit::CKeyBoardLineEdit(QWidget * parent):
    QLineEdit(parent)
{

    recv = false;
    this->setStyleSheet("QLineEdit{font-family:wenquanyi; font-size:30px;padding: 1px;border-style: solid;border: 2px solid gray;border-radius: 8px;}");
}

CKeyBoardLineEdit::~CKeyBoardLineEdit()
{

}

void CKeyBoardLineEdit::bindKeyPage(keyboard *page)
{
    mykeyboard = page;
    connect(mykeyboard,SIGNAL(btn0signal()),this,SLOT(btn0slot()));
    connect(mykeyboard,SIGNAL(btn1signal()),this,SLOT(btn1slot()));
    connect(mykeyboard,SIGNAL(btn2signal()),this,SLOT(btn2slot()));
    connect(mykeyboard,SIGNAL(btn3signal()),this,SLOT(btn3slot()));
    connect(mykeyboard,SIGNAL(btn4signal()),this,SLOT(btn4slot()));
    connect(mykeyboard,SIGNAL(btn5signal()),this,SLOT(btn5slot()));
    connect(mykeyboard,SIGNAL(btn6signal()),this,SLOT(btn6slot()));
    connect(mykeyboard,SIGNAL(btn7signal()),this,SLOT(btn7slot()));
    connect(mykeyboard,SIGNAL(btn8signal()),this,SLOT(btn8slot()));
    connect(mykeyboard,SIGNAL(btn9signal()),this,SLOT(btn9slot()));
    connect(mykeyboard,SIGNAL(btnXsignal()),this,SLOT(btnXslot()));
    connect(mykeyboard,SIGNAL(btnspacesignal()),this,SLOT(btnspaceslot()));
    connect(mykeyboard,SIGNAL(btnwellsignal()),this,SLOT(btnwellslot()));
    connect(mykeyboard,SIGNAL(backbtnsignal()),this,SLOT(backbtnslot()));
    connect(mykeyboard,SIGNAL(okbtnsignal()),this,SLOT(okbtnslot()));
    connect(mykeyboard,SIGNAL(cancelbtnsignal()),this,SLOT(cancelbtnslot()));

    connect(mykeyboard,SIGNAL(btn0signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btn1signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btn2signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btn3signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btn4signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btn5signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btn6signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btn7signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btn8signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btn9signal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btnXsignal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btnspacesignal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(btnwellsignal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(backbtnsignal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(okbtnsignal()),this,SIGNAL(btnPress()));
    connect(mykeyboard,SIGNAL(cancelbtnsignal()),this,SIGNAL(btnPress()));
}

/***************************************************************

        函数名称：focusInEvent()
        函数功能：当编辑框获取焦点的响应
        函数参数： 无
        返回值：无

***************************************************************/

void CKeyBoardLineEdit::focusInEvent(QFocusEvent *event)
{
   // SyszuxIM::hideInputMethod();
    if(!this->isReadOnly())
    {
        recv = true;
    }
    QLineEdit::focusInEvent(event);
}




void CKeyBoardLineEdit::focusOutEvent(QFocusEvent *event)
{
    //SyszuxIM::hideInputMethod();
    recv = false;
    QLineEdit::focusOutEvent(event);

}



void CKeyBoardLineEdit::btn0slot()
{
   if(recv)
   this->insert("0");
}


void CKeyBoardLineEdit::btn1slot()
{
    if(recv)
    this->insert("1");
}

void CKeyBoardLineEdit::btn2slot()
{
    if(recv)
    this->insert("2");
}

void CKeyBoardLineEdit::btn3slot()
{
    if(recv)
    this->insert("3");
}

void CKeyBoardLineEdit::btn4slot()
{
    if(recv)
    this->insert("4");
}

void CKeyBoardLineEdit::btn5slot()
{
    if(recv)
    this->insert("5");
}

void CKeyBoardLineEdit::btn6slot()
{
    if(recv)
    this->insert("6");
}


void CKeyBoardLineEdit::btn7slot()
{
    if(recv)
    this->insert("7");
}

void CKeyBoardLineEdit::btn8slot()
{
    if(recv)
    this->insert("8");
}

void CKeyBoardLineEdit::btn9slot()
{
    if(recv)
    this->insert("9");
}

void CKeyBoardLineEdit::btnXslot()
{
    if(recv)
    this->insert("X");
}

void CKeyBoardLineEdit::btnspaceslot()
{
    if(recv)
    this->insert(".");
}

void CKeyBoardLineEdit::btnwellslot()
{
    if(recv)
    this->insert("#");
}

void CKeyBoardLineEdit::backbtnslot()
{
    if(recv)
    this->backspace();
}

void CKeyBoardLineEdit::okbtnslot()
{
   this->focusNextChild();
   emit okBtnPress();
}

void CKeyBoardLineEdit::cancelbtnslot()
{
    this->clear();

}

void CKeyBoardLineEdit::keyPressEvent(QKeyEvent *event)
{

    if(!this->isReadOnly())
    {
        if(event->key() == Qt::Key_Delete)
        {
            cancelbtnslot();
        }
        else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        {
            okbtnslot();
        }
        else
        {
            QLineEdit::keyPressEvent(event);
        }
    }

}
