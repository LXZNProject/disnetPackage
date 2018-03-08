#include "exptakestatus.h"

CExpTakeStatus::CExpTakeStatus(STATUS status):
    CStatus(status)
{
    page = new exptakepage();

    connect(page,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CExpTakeStatus::enter(QByteArray param)
{
    openTimer();
    page->readExpTakePageInfo(param);
    page->show();
}

void CExpTakeStatus::leave(QByteArray param)
{
    stopTimer();
    page->hide();
}

void CExpTakeStatus::resetStart()
{
    clearTimer();
}

void CExpTakeStatus::jumpPage()
{
    page->quitBtn_slots();
}
