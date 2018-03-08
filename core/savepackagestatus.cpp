#include "savepackagestatus.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDataStream>
#include "driver.h"
#include <QDebug>

CSavepackageStatus::CSavepackageStatus(STATUS status):
    CStatus(status)
{
    savepage = new savepackage();

    connect(&mTimer,SIGNAL(timeout()),this,SLOT(timeOutComing()));
    connect(savepage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CSavepackageStatus::enter(QByteArray param)
{
    openTimer();
    data = param;
    mTimer.setSingleShot(true);
    mTimer.setInterval(100);

    qDebug() << QString(param);
    savepage->readShowInfo(param);
    savepage->show();
    mTimer.start();
}



void CSavepackageStatus::leave(QByteArray param)
{
    stopTimer();

   // savepackage->hide();
    QTimer::singleShot(800, this, SLOT(leaveSlots()));
}

void CSavepackageStatus::leaveSlots()
{
    savepage->hide();
}

void CSavepackageStatus::timeOutComing()
{
    QString isLong;
    QDataStream in(&data,QIODevice::ReadOnly);
    in >> isLong;
    if(isLong != "WAIT")
    {
        savepage->loadcodeScan();
    }

}


void CSavepackageStatus::jumpPage()
{
    savepage->quitBtn_slots();
}

void CSavepackageStatus::resetStart()
{
    clearTimer();
}

void CSavepackageStatus::coinInputProc(QByteArray param)
{
}


void CSavepackageStatus::cardInputProc(QByteArray param)
{
}

