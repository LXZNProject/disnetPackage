#ifndef SENDERWAITSTATUS_H
#define SENDERWAITSTATUS_H

#include "status.h"
#include "senderwaitpage.h"

class CSenderWaitStatus : public CStatus
{
    Q_OBJECT
public:
    CSenderWaitStatus(STATUS status = ST_SENDER_PACK_WAIT);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);


private:
    senderwaitpage *page;
};

#endif // SENDERWAITSTATUS_H
