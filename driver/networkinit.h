#ifndef NETWORKINIT_H
#define NETWORKINIT_H
#include <QObject>
#include "singleton.h"

class networkInit: public QObject
{
    Q_OBJECT
public:
    explicit networkInit(QObject *parent = 0);

    QString       getNetworkMethods();
    QStringList getNetwork();
    void            setNetwork(QStringList setValue);
    void            connetNetwork();
    void            disconnectNetwork();


private:
    void init();
private:
    DECLARE_SINGLETON_CLASS(networkInit)
};

typedef Singleton<networkInit> SCNetworkInit;

#endif // NETWORKINIT_H
