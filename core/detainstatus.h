#ifndef DETAINSTATUS_H
#define DETAINSTATUS_H

#include "status.h"
#include "detainpage.h"
class CDetainStatus : public CStatus
{
     Q_OBJECT
public:
    CDetainStatus(STATUS status = ST_DETAIN_PACK);

    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();
private:
    detainpage *page;
};

#endif // DETAINSTATUS_H
