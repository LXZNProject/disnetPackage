#include "housewaitstatus.h"

CHouseWaitStatus::CHouseWaitStatus(STATUS status):
    CStatus(status)
{
    page = new housewaitpage();

}

void CHouseWaitStatus::enter(QByteArray param)
{
    openTimer();
    page->readWaitInfo(param);
    page->show();
}

void CHouseWaitStatus::leave(QByteArray param)
{
    stopTimer();
    page->hide();
}

void CHouseWaitStatus::coinInputProc(QByteArray param)
{
}
