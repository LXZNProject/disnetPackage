#include "tempstoragewaitstatus.h"

CtempStorageWaitStatus::CtempStorageWaitStatus(STATUS status):
    CStatus(status)
{
    page = new tempStorageWait();
}
void CtempStorageWaitStatus::enter(QByteArray param)
{
    openTimer();
    page->readWaitInfo(param);
    page->show();
}

void CtempStorageWaitStatus::leave(QByteArray param)
{
    stopTimer();
    page->hide();
}

void CtempStorageWaitStatus::coinInputProc(QByteArray param)
{

}
