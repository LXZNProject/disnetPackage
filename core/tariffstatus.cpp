#include "tariffstatus.h"

CtariffStatus::CtariffStatus(STATUS status):
    CStatus(status)
{
    mtariffPage =  new tariffPage();

    connect(mtariffPage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CtariffStatus::enter(QByteArray param)
{
    openTimer();
    mtariffPage->readInfo();
    mtariffPage->show();
}

void CtariffStatus::leave(QByteArray param)
{
    stopTimer();
    mtariffPage->hide();
}

void CtariffStatus::cardInputProc(QByteArray param)
{
}

void CtariffStatus::resetStart()
{
    clearTimer();
}

void CtariffStatus::jumpPage()
{
    mtariffPage->quitBtn_slots();
}
