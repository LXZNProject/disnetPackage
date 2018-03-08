#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QThread>
#include "lockdrv.h"
#include "mainctrl.h"
#include "t_cabinet_cell.h"
#include "networkthread.h"
#include "servercomm.h"
#include <QDateTime>
#include <QDebug>

#define EXCEPTION_PROC_INTERVAL  1*60

class CExceptionThrd : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        start();
    }  

private:
    CExceptionThrd(){}
    ~CExceptionThrd(){}

    CLockDrv lockoperation;  

    void run()
    {
        while(1)
        {
            sleep(EXCEPTION_PROC_INTERVAL);
            qDebug() << "--info--: exception thread proccess ...";
      
            if (SCMainCtrl::instance()->curStatus() == 1)
            {
               openExceptionProc();

            }
        }

    }


    void openExceptionProc()
    {
        QList<cellError> celllist;
        ST_cabinet_cell::instance()->queryOpenCellError(celllist);
       // qDebug() << "--info--: open Exception, size " << celllist.size();  

        qint8 cellStatus = 1;
        for (int i = 0; i < celllist.size(); i++)
        {
            qint8 cabinetId = celllist.at(i).cellId.left(2).toInt();
            qint8 cellId = celllist.at(i).cellId.right(2).toInt();

            qint8 retQueryValue = lockoperation.queryLock(cabinetId, cellId);

            if(retQueryValue == 0)
            {
                cellError tempValue;
                tempValue = celllist.at(i);
                tempValue.repairtime = QDateTime::currentDateTime();
                tempValue.status = 2;
                ST_cabinet_cell::instance()->insertCellError(tempValue);
                ST_cabinet_cell::instance()->changeCellStatus(tempValue.cellId,cellStatus);

                if(tempValue.submit == 2)
                {
                    NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->repairCellError(tempValue.cellId,3);
                    SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);
                }

            }
        }

    }




    DECLARE_SINGLETON_CLASS(CExceptionThrd)
};

typedef Singleton<CExceptionThrd> SExceptionThrd;

#endif // EXCEPTION_H
