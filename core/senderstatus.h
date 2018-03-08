#ifndef SENDERSTATUS_H
#define SENDERSTATUS_H


#include "status.h"
#include "senderpage.h"
class CSenderStatus : public CStatus
{
    Q_OBJECT
public:
    CSenderStatus(STATUS status = ST_SENDER_PACK_MENU);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();
private:

    senderpage *page;

};

#endif // SENDERSTATUS_H
