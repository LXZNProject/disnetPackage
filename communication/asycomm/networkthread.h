#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QObject>
#include <QEvent>
#include "singleton.h"
#include "t_log_network.h"

static const QEvent::Type NET_COMM_TYPE = (QEvent::Type)QEvent::registerEventType(QEvent::User + 5); //异步通讯

class CNetworkEvent : public QEvent
{
public:
    CNetworkEvent(QEvent::Type type,NetworkDesc_t netInfo):
        QEvent(type),
        mNetInfo(netInfo)
    {
    }

public:
  NetworkDesc_t mNetInfo;
};


class CNetworkThread : public QObject
{
    Q_OBJECT
public:
    explicit CNetworkThread(QObject *parent = 0);

    void init();
    void sendNetWorkEvent(NetworkDesc_t networkInfo);
    bool sendNetworkProc(NetworkDesc_t networkInfo);
    
signals:
    
public slots:
    
protected:
    void customEvent(QEvent *event);

private:

    DECLARE_SINGLETON_CLASS(CNetworkThread)
};

typedef Singleton<CNetworkThread> SCNetworkThread;

#endif // NETWORKTHREAD_H
