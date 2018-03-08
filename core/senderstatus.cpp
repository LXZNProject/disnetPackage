#include "senderstatus.h"

CSenderStatus::CSenderStatus(STATUS status):
    CStatus(status)
{
    page = new senderpage();

    connect(page,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CSenderStatus::enter(QByteArray param)
{
     openTimer();
     page->readShowInfo(param);
     page->show();
}

void CSenderStatus::leave(QByteArray param)
{
    stopTimer();
    page->hide();
}

void CSenderStatus::coinInputProc(QByteArray param)
{
    clearTimer();
}

void CSenderStatus::cardInputProc(QByteArray param)
{
}

void CSenderStatus::resetStart()
{
    clearTimer();
}

void CSenderStatus::jumpPage()
{
    page->quitBtn_slots();
}
