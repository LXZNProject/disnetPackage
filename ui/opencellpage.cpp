#include "opencellpage.h"
#include "ui_opencellpage.h"
#include <QDesktopWidget>

opencellpage::opencellpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::opencellpage)
{
    ui->setupUi(this);

    initPage();
}

opencellpage::~opencellpage()
{
    delete ui;
}

void opencellpage::initPage()
{
    timer.setInterval(100 * 1000);
    timer.setSingleShot(true);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(350,250);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/msgbox/images/msgBox/msgbox_bg.png")));
    this->setPalette(palette);
    this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);

    //ui->door->resize(QPixmap(":/save/images/saveWait/door.png").size());
    //ui->door->setPixmap(QPixmap(":/save/images/saveWait/door.png"));
    ui->door->setStyleSheet("font-family:wenquanyi; font-size:18px; color: rgb(0, 0, 0)");
    ui->doorId->setStyleSheet("font-family:wenquanyi; font-size:18px; color: rgb(0, 0, 0)");
    ui->door->setText(QString::fromUtf8("箱门："));

    ui->phoneLabel->setStyleSheet("font-family:wenquanyi; font-size:18px; color: rgb(0, 0, 0)");
    ui->phoneInfoLabel->setStyleSheet("font-family:wenquanyi; font-size:18px; color: rgb(0, 0, 0)");
    ui->phoneLabel->setText(QString::fromUtf8("寄件人手机号码："));

    ui->companyLabel->setStyleSheet("font-family:wenquanyi; font-size:18px; color: rgb(0, 0, 0)");
    ui->companyInfoLabel->setStyleSheet("font-family:wenquanyi; font-size:18px; color: rgb(0, 0, 0)");
    ui->companyLabel->setText(QString::fromUtf8("选择快递公司："));

    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/msgbox/images/msgBox/okBtn_bg.png"));
    ui->okBtn->setFixedSize(QPixmap(":/msgbox/images/msgBox/okBtn_bg.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");

    connect(&timer,SIGNAL(timeout()),this,SLOT(timerOver()));
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
}

void opencellpage::showOpneCell(SenderDesc_t senderInfo)
{
    if(senderInfo.deliveryType == 2)
    {
        ui->companyLabel->hide();
        ui->companyInfoLabel->hide();
    }
    else
    {
        ui->companyLabel->show();
        ui->companyInfoLabel->show();
        ui->companyInfoLabel->setText(senderInfo.companyName);
    }

    ui->doorId->setText(senderInfo.cellId);
    ui->phoneInfoLabel->setText(senderInfo.mobilephone);

    timer.start();
    this->exec();
}

void opencellpage::timerOver()
{
    emit disOpenCell();
    this->close();
}

void opencellpage::okBtn_slots()
{
    timer.stop();
    emit disOpenCell();
    this->close();
}
