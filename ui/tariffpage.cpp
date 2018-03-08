#include "tariffpage.h"
#include "ui_tariffpage.h"
#include "mainctrl.h"
#include "t_basic_terminal.h"
#include <QBitmap>
#include <QTabBar>
#include <QTextCodec>

tariffPage::tariffPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tariffPage)
{
    ui->setupUi(this);
    initPage();
}

tariffPage::~tariffPage()
{
    delete ui;
}

void tariffPage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setFixedSize(800,600);
    QPixmap pixmap(":/tariff/images/tariff/background.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);


    ui->tabWidget->setTabIcon(2,QIcon(":/tariff/images/tariff/3.png"));
    ui->tabWidget->setTabIcon(1,QIcon(":/tariff/images/tariff/2.png"));
    ui->tabWidget->setTabIcon(0,QIcon(":/tariff/images/tariff/1.png"));
    QSize size = QPixmap(":/tariff/images/tariff/1.png").size();
    ui->tabWidget->setIconSize(QSize(size.width(),size.height()));

    ui->tabWidget->setFocusPolicy(Qt::NoFocus);
    QString style = "QTabBar::tab{border: 0px;border-top-left-radius: 0px;border-top-right-radius: 0px;min-width: 0ex;padding: 0px;min-width:100px;min-height:96px}\
            QTabWidget::pane{border-image: url(:/page/images/main/tab_bg.png)}";

    ui->tabWidget->setStyleSheet(style);


    //第一页
    ui->label1->resize(QPixmap(":/tariff/images/tariff/save/1.png").size());
    ui->label1->setPixmap(QPixmap(":/tariff/images/tariff/save/1.png"));

    ui->label2->resize(QPixmap(":/tariff/images/tariff/save/2.png").size());
    ui->label2->setPixmap(QPixmap(":/tariff/images/tariff/save/2.png"));

    ui->label3->resize(QPixmap(":/tariff/images/tariff/save/3.png").size());
    ui->label3->setPixmap(QPixmap(":/tariff/images/tariff/save/3.png"));

    ui->label4->resize(QPixmap(":/tariff/images/tariff/save/4.png").size());
    ui->label4->setPixmap(QPixmap(":/tariff/images/tariff/save/4.png"));

    ui->label5->resize(QPixmap(":/tariff/images/tariff/save/5.png").size());
    ui->label5->setPixmap(QPixmap(":/tariff/images/tariff/save/5.png"));

    ui->label6->resize(QPixmap(":/tariff/images/tariff/save/6.png").size());
    ui->label6->setPixmap(QPixmap(":/tariff/images/tariff/save/6.png"));

    //第二页

    ui->label7->resize(QPixmap(":/tariff/images/tariff/temp/1.png").size());
    ui->label7->setPixmap(QPixmap(":/tariff/images/tariff/temp/1.png"));

    ui->label8->resize(QPixmap(":/tariff/images/tariff/temp/2.png").size());
    ui->label8->setPixmap(QPixmap(":/tariff/images/tariff/temp/2.png"));

    //第三页

    ui->label9->resize(QPixmap(":/tariff/images/tariff/usersave/1.png").size());
    ui->label9->setPixmap(QPixmap(":/tariff/images/tariff/usersave/1.png"));

    ui->label10->resize(QPixmap(":/tariff/images/tariff/usersave/2.png").size());
    ui->label10->setPixmap(QPixmap(":/tariff/images/tariff/usersave/2.png"));

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabPageChange_slots(int)));

    tc =  QTextCodec::codecForName("UTF-8");
}

void tariffPage::readInfo()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabIcon(2,QIcon(":/tariff/images/tariff/3-3.png"));
    ui->tabWidget->setTabIcon(1,QIcon(":/tariff/images/tariff/2-2.png"));
    ui->tabWidget->setTabIcon(0,QIcon(":/tariff/images/tariff/1.png"));

    //第一页
    ui->label1_1->setText(getNameFrom());
    ui->label2_2->setText(QString::fromUtf8("%1元").arg(QString::number(SCTerminal::instance()->terminalInfo.userBasicTariff)));
    ui->label3_3->setText(QString::fromUtf8("%1元").arg(QString::number(SCTerminal::instance()->terminalInfo.bigTariff)));
    ui->label4_4->setText(QString::fromUtf8("%1元").arg(QString::number(SCTerminal::instance()->terminalInfo.middleTariff)));
    ui->label5_5->setText(QString::fromUtf8("%1元").arg(QString::number(SCTerminal::instance()->terminalInfo.smallTariff)));
    if(SCTerminal::instance()->terminalInfo.expireStatus == 1)
    {
        ui->label6_6->setText(QString::fromUtf8("不收取滞留费用。"));
    }
    else
    {
        ui->label6_6->setText(QString::fromUtf8("超过%1小时之后每过%2小时收取%3元").arg(SCTerminal::instance()->terminalInfo.expireHours) \
                              .arg(SCTerminal::instance()->terminalInfo.expireStep).arg(QString::number(SCTerminal::instance()->terminalInfo.expireStepMoney)));
    }

    //第二页
    ui->label7_7->setText(QString::fromUtf8("%1元").arg(QString::number(SCTerminal::instance()->terminalInfo.storageStartTariff)));
    ui->label8_8->setText(QString::fromUtf8("超过%1小时之后每过%2小时收取%3元").arg(SCTerminal::instance()->terminalInfo.storageHours) \
                          .arg(SCTerminal::instance()->terminalInfo.storageStep).arg(QString::number(SCTerminal::instance()->terminalInfo.storageStepMoney)));

    //第三页
    ui->label9_9->setText(QString::fromUtf8("%1元").arg(QString::number(SCTerminal::instance()->terminalInfo.sendBaseTariff)));
    ui->label10_10->setText(QString::fromUtf8("%1元").arg(QString::number(SCTerminal::instance()->terminalInfo.expGetTariff)));

}

void tariffPage::quitBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void tariffPage::tabPageChange_slots(int id)
{
    ui->tabWidget->currentWidget()->setFocus();
    if(2 == id)
    {
        ui->tabWidget->setTabIcon(2,QIcon(":/tariff/images/tariff/3.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/tariff/images/tariff/2-2.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/tariff/images/tariff/1-1.png"));
    }
    else if(1 == id)
    {
        ui->tabWidget->setTabIcon(2,QIcon(":/tariff/images/tariff/3-3.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/tariff/images/tariff/2.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/tariff/images/tariff/1-1.png"));
    }
    else if(0 == id)
    {
        ui->tabWidget->setTabIcon(2,QIcon(":/tariff/images/tariff/3-3.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/tariff/images/tariff/2-2.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/tariff/images/tariff/1.png"));
    }
}

QString tariffPage::getNameFrom()
{
    QString mystr;
    int temp  = SCTerminal::instance()->terminalInfo.tariffType;
    switch(temp)
    {
    case 1:
        mystr = QString::fromUtf8("不收费");
        ui->label2->hide();
        ui->label2_2->hide();
        ui->label3->hide();
        ui->label3_3->hide();
        ui->label4->hide();
        ui->label4_4->hide();
        ui->label5->hide();
        ui->label5_5->hide();
        break;
    case 2:
        mystr = QString::fromUtf8("快递员收费");
        ui->label2->hide();
        ui->label2_2->hide();
        ui->label3->show();
        ui->label3_3->show();
        ui->label4->show();
        ui->label4_4->show();
        ui->label5->show();
        ui->label5_5->show();
        break;
    case 3:
        mystr = QString::fromUtf8("用户收费");
        ui->label2->show();
        ui->label2_2->show();
        ui->label3->hide();
        ui->label3_3->hide();
        ui->label4->hide();
        ui->label4_4->hide();
        ui->label5->hide();
        ui->label5_5->hide();
        break;
   default:
        mystr = QString::fromUtf8("双向收费");
        ui->label2->show();
        ui->label2_2->show();
        ui->label3->show();
        ui->label3_3->show();
        ui->label4->show();
        ui->label4_4->show();
        ui->label5->show();
        ui->label5_5->show();
        break;
    }
    return mystr;
}
