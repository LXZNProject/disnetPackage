#include "tempstoragestatus.h"

CtempStorageStatus::CtempStorageStatus(STATUS status):
    CStatus(status)
{
    tempStoragePage = new tempStorage();
    connect(tempStoragePage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}
void CtempStorageStatus::enter(QByteArray param)
{
    openTimer();
    tempStoragePage->readShowInfo();
    tempStoragePage->show();
}

void CtempStorageStatus::leave(QByteArray param)
{
    stopTimer();
    tempStoragePage->hide();
}

void CtempStorageStatus::cardInputProc(QByteArray param)
{
}

void CtempStorageStatus::resetStart()
{
    clearTimer();
}

void CtempStorageStatus::jumpPage()
{
    tempStoragePage->quitBtn_slots();
}

