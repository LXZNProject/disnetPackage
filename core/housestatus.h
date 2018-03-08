#ifndef HOUSESTATUS_H
#define HOUSESTATUS_H


#include "status.h"
#include "housepage.h"
class CHouseStatus : public CStatus
{
    Q_OBJECT
public:
    CHouseStatus(STATUS status = ST_HOUSE_PACK_MENU);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void cardInputProc(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();
private:
    housepage *page;

   // senderpage *page;
};

#endif // HOUSESTATUS_H
