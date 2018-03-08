#include "savepackageliststatus.h"
#include "mainctrl.h"
#include <QObject>

CSavepackagelistStatus::CSavepackagelistStatus(STATUS status) :
     CStatus(status)
{
    savelistpage = new savepackagelistpage();

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeOutComing()));
    connect(this->savelistpage, SIGNAL(timeStop()), this, SLOT(timeStop()));
}


void CSavepackagelistStatus::enter(QByteArray param)
{
    mTimer.setSingleShot(true);
    mTimer.setInterval(120*1000);

    QString isShowList;
    QStringList cardInfo;
    QStringList notOpenBoxList;
    QStringList notCheckBoxList;
    QString notCloseBox;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> isShowList >> cardInfo >> notOpenBoxList >> notCheckBoxList >> notCloseBox;

    savelistpage->setExceptionBoxInfo(cardInfo,notOpenBoxList,notCheckBoxList,notCloseBox);

    if(isShowList == "ADD_PACK")//||  savelistpage->daemonListSize() //不需要增加信息直接发送
    {
        savelistpage->readListInfo(param);
        savelistpage->show();
    }
    else if(isShowList == "ADD_WAIT_PACK")
    {
        savelistpage->readListInfo(param);
        mTimer.setInterval(60);
        savelistpage->hide();
    }
    else if(isShowList == "NO_PACK")
    {
  //      qDebug() << "isShowList:" << "NO_ADD_SHOW";
        mTimer.setInterval(10);
        savelistpage->hide();
    }
    else
    {
        mTimer.setInterval(10);
        savelistpage->hide();
    }

    mTimer.start();
}

void CSavepackagelistStatus::leave(QByteArray param)
{
    mTimer.stop();
  //  savelistpage->hide();
    QTimer::singleShot(800, this, SLOT(leaveSlots()));
 //   qDebug() << "leave save ";
}

void CSavepackagelistStatus::leaveSlots()
{
    savelistpage->hide();
    //qDebug() <<  QTime::currentTime().toString();

}

void CSavepackagelistStatus::timeOutComing()
{
    savelistpage->okBtn_slots();
}

void CSavepackagelistStatus::timeStop()
{
    mTimer.stop();
}
