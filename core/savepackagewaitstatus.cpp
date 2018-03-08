#include "savepackagewaitstatus.h"

CSavepackagewaitStatus::CSavepackagewaitStatus(STATUS status):
    CStatus(status)
{
    savewaitpage = new savepackagewaitpage;
	connect(&mTimer,SIGNAL(timeout()),this,SLOT(leaveSlots()));
}


void CSavepackagewaitStatus::enter(QByteArray param)
{
    savewaitpage->readWaitInfo(param);
    savewaitpage->show();

}

void CSavepackagewaitStatus::leave(QByteArray param)
{
   // qDebug() << "leave save ";
    mTimer.setSingleShot(true);
    mTimer.setInterval(800);
    mTimer.start();
    //savewaitpage->hide();
    //delete savepackage;
   // savepackage = NULL;
}

void CSavepackagewaitStatus::leaveSlots()
{
	savewaitpage->hide();
}
