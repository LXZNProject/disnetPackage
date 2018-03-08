#include "waitconfirmstatus.h"
#include "mainctrl.h"
#include <QKeyEvent>
#include <QApplication>

CWaitconfirmStatus::CWaitconfirmStatus(STATUS status):
        CStatus(status)
{
    waitconfirm = new waitconfirmpage;

    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CWaitconfirmStatus::enter(QByteArray param)
{
    openTimer();

    waitconfirm->readwaitconfirminfo(param);
    waitconfirm->show();


}

void CWaitconfirmStatus::leave(QByteArray param)
{
    stopTimer();
 //   qDebug() << param;
    waitconfirm->hide();
    //delete waitconfirm;
   // waitconfirm = NULL;
}

void CWaitconfirmStatus::jumpPage()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
