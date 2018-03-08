#include "msgbox.h"
#include "ui_msgbox.h"
#include <QDesktopWidget>
#include <QTextCodec>
#include <QTimer>
#include <QDebug>

MsgBox::MsgBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::msgbox)
{
    ui->setupUi(this);
    initMsgBoxPage();

}

MsgBox::~MsgBox()
{
    delete ui;
}

void MsgBox::setCheckTimer(qint8 time)
{
    timer.setInterval(time * 1000);
}


void MsgBox::initMsgBoxPage()
{
    timer.setInterval(110 * 1000);
    timer.setSingleShot(true);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_ShowWithoutActivating,true);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/msgbox/images/msgBox/msgbox_bg.png")));
    this->setPalette(palette);
    this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);

    ui->label->setFixedSize(QPixmap(":/msgbox/images/msgBox/word.png").size());
    ui->label->setPixmap(QPixmap(":/msgbox/images/msgBox/word.png"));

    ui->wordLabel->setStyleSheet("font-family:wenquanyi;font-size:24px;color:rgb(45,57,187)");

    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/msgbox/images/msgBox/okBtn_bg.png"));
    ui->okBtn->setFixedSize(QPixmap(":/msgbox/images/msgBox/okBtn_bg.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");

    ui->cancelBtn->setFlat(true);
    ui->cancelBtn->setFocusPolicy(Qt::NoFocus);
    ui->cancelBtn->setIcon(QIcon(":/msgbox/images/msgBox/cancelBtn_bg.png"));
    ui->cancelBtn->setFixedSize(QPixmap(":/msgbox/images/msgBox/cancelBtn_bg.png").size());
    ui->cancelBtn->setIconSize(ui->cancelBtn->size());
    ui->cancelBtn->setStyleSheet("background-color:transparent");

    connect(ui->okBtn,SIGNAL(clicked()),this,SIGNAL(msgBox_Ok()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SIGNAL(msgBox_Cancel()));
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(stopTimer()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(stopTimer()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(stopTimer()));

    tc =  QTextCodec::codecForName("UTF-8");
}

void MsgBox::showMsgBox(QString text,bool code , qint8 btnNum, Qt::WindowModality model)
{
    timer.start();

    if(code)
    {
        text = tc->toUnicode(text.toAscii());
    }
    ui->wordLabel->setText(text);

    if(btnNum == 1)
    {
        ui->cancelBtn->hide();
    }
    else
    {
        ui->cancelBtn->show();
    }

    if(model == Qt::NonModal)
    {
        this->show();
    }
    else
    {
        this->exec();
    }
}

void MsgBox::stopTimer()
{
    emit msgBox_Close();
    timer.stop();
    this->close();
}
