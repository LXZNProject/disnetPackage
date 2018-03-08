#include "keyboard.h"
#include "ui_keyboard.h"
#include <QBoxLayout>
#include <QBitmap>
#include <QDebug>

keyboard::keyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyboard)
{
    ui->setupUi(this);

    initpage();

    connect(ui->btn0,SIGNAL(clicked()),this,SIGNAL(btn0signal()));
    connect(ui->btn1,SIGNAL(clicked()),this,SIGNAL(btn1signal()));
    connect(ui->btn2,SIGNAL(clicked()),this,SIGNAL(btn2signal()));
    connect(ui->btn3,SIGNAL(clicked()),this,SIGNAL(btn3signal()));
    connect(ui->btn4,SIGNAL(clicked()),this,SIGNAL(btn4signal()));
    connect(ui->btn5,SIGNAL(clicked()),this,SIGNAL(btn5signal()));
    connect(ui->btn6,SIGNAL(clicked()),this,SIGNAL(btn6signal()));
    connect(ui->btn7,SIGNAL(clicked()),this,SIGNAL(btn7signal()));
    connect(ui->btn8,SIGNAL(clicked()),this,SIGNAL(btn8signal()));
    connect(ui->btn9,SIGNAL(clicked()),this,SIGNAL(btn9signal()));
    connect(ui->btnspace,SIGNAL(clicked()),this,SIGNAL(btnspacesignal()));
    connect(ui->backbtn,SIGNAL(clicked()),this,SIGNAL(backbtnsignal()));


}

keyboard::~keyboard()
{
    delete ui;
}

void keyboard::initpage()
{
//    QPixmap pixmap(":/key/images/keyboard/key_bg.png");
//    this->setMask(pixmap.mask());
//    this->resize( pixmap.size() );
//    QPalette palette;
//    palette.setBrush(QPalette::Background, QBrush(pixmap) );
//    this->setPalette(palette);

  //  this->setStyleSheet("background-color:transparent;");
    this->setAttribute(Qt::WA_ShowWithoutActivating,true);

    this->setWindowFlags(Qt::FramelessWindowHint);


    this->move(0,0);


    ui->btn0->setFocusPolicy(Qt::NoFocus);
    ui->btn0->setFlat(true);
    ui->btn0->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btn1->setFocusPolicy(Qt::NoFocus);
    ui->btn1->setFlat(true);
    ui->btn1->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btn2->setFocusPolicy(Qt::NoFocus);
    ui->btn2->setFlat(true);
    ui->btn2->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btn3->setFocusPolicy(Qt::NoFocus);
    ui->btn3->setFlat(true);
    ui->btn3->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btn4->setFocusPolicy(Qt::NoFocus);
    ui->btn4->setFlat(true);
    ui->btn4->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btn5->setFocusPolicy(Qt::NoFocus);
    ui->btn5->setFlat(true);
    ui->btn5->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btn6->setFocusPolicy(Qt::NoFocus);
    ui->btn6->setFlat(true);
    ui->btn6->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btn7->setFocusPolicy(Qt::NoFocus);
    ui->btn7->setFlat(true);
    ui->btn7->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btn8->setFocusPolicy(Qt::NoFocus);
    ui->btn8->setFlat(true);
    ui->btn8->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btn9->setFocusPolicy(Qt::NoFocus);
    ui->btn9->setFlat(true);
    ui->btn9->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->backbtn->setFocusPolicy(Qt::NoFocus);
    ui->backbtn->setFlat(true);
    ui->backbtn->setStyleSheet("background-color:transparent;background-image: url(:)");

    ui->btnspace->setFocusPolicy(Qt::NoFocus);
    ui->btnspace->setFlat(true);
    ui->btnspace->setStyleSheet("background-color:transparent;background-image: url(:)");

}


void keyboard::setLineEditData(QString text)
{
    this->lineEditData = text;
    result = result + lineEditData;
}


//响应相应的软盘按下事件
void keyboard::btn0slot()
{
    result = result + "0";
    emit dataChange();
}

void keyboard::btn1slot()
{
    result = result + "1";
    emit dataChange();
}

void keyboard::btn2slot()
{
    result = result + "2";
    emit dataChange();
}

void keyboard::btn3slot()
{
    result = result + "3";
    emit dataChange();
}

void keyboard::btn4slot()
{
    result = result + "4";
    emit dataChange();
}

void keyboard::btn5slot()
{
    result = result + "5";
    emit dataChange();
}

void keyboard::btn6slot()
{
    result = result + "6";
    emit dataChange();
}

void keyboard::btn7slot()
{
    result = result + "7";
    emit dataChange();
}

void keyboard::btn8slot()
{
    result = result + "8";
    emit dataChange();
}

void keyboard::btn9slot()
{
    result = result + "9";
    emit dataChange();
}

void keyboard::btnXslot()
{
    result = result + "X";
    emit dataChange();
}

void keyboard::btnspaceslot()
{
    result = result + ".";
    emit dataChange();
}

void keyboard::btnwellslot()
{
    result = result + "#";
    emit dataChange();

}

void keyboard::backbtnslot()
{
    if(result.length() == 0)
        return;
    else
    {
        //退格的响应
        result = result.left(result.length()-1);
        emit dataChange();
    }

}


/****************************************************************
**
*	函数名称：	okbtnslot
*	函数功能：	确认按钮槽
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void keyboard::okbtnslot()
{
    if(result.size())
    {
        emit focusChange();
        result.clear();

    }
    this->close();
}


/****************************************************************
**
*	函数名称：	cancelbtnslot
*	函数功能：	取消按钮槽
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void keyboard::cancelbtnslot()
{
    /*
    lastresult = result;
    */
  //  lastresult = result;
    result = this->lineEditData;
    emit dataChange();
    result.clear();
    this->close();
}

/*
void keyboard::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_0:
        btn0slot();
        break;
    case Qt::Key_1:
         btn1slot();
         break;
    case Qt::Key_2:
        btn2slot();
        break;
    case Qt::Key_3:
        btn3slot();
        break;
    case Qt::Key_4:
        btn4slot();
        break;
    case Qt::Key_5:
        btn5slot();
        break;
    case Qt::Key_6:
        btn6slot();
        break;
    case Qt::Key_7:
        btn7slot();
        break;
    case Qt::Key_8:
        btn8slot();
        break;
    case Qt::Key_9:
        btn9slot();
        break;
    case 0x20:
        btnspaceslot();
        break;
    case 0x58:
        btnXslot();
        break;
    case 0x23:         //#
        btnwellslot();
        break;
    case 0x08:            //delf
        backbtnslot();
        break;
    case 0x18:
        cancelbtnslot();
        break;
    case 0x0D:
        okbtnslot();
        break;

    }
}

*/

void keyboard::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

