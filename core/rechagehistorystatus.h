#ifndef RECHAGEHISTORYSTATUS_H
#define RECHAGEHISTORYSTATUS_H
#include "status.h"
#include "rechagehistorypage.h"
class CRechageHistoryStatus: public CStatus
{
public:
    CRechageHistoryStatus(STATUS status = ST_RECHARGE_QUERY);


    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);
private:
    rechageHistoryPage *myrechageHistoryPage;
};

#endif // RECHAGEHISTORYSTATUS_H
