#ifndef SERVERSTATUS_H
#define SERVERSTATUS_H


#include "status.h"

#include "serverpage.h"
class CServerStatus : public CStatus
{
    Q_OBJECT
public:
    CServerStatus(STATUS status = ST_HOUSE_SERVICE);

    void enter(QByteArray param);
    void leave(QByteArray param);
public slots:
    void jumpPage();
    void leaveSlots();
private:
    serverpage *page;
};

#endif // SERVERSTATUS_H
