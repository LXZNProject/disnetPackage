#ifndef RECHARGESTATUS_H
#define RECHARGESTATUS_H
#include "status.h"
#include "recharge.h"

class CRechargeStatus: public CStatus
{
    Q_OBJECT
public:
    CRechargeStatus(STATUS status = ST_RECHARGE);
    void enter(QByteArray param);
    void leave(QByteArray param);


public slots:
    void resetStart();
    void jumpPage();
private:
   recharge *rechargePage;
};

#endif // RECHARGESTATUS_H
