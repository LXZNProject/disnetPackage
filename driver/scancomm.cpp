#include "scancomm.h"
#include <QDebug>
scancomm::scancomm(QObject *parent) :
    QObject(parent)
{
    myCom = NULL;
}

void scancomm::init()
{
    if(myCom)
    {
        myCom->close();
        myCom = NULL;
    }
    comName = "/dev/ttyO2";

    myCom = new QextSerialPort(comName);
    myCom->open(QIODevice::ReadWrite);
    myCom->setBaudRate(BAUD9600);
    myCom->setDataBits(DATA_8);
    myCom->setParity(PAR_NONE);
    myCom->setStopBits(STOP_1);
    myCom->setFlowControl(FLOW_OFF);
    myCom->setTimeout(0,100);

    QByteArray startByte;
    QString startStr = "$$$$#99900031;#99900114;#99900032;";
    startByte.append(startStr.toAscii());

    myCom->write(startByte);
    myCom->readAll();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(readMyCom()));
}

void scancomm::openMyCom()
{
    myCom->readAll();
    if(!timer->isActive())
    {
        timer->start(800);
    }

}

void scancomm::closeReadCom()
{
    timer->stop();
}

void scancomm::readMyCom()
{
    QByteArray endData;
    endData.append(0x0D);
    endData.append(0x0A);
    QByteArray temp = myCom->readAll();

    qDebug() << "readMyCom:"<< temp;
    if(!temp.isEmpty() && endData.contains(temp.right(2)))
    {
        temp.chop(2);
        QString scan = QString::fromAscii(temp.data());
        emit scanSignal(scan);
        qDebug() << "scan:"<< scan;
        timer->stop();
    }

}
