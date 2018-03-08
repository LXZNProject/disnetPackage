#include "serverstatus.h"
#include <QTimer>

CServerStatus::CServerStatus(STATUS status):
    CStatus(status)
{
    page = new serverpage();
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CServerStatus::enter(QByteArray param)
{
    openTimer();
    page->show();
}

void CServerStatus::leave(QByteArray param)
{
    stopTimer();

    QTimer::singleShot(60, this, SLOT(leaveSlots()));
}

void CServerStatus::jumpPage()
{
    page->quitBtn_slots();
}

void CServerStatus::leaveSlots()
{
     page->hide();
}
