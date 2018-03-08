#ifndef BALANCESTATUS_H
#define BALANCESTATUS_H
#include "status.h"
#include "balancepage.h"

class CBalanceStatus: public CStatus
{
public:
    CBalanceStatus(STATUS status = ST_QUERY_BALANCE);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);


private:
    balancePage *balancepage;
};

#endif // BALANCESTATUS_H
