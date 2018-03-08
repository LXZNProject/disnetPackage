#ifndef NETWORKCONIGSTATUS_H
#define NETWORKCONIGSTATUS_H
#include "status.h"
#include "networkconfigure.h"
class CNetworkConigStatus: public CStatus
{
public:
    CNetworkConigStatus(STATUS status = ST_NETWORK_SET);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);

private:

       networkConfigure *myPage;
};

#endif // NETWORKCONIGSTATUS_H
