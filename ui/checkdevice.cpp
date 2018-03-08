#include "checkdevice.h"
#include "ui_checkdevice.h"
#include "t_cabinet_cell.h"
#include "driver.h"
#include "mainctrl.h"
#include "portcomm.h"
#include "networkinit.h"
#include <QKeyEvent>
#include <QEvent>
#include <QBitmap>
#include "scancomm.h"
checkDevice::checkDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::checkDevice)
{
    ui->setupUi(this);
    initPage();
}

checkDevice::~checkDevice()
{
    delete ui;
}

void checkDevice::readInfo()
{
    ui->cabinetLineEdit->setFocus();
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void checkDevice::checkScan(QString param)
{
    emit resetTime();
    if(ui->scanLineEdit == this->focusWidget())
    {
        ui->scanLineEdit->setText(param);
    }
}

void checkDevice::initPage()
{

    tc =  QTextCodec::codecForName("UTF-8");

    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap pixmap(":/checkDevice/images/checkDevice/background.png");
    this->setMask(pixmap.mask());
    this->resize(pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    ui->cabinetLabel->resize(QPixmap(":/checkDevice/images/checkDevice/cabinetLabel.png").size());
    ui->cabinetLabel->setPixmap(QPixmap(":/checkDevice/images/checkDevice/cabinetLabel.png"));

    ui->cellLabel->resize(QPixmap(":/checkDevice/images/checkDevice/cellLabel.png").size());
    ui->cellLabel->setPixmap(QPixmap(":/checkDevice/images/checkDevice/cellLabel.png"));

    ui->networkLabel->resize(QPixmap(":/checkDevice/images/checkDevice/networkLabel.png").size());
    ui->networkLabel->setPixmap(QPixmap(":/checkDevice/images/checkDevice/networkLabel.png"));

    ui->toubiLabel->resize(QPixmap(":/checkDevice/images/checkDevice/toubiLabel.png").size());
    ui->toubiLabel->setPixmap(QPixmap(":/checkDevice/images/checkDevice/toubiLabel.png"));

    ui->scanLabel->resize(QPixmap(":/checkDevice/images/checkDevice/scanLabel.png").size());
    ui->scanLabel->setPixmap(QPixmap(":/checkDevice/images/checkDevice/scanLabel.png"));

    ui->timerLabel->resize(QPixmap(":/checkDevice/images/checkDevice/timerLabel.png").size());
    ui->timerLabel->setPixmap(QPixmap(":/checkDevice/images/checkDevice/timerLabel.png"));

    ui->readCardLabel ->resize(QPixmap(":/checkDevice/images/checkDevice/readCardLabel.png").size());
    ui->readCardLabel->setPixmap(QPixmap(":/checkDevice/images/checkDevice/readCardLabel.png"));

    ui->cabinetBtn->setFlat(true);
    ui->cabinetBtn->setFocusPolicy(Qt::NoFocus);
    ui->cabinetBtn->setIcon(QIcon(":/checkDevice/images/checkDevice/ok.png"));
    ui->cabinetBtn->setMinimumSize(QPixmap(":/checkDevice/images/checkDevice/ok.png").size());
    ui->cabinetBtn->setIconSize(ui->cabinetBtn->size());
    ui->cabinetBtn->setStyleSheet("background-color:transparent");

    ui->cellBtn->setFlat(true);
    ui->cellBtn->setFocusPolicy(Qt::NoFocus);
    ui->cellBtn->setIcon(QIcon(":/checkDevice/images/checkDevice/ok.png"));
    ui->cellBtn->setMinimumSize(QPixmap(":/checkDevice/images/checkDevice/ok.png").size());
    ui->cellBtn->setIconSize(ui->cellBtn->size());
    ui->cellBtn->setStyleSheet("background-color:transparent");

    ui->networkBtn->setFlat(true);
    ui->networkBtn->setFocusPolicy(Qt::NoFocus);
    ui->networkBtn->setIcon(QIcon(":/checkDevice/images/checkDevice/ok.png"));
    ui->networkBtn->setMinimumSize(QPixmap(":/checkDevice/images/checkDevice/ok.png").size());
    ui->networkBtn->setIconSize(ui->networkBtn->size());
    ui->networkBtn->setStyleSheet("background-color:transparent");

    ui->timerBtn->setFlat(true);
    ui->timerBtn->setFocusPolicy(Qt::NoFocus);
    ui->timerBtn->setIcon(QIcon(":/checkDevice/images/checkDevice/ok.png"));
    ui->timerBtn->setMinimumSize(QPixmap(":/checkDevice/images/checkDevice/ok.png").size());
    ui->timerBtn->setIconSize(ui->timerBtn->size());
    ui->timerBtn->setStyleSheet("background-color:transparent");

    ui->readCardBtn->setFlat(true);
    ui->readCardBtn->setFocusPolicy(Qt::NoFocus);
    ui->readCardBtn->setIcon(QIcon(":/checkDevice/images/checkDevice/ok.png"));
    ui->readCardBtn->setMinimumSize(QPixmap(":/checkDevice/images/checkDevice/ok.png").size());
    ui->readCardBtn->setIconSize(ui->readCardBtn->size());
    ui->readCardBtn->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");


    ui->toubiLineEdit->setText("0");
    ui->scanLineEdit->installEventFilter(this);

    connect(ui->cabinetBtn,SIGNAL(clicked()),this,SLOT(cabinetBtn_slots()));
    connect(ui->cellBtn,SIGNAL(clicked()),this,SLOT(cellBtn_slots()));
    connect(ui->networkBtn,SIGNAL(clicked()),this,SLOT(networkBtn_slots()));
    connect(ui->timerBtn,SIGNAL(clicked()),this,SLOT(timerBtn_slots()));
    connect(ui->readCardBtn,SIGNAL(clicked()),this,SLOT(readCardBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
    connect(ui->scanLineEdit,SIGNAL(editingFinished()),SCScanComm::instance(),SLOT(closeReadCom()));
    connect(SCScanComm::instance(),SIGNAL(scanSignal(QString)),this,SLOT(checkScan(QString)));
}

void checkDevice::clearInfo()
{
    ui->cabinetLineEdit->clear();
    ui->cellLineEdit->clear();
    ui->networkLineEdit->clear();
    ui->toubiLineEdit->clear();
    ui->scanLineEdit->clear();
    ui->readCardLineEdit->clear();
}

void checkDevice::cabinetBtn_slots()
{
    emit resetTime();
    setBtnEnable(false);
    QString cabinetNumStr = ui->cabinetLineEdit->text().trimmed();
    if(cabinetNumStr.size()!=2)
    {
        myMsgBox.showMsgBox("请出入正确的柜子号。");
        setBtnEnable(true);
        return;
    }

    for(int i = 0; i  < 20; i++)
    {
        qint8 cabinetId = cabinetNumStr.toInt();
        qint8 cellId =  i + 1;
        lockoperation.openLock(cabinetId,cellId);
    }
    myMsgBox.showMsgBox("打开柜子号完毕。");


    setBtnEnable(true);
}

void checkDevice::cellBtn_slots()
{
    emit resetTime();
    setBtnEnable(false);
    QString cellNumStr = ui->cellLineEdit->text().trimmed();
    if(cellNumStr.size()!=4)
    {
        myMsgBox.showMsgBox("请出入正确的格门号。");
        setBtnEnable(true);
        return;
    }
    else
    {
        bool ok=0;
        qint8 cabinetId = cellNumStr.left(2).toInt(&ok,10);
        qint8 cellId = cellNumStr.right(2).toInt(&ok,10);
        if(!lockoperation.openLock(cabinetId,cellId))
        {
            myMsgBox.showMsgBox("箱门打不开。");
        }
    }
    setBtnEnable(true);
}

void checkDevice::networkBtn_slots()
{
    emit resetTime();
    setBtnEnable(false);
    ui->networkBtn->setEnabled(false);
    ui->networkLineEdit->setText(tc->toUnicode("等待结果..."));

    QStringList tempList = SCNetworkInit::instance()->getNetwork();
    QString pingIP = tempList.at(0);
    pingIP = "ping -c 1 -s 0 "+ pingIP +" > /dev/null";
    int returnValue = system(pingIP.toStdString().c_str());

    if (WEXITSTATUS( returnValue ) == 0)
    {
        ui->networkLineEdit->setText(tc->toUnicode("网络正常"));
    }
    else
    {
        ui->networkLineEdit->setText(tc->toUnicode("网络异常"));
    }
    ui->networkBtn->setEnabled(true);
    setBtnEnable(true);
}

bool checkDevice::event(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *myEvent = static_cast<QKeyEvent *>(event);
        if(myEvent->key() == Qt::Key_Insert)
        {
            int count = ui->toubiLineEdit->text().toInt();
            ui->toubiLineEdit->setText(QString("%1").arg(++count));
        }
    }
    return  QWidget::event(event);
}

bool checkDevice::eventFilter(QObject * obj, QEvent * event)
{
    if(obj == ui->scanLineEdit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            scan();
        }
    }
    return QWidget::eventFilter(obj,event);
}

void checkDevice::scan()
{
    emit resetTime();
    //播放音乐
    QString mp3file = "./music/scan.mp3";
    QString vol = "15";
    SCDriver::instance()->playVoice(mp3file,vol);
    ui->scanLineEdit->setFocus();
    SCScanComm::instance()->openMyCom();
}

void checkDevice::setBtnEnable(bool value)
{
    ui->cabinetBtn->setEnabled(value);
    ui->cellBtn->setEnabled(value);
    ui->networkBtn->setEnabled(value);
    ui->timerBtn->setEnabled(value);
    ui->quitBtn->setEnabled(value);
    ui->readCardBtn->setEnabled(value);
}

void checkDevice::timerBtn_slots()
{
    emit resetTime();
    setBtnEnable(false);
    QString clock = ui->dateTimeEdit->text();
    clock = "date -s \"" + clock + "\"";
    QByteArray clockData = clock.toLatin1();
    system(clockData.data());

    system("hwclock -u -w");
    myMsgBox.showMsgBox("设置完成。");
    setBtnEnable(true);
}

void checkDevice::quitBtn_slots()
{
    clearInfo();
    myMsgBox.close();
    SCMainCtrl::instance()->enterStatus(ST_HOME, "");
}

void checkDevice::readCardBtn_slots()
{
    emit resetTime();
    QString myshowStr;
    QStringList myReadDataList;
    myReadDataList.clear();

    if(SCPortComm::instance()->queryCardInfo(myReadDataList) >= 0)
    {
        myshowStr = tc->toUnicode("读卡成功,");
    }
    else
    {
        myshowStr =  tc->toUnicode("读卡失败,");
    }

    QString info;
    if(myReadDataList.size() > 6)
    {
        for(int i= 0;i < myReadDataList.size();i++)
        {
            info += (myReadDataList.at(i) + QString("|"));
        }
    }
    else
    {
        QString tempAdd = QString("null") + QString("|");

        for(int i = 0; i < 10;i++)
        {
            info += tempAdd;
        }
    }



    bool myResult = SCPortComm::instance()->writeCardInfo(info);
    if(myResult)
    {
        myshowStr +=  tc->toUnicode("写卡成功");
    }
    else
    {
        myshowStr += tc->toUnicode("写卡失败");
    }
    ui->readCardLineEdit->setText(myshowStr);
}

