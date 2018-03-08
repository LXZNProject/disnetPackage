#include "takepackagestatus.h"
#include <QApplication>
#include <QKeyEvent>

CTakePageStatus::CTakePageStatus(STATUS status):
     CStatus(status)
{

   getpackage = new takepackagepage;

   connect(getpackage,SIGNAL(resetTime()),this,SLOT(resetStart()));
 //  connect(this, SIGNAL(coincoming()), getpackage,SLOT(payCoin_slot()));
   connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
 //  connect(this,SIGNAL(cardcoming(QString)),getpackage,SLOT(payCard_slot(QString)));
   connect(&queryCardTimer,SIGNAL(timeout()),getpackage,SLOT(queryCardInfo_slots()));

}

void CTakePageStatus::enter(QByteArray param)
{
 //   qDebug() << param <<"CTake is coming";

    openTimer();

    queryCardTimer.start(1000);

    getpackage->readTakePackInfo();
    getpackage->show();

}


void CTakePageStatus::leave(QByteArray param)
{
    stopTimer();
 //   qDebug() << param << "CTake is leave";

  // QTimer::singleShot(1000, this, SLOT(leaveSlots()));
    queryCardTimer.stop();
    getpackage->hide();
   // delete getpackage;
   // getpackage = NULL;
}


void CTakePageStatus::leaveSlots()
{
    getpackage->hide();
}


void CTakePageStatus::cardInputProc(QByteArray param)
{
    clearTimer();
    //getpackage->payCard(QString(param));
   // emit cardcoming(QString(param));
}


void CTakePageStatus::resetStart()
{
    clearTimer();
}

void CTakePageStatus::jumpPage()
{
    getpackage->quitBtn_slots();
}

