#ifndef SENDFILE_H
#define SENDFILE_H

#include <QThread>
#include "singleton.h"
#include "servercomm.h"
#include "mainctrl.h"
#include "db.h"
#include <QFile>
#include <QTime>
#include <QDebug>

#define FILE_SLEEP_TIME  35*60

class CSendFileThrd : QThread
{
    Q_OBJECT

public:

    void init()
    {
        start();
    }


private:
    CSendFileThrd(){}
    ~CSendFileThrd(){}

    bool performStatus;

    void sendFileProc()
    {

        QTime time = QTime::currentTime();
        int hour = time.hour();
        QString fileName = "debuglog.txt";
        QString dbName = "database.sqlite";

        if(4 != hour)
        {
            performStatus = true;
        }
        if((4 == hour) && performStatus)
        {

           int filetime = qrand()%15;
           qDebug() << "start send file" << filetime;
           sleep(filetime*60);
           QFile filesize("debuglog.txt");

           if(filesize.size() > 10*1024*1024)
           {
               filesize.resize(0);
           }
           heartBeatReportRet ret;
           SCServerComm::instance()->uploadDbFiles(dbName,&ret);
           if(ret.resultCode() == RET_TRUE)
           {
              performStatus = false;
           }

           heartBeatReportRet respone;
           SCServerComm::instance()->uploadDebugFiles(fileName,&respone);
           if(respone.resultCode() == RET_TRUE)
           {
              performStatus = false;
              filesize.resize(0);
           }
        }
    }

    void run()
    {
        qsrand(time(0));
        performStatus = true;
        while(1)
        {

            sleep(FILE_SLEEP_TIME);

            if((SCMainCtrl::instance()->curStatus() == 1))
            {
                sendFileProc();
            }
        }
    }
    DECLARE_SINGLETON_CLASS(CSendFileThrd)
};

typedef Singleton<CSendFileThrd> SCSendFileThrd;
#endif // SENDFILE_H
