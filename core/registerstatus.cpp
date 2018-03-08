#include "registerstatus.h"
CRegisterStatus::CRegisterStatus(STATUS status):
    CStatus(status)
{
    registerpage = new registerPackage();
    connect(registerpage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CRegisterStatus::enter(QByteArray param)
{
    openTimer();
    registerpage->readInfo(param);
    registerpage->show();
}

void CRegisterStatus::leave(QByteArray param)
{
    stopTimer();
    registerpage->hide();
}

void CRegisterStatus::resetStart()
{
    clearTimer();
}

void CRegisterStatus::jumpPage()
{
    registerpage->quitBtn_slots();
}


