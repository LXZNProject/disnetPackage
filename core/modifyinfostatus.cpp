#include "modifyinfostatus.h"
#include <QObject>
CModifyInfoStatus::CModifyInfoStatus(STATUS status):
    CStatus(status)
{
    modifyPage = new modifyINfoPage();
    connect(modifyPage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CModifyInfoStatus::enter(QByteArray param)
{
    openTimer();
    modifyPage->show();
    modifyPage->timeStart();
}

void CModifyInfoStatus::leave(QByteArray param)
{
    stopTimer();
    modifyPage->hide();
}

void CModifyInfoStatus::resetStart()
{
    clearTimer();
}

void CModifyInfoStatus::jumpPage()
{
    modifyPage->quitBtn_slots();
}
