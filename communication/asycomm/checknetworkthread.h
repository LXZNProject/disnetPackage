#ifndef CHECKNETWORKTHREAD_H
#define CHECKNETWORKTHREAD_H

#include <QThread>
#include "singleton.h"
#include "mainctrl.h"
#include "t_log_network.h"
#include "t_basic_terminal.h"
#include "networkthread.h"
#include <QDebug>

#define CHECK_NETWORK_TIME 3*60

class CheckNetWorkThread : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        start();
    }


private:
    CheckNetWorkThread(){}
    ~CheckNetWorkThread(){}

    void run()
    {
        bool ret = true;
        while(1)
        {
            sleep(CHECK_NETWORK_TIME);
            qDebug() << "--info--: check network thread proccess ...";

            QTime time = QTime::currentTime();
            int hour = time.hour();
            if(hour != 3)
            {
                ret = true;
            }
            if((hour == 3) && ret)
            {
                networkBreakProc();
                ret = false;
            }
            networkExceptionProc();
        }

    }

    void networkExceptionProc()
    {
        QList<NetworkDesc_t> networkList;
        SCNetwork::instance()->queryNetworkInfo(networkList,2);

        for(int i = 0; i < networkList.size(); i++)
        {
            SCNetworkThread::instance()->sendNetWorkEvent(networkList.at(i));
        }
    }

    void networkBreakProc()
    {
        QList<NetworkDesc_t> networkList;
        SCNetwork::instance()->queryNetworkInfo(networkList,3);

        for(int i = 0; i < networkList.size(); i++)
        {
            SCNetworkThread::instance()->sendNetWorkEvent(networkList.at(i));
        }
    }



    DECLARE_SINGLETON_CLASS(CheckNetWorkThread)
};

typedef Singleton<CheckNetWorkThread> SCheckNetWorkThread;


#endif // CHECKNETWORKTHREAD_H
