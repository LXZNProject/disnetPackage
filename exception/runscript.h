#ifndef RUNSCRIPT_H
#define RUNSCRIPT_H

#include <QThread>
#include <QObject>
#include <QTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>

#include "singleton.h"
#include "mainctrl.h"
#include "networkinit.h"
#include "gprsreset.h"

#define CHECKNET_SLEEP_TIME  30
#define ERROR_TIMES        30

class runScriptThrd : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        start();
    }

signals:


private:
    runScriptThrd(){}
    ~runScriptThrd(){}

    void run()
    {
        int returnValue = 0;
        int errorTimes = 0;
        QString pingAddrStr;
        QString pingIP;
        QStringList tempList;

        QTime temptime = QTime::currentTime();

        qDebug() << "check netWork thread running";
        while(1)
        {
            tempList.clear();
            sleep(CHECKNET_SLEEP_TIME);
            tempList = SCNetworkInit::instance()->getNetwork();
            pingIP = tempList.at(0);
//            qDebug() << "runScriptThrd pingIP" << pingIP;
            pingAddrStr = "ping -c 1 -s 0 "+ pingIP +" > /dev/null";
            returnValue = system(pingAddrStr.toStdString().c_str());
            if (WEXITSTATUS( returnValue ) == 0)
            {
                errorTimes = 0;
            }
            else
            {
                errorTimes++;
                qDebug() << "errorTimes " << errorTimes;
                SCNetworkInit::instance()->connetNetwork();
            }

            if (SCMainCtrl::instance()->curStatus() == 1)
            {
                if(errorTimes >= 15)
                {
//                    qDebug() << "runScriptThrd start reboot errorTimes" << errorTimes ;
                    //errorTimes = 0;
                    //system("reboot");
                    if(SCNetworkInit::instance()->getNetworkMethods() == "1")
                    {
                        qDebug() << "runScriptThrd SGprsReset" ;
                        if(errorTimes >= ERROR_TIMES)
                        {
                            errorTimes = 0;
                            qDebug() << "network exception start reboot";
                            system("reboot");
                        }
                        SCNetworkInit::instance()->disconnectNetwork();
                        SGprsReset::instance()->reset();                       
                    }
                }

                //获取当前系统时间判断是否需要重启
                temptime = QTime::currentTime();
                if((temptime.hour()==4)&&(temptime.minute()>=59))
                {
//                    system("sed -i '/pppd call cdma/d' /etc/init.sh");
//                    system("sed -i '/ifconfig eth0 up/d' /etc/init.sh");
//                    system("sed -i '/udhcpc/d' /etc/init.sh");
                    qDebug() << "system start reboot";
                    system("reboot");
                }             
            }
        }
    }
    DECLARE_SINGLETON_CLASS(runScriptThrd)
};

typedef Singleton<runScriptThrd> SRunScriptThrd;

#endif // RUNSCRIPT_H
