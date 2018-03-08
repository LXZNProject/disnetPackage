#include "assiststatus.h"

CAssistStatus::CAssistStatus(STATUS status):
    CStatus(status)
{
    page = new assistpage();

    connect(page,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CAssistStatus::enter(QByteArray param)
{
    openTimer();
    page->show();
}

void CAssistStatus::leave(QByteArray param)
{
    stopTimer();
    page->hide();
}

void CAssistStatus::resetStart()
{
    clearTimer();
}

void CAssistStatus::jumpPage()
{
    page->quitBtn_slots();
}
