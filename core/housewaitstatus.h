#ifndef HOUSEWAITSTATUS_H
#define HOUSEWAITSTATUS_H


#include "status.h"
#include "housewaitpage.h"
class CHouseWaitStatus : public CStatus
{
public:
    CHouseWaitStatus(STATUS status = ST_HOUSE_PACK_WAIT);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);


private:
    housewaitpage *page;
};

#endif // HOUSEWAITSTATUS_H
