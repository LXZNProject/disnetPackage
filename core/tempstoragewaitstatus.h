#ifndef TEMPSTORAGEWAITSTATUS_H
#define TEMPSTORAGEWAITSTATUS_H
#include "status.h"
#include "tempstoragewait.h"
class CtempStorageWaitStatus: public CStatus
{
public:
    CtempStorageWaitStatus(STATUS status = ST_TEMP_STORAGE_WAIT);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);

private:
    tempStorageWait *page;
};
#endif
