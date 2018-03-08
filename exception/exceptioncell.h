#ifndef EXCEPTIONCELL_H
#define EXCEPTIONCELL_H

#include <QThread>
#include "t_cabinet_cell.h"
#include "mainctrl.h"
#include "servercomm.h"
#include "networkthread.h"
#include <QDebug>


#define EXCEPTIONCELL_PROC_TIME  1*60*60

class CExceptionCellThrd : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        start();
    }

private:
    CExceptionCellThrd(){}
    ~CExceptionCellThrd(){}


    void run()
    {
        while(1)
        {
            sleep(EXCEPTIONCELL_PROC_TIME);
            qDebug() << "--info--: exceptioncell thread proccess ...";        
            notCloseExceptionProc();

        }

    }


    void notCloseExceptionProc()
    {
          QList<NetCellExcept> cellList = ST_cabinet_cell::instance()->getNotCloseCellInfo();

          for(int i = 0; i < cellList.size(); i++)
          {
              QList<NetCellExcept> expList ;
              NetCellExcept cellExp = cellList.at(i);
              expList.append(cellExp);
              NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->reportCellError(expList);
              SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);

          }

//          if(!cellList.isEmpty())
//          {
//              NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->reportCellError(cellList);
//              SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);
//          }
    }




    DECLARE_SINGLETON_CLASS(CExceptionCellThrd)
};

typedef Singleton<CExceptionCellThrd> SCExceptionCellThrd;
#endif // EXCEPTIONCELL_H
