#include "rechargedialogstatus.h"
#include <QObject>
#include <QTimer>
CRechargedialogStatus::CRechargedialogStatus(STATUS status):
    CStatus(status)
{
    rechargeDialogPage = new rechargeDialog();
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CRechargedialogStatus::enter(QByteArray param)
{
    openTimer();
    rechargeDialogPage->show();

}

void CRechargedialogStatus::leave(QByteArray param)
{
    stopTimer();

    QTimer::singleShot(600, this, SLOT(leaveSlots()));

}

void CRechargedialogStatus::leaveSlots()
{
    rechargeDialogPage->hide();
}


void CRechargedialogStatus::jumpPage()
{
    rechargeDialogPage->quitBtn_slots();
}


