#ifndef TARIFFSTATUS_H
#define TARIFFSTATUS_H
#include "status.h"
#include "tariffpage.h"
class CtariffStatus: public CStatus
{
    Q_OBJECT
public:
    CtariffStatus(STATUS status = ST_TARIFF);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void cardInputProc(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();
private:
    tariffPage  * mtariffPage;

};

#endif // TARIFFSTATUS_H
