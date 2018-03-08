#include "senderwaitstatus.h"

CSenderWaitStatus::CSenderWaitStatus(STATUS status):
    CStatus(status)
{
    page = new senderwaitpage();
}

void CSenderWaitStatus::enter(QByteArray param)
{
    openTimer();
    page->readWaitInfo(param);
    page->show();
}

void CSenderWaitStatus::leave(QByteArray param)
{
    stopTimer();
    page->hide();
}

void CSenderWaitStatus::coinInputProc(QByteArray param)
{
}
