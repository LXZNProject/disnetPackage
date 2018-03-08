#ifndef RECHARGEDIALOGSTATUS_H
#define RECHARGEDIALOGSTATUS_H
#include "status.h"
#include "rechargedialog.h"
class CRechargedialogStatus: public CStatus
{
    Q_OBJECT
public:
    CRechargedialogStatus(STATUS status = ST_RECHARGE_DIAG);
    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void jumpPage();
    void leaveSlots();
private:
   rechargeDialog *rechargeDialogPage;
};

#endif // RECHARGEDIALOGSTATUS_H
