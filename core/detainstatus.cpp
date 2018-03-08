#include "detainstatus.h"

CDetainStatus::CDetainStatus(STATUS status):
    CStatus(status)
{
    page = new detainpage();

    connect(page,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CDetainStatus::enter(QByteArray param)
{
    openTimer();
    page->readDetainInfo(param);
    page->show();
}

void CDetainStatus::leave(QByteArray param)
{
    stopTimer();
    page->hide();
}

void CDetainStatus::resetStart()
{
    clearTimer();
}

void CDetainStatus::jumpPage()
{
    page->quitBtn_slots();
}
