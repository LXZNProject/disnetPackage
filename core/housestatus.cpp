#include "housestatus.h"

CHouseStatus::CHouseStatus(STATUS status):
    CStatus(status)
{
    page = new housepage();

    connect(page,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CHouseStatus::enter(QByteArray param)
{
    openTimer();
    page->readShowInfo(param);
    page->show();
}

void CHouseStatus::leave(QByteArray param)
{
    stopTimer();
    page->hide();
}


void CHouseStatus::cardInputProc(QByteArray param)
{
}

void CHouseStatus::resetStart()
{
    clearTimer();
}

void CHouseStatus::jumpPage()
{
    page->quitBtn_slots();
}
